#include "Controller.h"
#include <QInputDialog>
#include <QMessageBox>

/**
 * @brief Constructs the TransactionController and establishes connections
 *
 * Sets up connections between view signals and controller slot methods
 * for authentication, profile management, and category operations.
 *
 * @param parent Parent QObject
 */
TransactionController::TransactionController(QObject* parent) : QObject(parent)
{
    // Connect authentication dialog signals
    connect(&loginDialog, &LoginDialog::loginRequested,
        this, &TransactionController::handleLoginRequested);
    connect(&loginDialog, &LoginDialog::registerRequested,
        this, &TransactionController::handleRegisterRequested);

    // Connect profile dialog signals
    connect(&profileDialog, &ProfileDialog::profileSelected,
        this, &TransactionController::handleProfileSelected);
    connect(&profileDialog, &ProfileDialog::addProfileRequested,
        this, &TransactionController::handleAddProfileRequested);
    connect(&profileDialog, &ProfileDialog::removeProfileRequested,
        this, &TransactionController::handleRemoveProfileRequested);

    // Connect category dialog signals
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory,
        this, &TransactionController::handleCategorySelected);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory,
        this, &TransactionController::handleAddCategoryRequested);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory,
        this, &TransactionController::handleDeleteCategoryRequested);
}

/**
 * @brief Starts the application by displaying the login screen
 */
void TransactionController::run()
{
    loginDialog.show();
}

/**
 * @brief Handles user login attempt
 * @param username User's login name
 * @param password User's password
 */
void TransactionController::handleLoginRequested(const QString& username, const QString& password)
{
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        QMessageBox::warning(&loginDialog,
            tr("Login"),
            tr("Please enter username and password."));
        return;
    }

    int userId = userRepository.authenticateUser(username, password);
    if (userId < 0) {
        QMessageBox::warning(&loginDialog, tr("Login"), tr("Invalid username or password"));
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
void TransactionController::handleRegisterRequested(const QString& username, const QString& password)
{
    if (!userRepository.addUser(username, password)) {
        QMessageBox::warning(&loginDialog, tr("Registration"), tr("Failed to create user"));
        return;
    }

    QMessageBox::information(&loginDialog, tr("Registration"),
        tr("User created. You can now log in."));
}

/**
 * @brief Displays profiles associated with the current user
 */
void TransactionController::showProfilesForCurrentUser()
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
void TransactionController::handleProfileSelected(int profileId)
{
    currentProfileId = profileId;
    profileDialog.hide();

    // Initialize main window connections on first profile selection
    if (!mainWindowInitialized) {
        connect(&mainWindowView, &MainWindow::addTransactionRequested,
            this, &TransactionController::handleAddTransactionRequested);
        connect(&mainWindowView, &MainWindow::deleteTransactionRequested,
            this, &TransactionController::handleDeleteTransactionRequested);
        connect(&mainWindowView, &MainWindow::manageCategoriesRequested,
            this, &TransactionController::showCategorySelectionDialog);

        mainWindowInitialized = true;
    }

    refreshTransactionsView();
    mainWindowView.show();
}

/**
 * @brief Handles creation of a new budget profile
 * @param name Name for the new profile
 */
void TransactionController::handleAddProfileRequested(const QString& name)
{
    if (!profilesRepository.addProfile(currentUserId, name)) {
        QMessageBox::warning(&profileDialog, tr("Profile"),
            tr("Failed to add profile"));
        return;
    }

    showProfilesForCurrentUser();
}

/**
 * @brief Handles deletion of a budget profile
 * @param profileId ID of the profile to delete
 */
void TransactionController::handleRemoveProfileRequested(int profileId)
{
    if (!profilesRepository.removeProfileById(profileId)) {
        QMessageBox::warning(&profileDialog, tr("Profile"),
            tr("Failed to delete profile"));
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
void TransactionController::refreshTransactionsView()
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

    mainWindowView.setTransactionRows(tableRows);
}

/**
 * @brief Handles creation of a new transaction
 */
void TransactionController::handleAddTransactionRequested()
{
    if (currentProfileId < 0) {
        QMessageBox::warning(&mainWindowView, tr("Error"),
            tr("Please select a profile first."));
        return;
    }

    bool correctData = false;

    // Get transaction name
    QString name = QInputDialog::getText(
        &mainWindowView,
        tr("New transaction"),
        tr("Name:"),
        QLineEdit::Normal,
        "",
        &correctData
    );
    if (!correctData || name.trimmed().isEmpty())
        return;

    // Get transaction amount
    double amount = QInputDialog::getDouble(
        &mainWindowView,
        tr("New transaction"),
        tr("Amount:"),
        0.0,
        0,
        1e9,
        2,
        &correctData
    );
    if (!correctData)
        return;

    // Get transaction description
    QString description = QInputDialog::getText(
        &mainWindowView,
        tr("New transaction"),
        tr("Description:"),
        QLineEdit::Normal,
        "",
        &correctData
    );
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

    if (!transactionRepository.add(newTransaction)) {
        QMessageBox::warning(
            &mainWindowView,
            tr("Error"),
            tr("Failed to add transaction to database.")
        );
        return;
    }

    refreshTransactionsView();
}

/**
 * @brief Handles deletion of a transaction
 */
void TransactionController::handleDeleteTransactionRequested()
{
    int transactionId = mainWindowView.selectedTranstacionId();
    if (transactionId < 0) {
        QMessageBox::information(
            &mainWindowView,
            tr("Delete transaction"),
            tr("No transaction selected.")
        );
        return;
    }

    if (!transactionRepository.removeById(transactionId)) {
        QMessageBox::warning(
            &mainWindowView,
            tr("Error"),
            tr("Failed to delete transaction from database.")
        );
        return;
    }

    refreshTransactionsView();
}

/**
 * @brief Opens the category management dialog
 */
void TransactionController::showCategorySelectionDialog()
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
void TransactionController::openCategoryDialog(bool withSelectButton)
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
void TransactionController::handleCategorySelected(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

/**
 * @brief Prompts user to select a category for a transaction
 * @return ID of selected category, or -1 if cancelled
 */
int TransactionController::askUserForCategoryId()
{
    selectedCategoryIdForTransaction = -1;
    openCategoryDialog(true);
    return selectedCategoryIdForTransaction;
}

/**
 * @brief Handles creation of a new category
 * @param categoryName Name of the category to create
 */
void TransactionController::handleAddCategoryRequested(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, currentProfileId))
    {
        QMessageBox::warning(&categoryDialog, tr("Error"),
            tr("Could not add category"));
    }

    refreshCategoryDialogList();
}

/**
 * @brief Handles deletion of a category
 * @param categoryId ID of the category to delete
 */
void TransactionController::handleDeleteCategoryRequested(int categoryId)
{
    if (categoryId == 1 || !categoryRepository.removeCategoryUsingId(categoryId)) {
        QMessageBox::warning(&categoryDialog, tr("Error"),
            tr("Could not delete category"));
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
void TransactionController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}