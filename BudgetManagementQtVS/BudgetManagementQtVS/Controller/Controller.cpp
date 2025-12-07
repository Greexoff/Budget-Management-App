#include "Controller.h"
#include <QInputDialog>
#include <QMessageBox>

/**
 * @brief Constructs the controller and connects signals to slots
 * 
 * This constructor sets up all necessary connections between view signals
 * and controller slot methods, enabling the MVC communication pattern.
 * 
 * @param parent Parent QObject
 */
TransactionController::TransactionController(QObject* parent): QObject(parent)
{
    // Connect authentication signals
    connect(&loginDialog, &LoginDialog::loginRequested,this, &TransactionController::handleLoginRequested);
    connect(&loginDialog, &LoginDialog::registerRequested,this, &TransactionController::handleRegisterRequested);

    // Connect profile management signals
    connect(&profileDialog, &ProfileDialog::profileSelected,this, &TransactionController::handleProfileSelected);
    connect(&profileDialog, &ProfileDialog::addProfileRequested,this, &TransactionController::handleAddProfileRequested);
    connect(&profileDialog, &ProfileDialog::removeProfileRequested,this, &TransactionController::handleRemoveProfileRequested);
    
    // Connect category management signals
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory, this, &TransactionController::handleCategorySelected);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory, this, &TransactionController::handleAddCategoryRequested);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory, this, &TransactionController::handleDeleteCategoryRequested);   
}

/**
 * @brief Starts the application flow
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
            tr("Logowanie"),
            tr("Podaj login i hasło."));
        return;
    }

    int userId = userRepository.authenticateUser(username, password);
    if (userId < 0) {
        QMessageBox::warning(&loginDialog, tr("Logowanie"), tr("Zły login lub hasło"));
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
        QMessageBox::warning(&loginDialog, tr("Rejestracja"), tr("Nie udało się utworzyć użytkownika"));
        return;
    }

    QMessageBox::information(&loginDialog, tr("Rejestracja"),
        tr("Użytkownik utworzony. Możesz się zalogować."));
}

/**
 * @brief Displays profiles associated with current user
 */
void TransactionController::showProfilesForCurrentUser()
{
    QVector<Profile> profiles = profilesRepository.getProfilesByUserId(currentUserId);
    profileDialog.setProfiles(profiles);
    profileDialog.exec();
}

/**
 * @brief Handles profile selection
 * @param profileId ID of the selected profile
 */
void TransactionController::handleProfileSelected(int profileId)
{
    currentProfileId = profileId;
    profileDialog.hide();

    if (!mainWindowInitialized) {
        connect(&mainWindowView, &MainWindow::addTransactionRequested,this, &TransactionController::handleAddTransactionRequested);

        connect(&mainWindowView, &MainWindow::deleteTransactionRequested,this, &TransactionController::handleDeleteTransactionRequested);

        connect(&mainWindowView, &MainWindow::manageCategoriesRequested, this, &TransactionController::showCategorySelectionDialog);

        mainWindowInitialized = true;
    }

    refreshTransactionsView();
    mainWindowView.show();
}

/**
 * @brief Handles adding a new profile
 * @param name Name of the new profile
 */
void TransactionController::handleAddProfileRequested(const QString& name)
{
    if (!profilesRepository.addProfile(currentUserId, name)) {
        QMessageBox::warning(&profileDialog, tr("Profil"),
            tr("Nie udało się dodać profilu"));
        return;
    }

    showProfilesForCurrentUser();
}

/**
 * @brief Handles profile removal
 * @param profileId ID of profile to remove
 */
void TransactionController::handleRemoveProfileRequested(int profileId)
{
    if (!profilesRepository.removeProfileById(profileId)) {
        QMessageBox::warning(&profileDialog, tr("Profil"),
            tr("Nie udało się usunąć profilu"));
        return;
    }

    showProfilesForCurrentUser();
}

/**
 * @brief Refreshes the transaction display in main window
 */
void TransactionController::refreshTransactionsView()
{
    if (currentProfileId < 0)
        return;

    QVector<Transaction> all = transactionRepository.getAllForProfile(currentProfileId);
    QVector<QStringList> rows;

    for (const auto& t : all) {
        QStringList row;
        row << QString::number(t.getTransactionId())
            << t.getTransactionName()
            << t.getTransactionDate().toString("yyyy-MM-dd")
            << t.getTransactionDescription()
            << QString::number(t.getTransactionAmount(), 'f', 2);

        QString typeStr = (t.getTransactionAmount() > 0.0) ? "Wydatek" : "Przychód";
        row << typeStr;
        row << categoryRepository.getNameOfCategoryBasedOnId(t.getCategoryId());
        rows.append(row);
    }

    mainWindowView.setTransactionRows(rows);
}

/**
 * @brief Handles adding a new transaction
 */
void TransactionController::handleAddTransactionRequested()
{
    if (currentProfileId < 0) {
        QMessageBox::warning(&mainWindowView, tr("Błąd"),
            tr("Najpierw wybierz profil."));
        return;
    }

    bool correctData = false;

    // Get transaction name
    QString name = QInputDialog::getText(
        &mainWindowView,
        tr("Nowa transakcja"),
        tr("Nazwa:"),
        QLineEdit::Normal,
        "",
        &correctData
    );
    if (!correctData || name.trimmed().isEmpty())
        return;

    // Get transaction amount
    double amount = QInputDialog::getDouble(
        &mainWindowView,
        tr("Nowa transakcja"),
        tr("Kwota:"),
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
        tr("Nowa transakcja"),
        tr("Opis:"),
        QLineEdit::Normal,
        "",
        &correctData
    );
    if (!correctData)
        return;

    // Determine transaction type (expense if positive, income if negative)
    TransactionType type = (amount > 0.0) ? EXPENSE : INCOME;
    
    // Get category ID from user
    int categoryId = askUserForCategoryId();
    if (categoryId < 0) {
        categoryId = 1; // default category
    }

    // Create new transaction object
    Transaction t(
        0,                      
        name,
        QDate::currentDate(),
        description,
        amount,
        type,
        categoryId,
        currentProfileId        
    );

    // Save transaction to database
    if (!transactionRepository.add(t)) {
        QMessageBox::warning(
            &mainWindowView,
            tr("Błąd"),
            tr("Nie udało się dodać transakcji do bazy.")
        );
        return;
    }

    refreshTransactionsView();
}

/**
 * @brief Handles transaction deletion
 */
void TransactionController::handleDeleteTransactionRequested()
{
    int id = mainWindowView.selectedTranstacionId();
    if (id < 0) {
        QMessageBox::information(
            &mainWindowView,
            tr("Usuń transakcję"),
            tr("Nie wybrano żadnej transakcji.")
        );
        return;
    }

    if (!transactionRepository.removeById(id)) {
        QMessageBox::warning(
            &mainWindowView,
            tr("Błąd"),
            tr("Nie udało się usunąć transakcji z bazy.")
        );
        return;
    }

    refreshTransactionsView();
}

/**
 * @brief Opens category management dialog
 */
void TransactionController::showCategorySelectionDialog()
{

    openCategoryDialog(false);
    /*QVector<Category> categories = categoryRepository.getAllCategories();
    categoryDialog.setCategories(categories);
    categoryDialog.exec();*/
}

/**
 * @brief Opens category dialog with optional selection button
 * @param withSelectButton True to show selection button, false for management only
 */
void TransactionController::openCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllCategories();
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}

/**
 * @brief Handles category selection
 * @param categoryId ID of selected category
 */
void TransactionController::handleCategorySelected(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}

/**
 * @brief Show user to select a category
 * @return Selected category ID or -1 if cancelled
 */
int TransactionController::askUserForCategoryId()
{
    selectedCategoryIdForTransaction = -1;
    openCategoryDialog(true);   

    return selectedCategoryIdForTransaction;   
}

/**
 * @brief Handles adding a new category
 * @param categoryName Name of category to add
 */
void TransactionController::handleAddCategoryRequested(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName))
    {
       QMessageBox::warning(&categoryDialog, tr("Error"),
       tr("Couldn't add category"));
    }
    
    refreshCategoryDialogList();
}

/**
 * @brief Handles category deletion
 * @param categoryId ID of category to delete
 */
void TransactionController::handleDeleteCategoryRequested(int categoryId)
{
    if (categoryId==1 || !categoryRepository.removeCategoryUsingId(categoryId)) {
        QMessageBox::warning(&categoryDialog, tr("Error"),
            tr("Couldn't delete category"));
    }
    
    refreshCategoryDialogList();
}

/**
 * @brief Refreshes category list in dialog
 */
void TransactionController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllCategories();
    categoryDialog.setCategories(categories);
}

