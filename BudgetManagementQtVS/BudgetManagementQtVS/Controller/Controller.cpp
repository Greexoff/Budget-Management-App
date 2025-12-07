#include "Controller.h"
#include <QInputDialog>


/**
 * @brief Constructs the TransactionController and establishes connections
 *
 * Sets up connections between view signals and controller slot methods
 * for authentication, profile management, and category operations.
 *
 * @param parent Parent QObject
 */
Controller::Controller(QObject* parent) : QObject(parent)
{
    // Connect authentication dialog signals
    connect(&loginDialog, &LoginDialog::loginRequested,
        this, &Controller::handleLoginRequested);
    connect(&loginDialog, &LoginDialog::registerRequested,
        this, &Controller::handleRegisterRequested);

    // Connect profile dialog signals
    connect(&profileDialog, &ProfileDialog::profileSelected,
        this, &Controller::handleProfileSelected);
    connect(&profileDialog, &ProfileDialog::addProfileRequested,
        this, &Controller::handleAddProfileRequested);
    connect(&profileDialog, &ProfileDialog::removeProfileRequested,
        this, &Controller::handleRemoveProfileRequested);

    // Connect category dialog signals
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &Controller::handleCategorySelected);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &Controller::handleAddCategoryRequested);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &Controller::handleDeleteCategoryRequested);
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
void Controller::handleLoginRequested(const QString& username, const QString& password)
{
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        const QString header = tr("Log in");
        const QString message = tr("Insert username and password.");
        loginDialog.displayLoginMessage(header, message, "error");
        return;
    }

    int userId = userRepository.authenticateUser(username, password);
    if (userId < 0) {
        const QString header = tr("Log in");
        const QString message = tr("Invalid username or password.");
        loginDialog.displayLoginMessage(header, message, "error");
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
void Controller::handleRegisterRequested(const QString& username, const QString& password)
{
    if (!userRepository.addUser(username, password)) {
        const QString header = tr("Registration");
        const QString message = tr("Failed to create user.");
        loginDialog.displayLoginMessage(header, message, "error");
        return;
    }
    const QString header = tr("Registration");
    const QString message = tr("Account created successfully.");
    loginDialog.displayLoginMessage(header, message, "message");
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
 * @brief Handles selection of a budget profile
 *
 * Sets the current profile and initializes the main window
 * with the profile's transaction data.
 *
 * @param profileId ID of the selected profile
 */
void Controller::handleProfileSelected(int profileId)
{
    currentProfileId = profileId;
    profileDialog.hide();

    // Initialize main window connections on first profile selection
    if (!mainWindowInitialized) {
        connect(&TransactionWindowView, &TransactionWindow::addTransactionRequested,
            this, &Controller::handleAddTransactionRequested);
        connect(&TransactionWindowView, &TransactionWindow::deleteTransactionRequested,
            this, &Controller::handleDeleteTransactionRequested);
        connect(&TransactionWindowView, &TransactionWindow::manageCategoriesRequested,
            this, &Controller::showCategorySelectionDialog);

        mainWindowInitialized = true;
    }

    refreshTransactionsView();
    TransactionWindowView.show();
}

/**
 * @brief Handles creation of a new budget profile
 * @param name Name for the new profile
 */
void Controller::handleAddProfileRequested(const QString& name)
{
    if (!profilesRepository.addProfile(currentUserId, name)) {
        const QString header = tr("New profile");
        const QString message = tr("Failed to add a profile.");
        profileDialog.displayProfileMessage(header, message, "error");
        return;
    }

    showProfilesForCurrentUser();
}

/**
 * @brief Handles deletion of a budget profile
 * @param profileId ID of the profile to delete
 */
void Controller::handleRemoveProfileRequested(int profileId)
{
    if (!profilesRepository.removeProfileById(profileId)) {
        const QString header = tr("Delete profile");
        const QString message = tr("Failed to delete a profile.");
        profileDialog.displayProfileMessage(header, message, "error");
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

    QVector<Transaction> allTransactions = transactionRepository.getAllForProfile(currentProfileId);
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
        rowData << categoryRepository.getNameOfCategoryBasedOnId(transaction.getCategoryId());

        tableRows.append(rowData);
    }

    TransactionWindowView.setTransactionRows(tableRows);
}

/**
 * @brief Handles creation of a new transaction
 */
void Controller::handleAddTransactionRequested()
{
    if (currentProfileId < 0) {
        const QString header = tr("New transaction");
        const QString message = tr("Please select a profile first.");
        TransactionWindowView.displayTransactionMessage(header, message, "error");
        return;
    }

    bool correctData = false;

    // Get transaction name
    QString name = TransactionWindowView.getTransactionNameFromInput(correctData);
    if (!correctData || name.trimmed().isEmpty())
        return;

    // Get transaction amount
    double amount = TransactionWindowView.getTransactionAmountFromInput(correctData);
    if (!correctData)
        return;

    // Get transaction description
    QString description = TransactionWindowView.getTransactionDescriptionFromInput(correctData);
    if (!correctData)
        return;

    // Determine transaction type based on amount sign
    TransactionType type = (amount > 0.0) ? EXPENSE : INCOME;

    // Prompt user to select a category
    int categoryId = askUserForCategoryId();
    if (categoryId < 0) {
        categoryId = 1; // Default category ("None")
    }

    // Create and save transaction
    Transaction newTransaction(
        0,                      // ID will be assigned by database
        name,
        QDate::currentDate(),
        description,
        amount,
        type,
        categoryId,
        currentProfileId
    );

    if (!transactionRepository.add(newTransaction))
    {
        const QString header = tr("New transaction");
        const QString message = tr("Failed to add transaction.");
        TransactionWindowView.displayTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
}

/**
 * @brief Handles deletion of a transaction
 */
void Controller::handleDeleteTransactionRequested()
{
    int transactionId = TransactionWindowView.selectedTranstacionId();
    if (transactionId < 0) {
        const QString header = tr("Delete transaction");
        const QString message = tr("No transaction selected.");
        TransactionWindowView.displayTransactionMessage(header, message, "error");
        return;
    }

    if (!transactionRepository.removeById(transactionId)) 
    {
        const QString header = tr("Delete transaction");
        const QString message = tr("Failed to delete transaction.");
        TransactionWindowView.displayTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
}

/**
 * @brief Opens the category management dialog
 */
void Controller::showCategorySelectionDialog()
{
    openCategoryDialog(false);
}

/**
 * @brief Opens the category dialog in specified mode
 * @param withSelectButton True for selection mode, false for management mode
 *
 * In selection mode, the dialog includes a "Select" button for
 * choosing a category for a transaction. In management mode,
 * only add/delete operations are available.
 */
void Controller::openCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllCategories(currentProfileId);
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}

/**
 * @brief Handles category selection from the dialog
 * @param categoryId ID of the selected category
 */
void Controller::handleCategorySelected(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

/**
 * @brief Prompts user to select a category for a transaction
 * @return ID of selected category, or -1 if cancelled
 */
int Controller::askUserForCategoryId()
{
    selectedCategoryIdForTransaction = -1;
    openCategoryDialog(true);
    return selectedCategoryIdForTransaction;
}

/**
 * @brief Handles creation of a new category
 * @param categoryName Name of the category to create
 */
void Controller::handleAddCategoryRequested(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, currentProfileId))
    {
        const QString header = tr("New category");
        const QString message = tr("Failed to add a category.");
        categoryDialog.displayCategoryMessage(header, message, "error");
    }

    refreshCategoryDialogList();
}

/**
 * @brief Handles deletion of a category
 * @param categoryId ID of the category to delete
 */
void Controller::handleDeleteCategoryRequested(int categoryId)
{
    if (categoryId == 1 || !categoryRepository.removeCategoryUsingId(categoryId)) 
    {
        const QString header = tr("Delete category");
        const QString message = tr("Failed to delete a category.");
        categoryDialog.displayCategoryMessage(header, message, "error");
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
    QVector<Category> categories = categoryRepository.getAllCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}