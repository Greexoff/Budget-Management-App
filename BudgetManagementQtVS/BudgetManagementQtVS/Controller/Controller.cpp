#include "Controller.h"
#include <QInputDialog>

/**
 * @brief Constructs the Controller and establishes connections
 *
 * Sets up connections between view signals and controller slot methods
 * for authentication, profile management, and category operations.
 * 
 * @param loginDialogRef Reference to the login dialog view
 * @param profileDialogRef Reference to the profile dialog view
 * @param categorySelectionViewRef Reference to the category selection view
 * @param transactionWindowViewRef Reference to the transaction window view
 * @param userRepositoryRef Reference to the user repository
 * @param profileRepositoryRef Reference to the profile repository
 * @param transactionRepositoryRef Reference to the transaction repository
 * @param categoryRepositoryRef Reference to the category repository
 * @param parent Parent QObject
 */
Controller::Controller(
    LoginDialog& loginDialogRef,
    ProfileDialog& profileDialogRef,
    CategorySelectionView& categorySelectionViewRef,
    TransactionWindow& transactionWindowViewRef,
    UserRepository& userRepositoryRef,
    ProfilesRepository& profileRepositoryRef,
    TransactionRepository& transactionRepositoryRef,
    CategoryRepository& categoryRepositoryRef,
    QObject* parent) : QObject(parent), 
    loginDialog(loginDialogRef),
    profileDialog(profileDialogRef),
    categoryDialog(categorySelectionViewRef),
    TransactionWindowView(transactionWindowViewRef),
    userRepository(userRepositoryRef),
    profilesRepository(profileRepositoryRef),
    transactionRepository(transactionRepositoryRef),
    categoryRepository(categoryRepositoryRef)
{
    // Connect authentication dialog signals
    connect(&loginDialog, &LoginDialog::loginRequested,
        this, &Controller::handleLoginRequest);
    connect(&loginDialog, &LoginDialog::registerRequested,
        this, &Controller::handleRegisterRequest);

    // Connect profile dialog signals
    connect(&profileDialog, &ProfileDialog::profileSelected,
        this, &Controller::handleProfileSelection);
    connect(&profileDialog, &ProfileDialog::addProfileRequested,
        this, &Controller::handleAddProfileRequest);
    connect(&profileDialog, &ProfileDialog::removeProfileRequested,
        this, &Controller::handleRemoveProfileRequest);

    // Connect category dialog signals
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &Controller::handleCategorySelection);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &Controller::handleAddCategoryRequest);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &Controller::handleDeleteCategoryRequest);
}

/**
 * @brief Starts the application by displaying the login screen
 */
void Controller::run()
{
    loginDialog.show();
}

/**
 * @brief Handles user login attempt
 * @param username User's login name
 * @param password User's password
 */
void Controller::handleLoginRequest(const QString& username, const QString& password)
{
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        const QString header = tr("Log in");
        const QString message = tr("Insert username and password.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }

    int userId = userRepository.getUserIdBasedOnUsername(username, password);
    if (userId < 0) {
        const QString header = tr("Log in");
        const QString message = tr("Invalid username or password.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }
    currentUserId = userId;
    loginDialog.hide();
    showProfilesForCurrentUser();
}

/**
 * @brief Handles new user registration
 * @param username Desired username
 * @param password Desired password
 */
void Controller::handleRegisterRequest(const QString& username, const QString& password)
{
    if (!userRepository.addUser(username, password)) {
        const QString header = tr("Registration");
        const QString message = tr("Failed to create user.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }
    const QString header = tr("Registration");
    const QString message = tr("Account created successfully.");
    loginDialog.showLoginMessage(header, message, "message");
}

/**
 * @brief Displays profiles associated with the current user
 */
void Controller::showProfilesForCurrentUser()
{
    QVector<Profile> profiles = profilesRepository.getProfilesByUserId(currentUserId);
    profileDialog.setProfiles(profiles);
    profileDialog.exec();
}

/**
 * @brief Handles selection of a profile
 *
 * Sets the current profile and initializes the main window
 * with the profile's transaction data.
 *
 * @param profileId ID of the selected profile
 */
void Controller::handleProfileSelection(int profileId)
{
    currentProfileId = profileId;
    profileDialog.hide();

    // Initialize main window connections on first profile selection
    if (!mainWindowInitialized) {
        connect(&TransactionWindowView, &TransactionWindow::addTransactionRequest,
            this, &Controller::handleAddTransactionRequest);
        connect(&TransactionWindowView, &TransactionWindow::deleteTransactionRequest,
            this, &Controller::handleDeleteTransactionRequest);
        connect(&TransactionWindowView, &TransactionWindow::showCategoriesRequest,
            this, &Controller::handleShowCategorySelectionRequest);

        mainWindowInitialized = true;
    }

    refreshTransactionsView();
    TransactionWindowView.show();
}

/**
 * @brief Handles creation of a new profile
 * @param name Name for the new profile
 */
void Controller::handleAddProfileRequest(const QString& name)
{
    if (!profilesRepository.addProfile(currentUserId, name)) {
        const QString header = tr("New profile");
        const QString message = tr("Failed to add a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    showProfilesForCurrentUser();
}

/**
 * @brief Handles deletion of a profile
 * @param profileId ID of the profile to delete
 */
void Controller::handleRemoveProfileRequest(int profileId)
{
    if (!profilesRepository.removeProfileById(profileId)) {
        const QString header = tr("Delete profile");
        const QString message = tr("Failed to delete a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    showProfilesForCurrentUser();
}

/**
 * @brief Refreshes the transaction display in the main window
 *
 * Retrieves all transactions for the current profile and updates
 * the main window's table view with formatted data including
 * category names resolved from category IDs.
 */
void Controller::refreshTransactionsView()
{
    if (currentProfileId < 0)
        return;

    QVector<Transaction> allTransactions = transactionRepository.getAllProfileTransaction(currentProfileId);
    QVector<QStringList> tableRows;

    for (const auto& transaction : allTransactions) {
        QStringList rowData;
        rowData << QString::number(transaction.getTransactionId())
            << transaction.getTransactionName()
            << transaction.getTransactionDate().toString("yyyy-MM-dd")
            << transaction.getTransactionDescription()
            << QString::number(transaction.getTransactionAmount(), 'f', 2);

        // Determine transaction type based on amount
        QString typeString = (transaction.getTransactionAmount() > 0.0) ? "Expense" : "Income";
        rowData << typeString;

        // Resolve category name from ID
        rowData << categoryRepository.getCategoryNameById(transaction.getCategoryId());

        tableRows.append(rowData);
    }

    TransactionWindowView.setTransactionRows(tableRows);
}

/**
 * @brief Handles creation of a new transaction
 */
void Controller::handleAddTransactionRequest()
{
    if (currentProfileId < 0) {
        const QString header = tr("New transaction");
        const QString message = tr("Please select a profile first.");
        TransactionWindowView.showTransactionMessage(header, message, "error");
        return;
    }

    bool correctData = false;

    TransactionBuilder builder;
    builder.withProfileId(currentProfileId);

    QString name = TransactionWindowView.getTransactionNameFromInput(correctData);
    if (!correctData || name.trimmed().isEmpty()) { return; }
    builder.withName(name);

    double amount = TransactionWindowView.getTransactionAmountFromInput(correctData);
    builder.withAmount(amount);

    QString description = TransactionWindowView.getTransactionDescriptionFromInput(correctData);
    builder.withDescription(description);

    int categoryId = getCategoryIdFromInput();
    builder.withCategoryId(categoryId);

    Transaction newTransaction = builder.build();
    if (!transactionRepository.addTransaction(newTransaction))
    {
        const QString header = tr("New transaction");
        const QString message = tr("Failed to add transaction.");
        TransactionWindowView.showTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
}

/**
 * @brief Handles deletion of a transaction
 */
void Controller::handleDeleteTransactionRequest()
{
    int transactionId = TransactionWindowView.getSelectedTransactionId();
    if (transactionId < 0) {
        const QString header = tr("Delete transaction");
        const QString message = tr("No transaction selected.");
        TransactionWindowView.showTransactionMessage(header, message, "error");
        return;
    }

    if (!transactionRepository.removeTransactionById(transactionId)) 
    {
        const QString header = tr("Delete transaction");
        const QString message = tr("Failed to delete transaction.");
        TransactionWindowView.showTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
}

/**
 * @brief Displays the category management dialog
 */
void Controller::handleShowCategorySelectionRequest()
{
    showCategoryDialog(false);
}

/**
 * @brief Displays the category dialog in specified mode
 * @param withSelectButton True for selection mode, false for management mode
 *
 * In selection mode, the dialog includes a "Select" button for
 * choosing a category for a transaction. In management mode,
 * only add/delete operations are available.
 */
void Controller::showCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}

/**
 * @brief Handles category selection from the dialog
 * @param categoryId ID of the selected category
 */
void Controller::handleCategorySelection(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

/**
 * @brief Prompts user to select a category for a transaction
 * @return ID of selected category, or -1 if cancelled
 */
int Controller::getCategoryIdFromInput()
{
    selectedCategoryIdForTransaction = -1;
    showCategoryDialog(true);
    return selectedCategoryIdForTransaction;
}

/**
 * @brief Handles creation of a new category
 * @param categoryName Name of the category to create
 */
void Controller::handleAddCategoryRequest(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, currentProfileId))
    {
        const QString header = tr("New category");
        const QString message = tr("Failed to add a category.");
        categoryDialog.showCategoryMessage(header, message, "error");
    }

    refreshCategoryDialogList();
}

/**
 * @brief Handles deletion of a category
 * @param categoryId ID of the category to delete
 */
void Controller::handleDeleteCategoryRequest(int categoryId)
{
    if (!categoryRepository.removeCategoryById(categoryId)) 
    {
        const QString header = tr("Delete category");
        const QString message = tr("Failed to delete a category.");
        categoryDialog.showCategoryMessage(header, message, "error");
    }

    refreshCategoryDialogList();

    // Refresh transaction view to update any category name displays
    if (currentProfileId >= 0 && mainWindowInitialized) {
        refreshTransactionsView();
    }
}

/**
 * @brief Refreshes the category list in the dialog
 *
 * Updates the category dialog with the current list of categories
 * from the database for the active profile.
 */
void Controller::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllProfileCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}