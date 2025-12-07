#include "Controller.h"
#include <QInputDialog>
#include <QMessageBox>

TransactionController::TransactionController(QObject* parent): QObject(parent)
{

    connect(&loginDialog, &LoginDialog::loginRequested,this, &TransactionController::handleLoginRequested);

    connect(&loginDialog, &LoginDialog::registerRequested,this, &TransactionController::handleRegisterRequested);

    connect(&profileDialog, &ProfileDialog::profileSelected,this, &TransactionController::handleProfileSelected);

    connect(&profileDialog, &ProfileDialog::addProfileRequested,this, &TransactionController::handleAddProfileRequested);

    connect(&profileDialog, &ProfileDialog::removeProfileRequested,this, &TransactionController::handleRemoveProfileRequested);
    
    connect(&categoryDialog, &CategorySelectionView::selectRequestedCategory, this, &TransactionController::handleCategorySelected);
    connect(&categoryDialog, &CategorySelectionView::addRequestedCategory, this, &TransactionController::handleAddCategoryRequested);
    connect(&categoryDialog, &CategorySelectionView::deleteRequestedCategory, this, &TransactionController::handleDeleteCategoryRequested);   
}

void TransactionController::run()
{
    loginDialog.show();
}

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

void TransactionController::handleRegisterRequested(const QString& username, const QString& password)
{
    if (!userRepository.addUser(username, password)) {
        QMessageBox::warning(&loginDialog, tr("Rejestracja"), tr("Nie udało się utworzyć użytkownika"));
        return;
    }

    QMessageBox::information(&loginDialog, tr("Rejestracja"),
        tr("Użytkownik utworzony. Możesz się zalogować."));
}

void TransactionController::showProfilesForCurrentUser()
{
    QVector<Profile> profiles = profilesRepository.getProfilesByUserId(currentUserId);
    profileDialog.setProfiles(profiles);
    profileDialog.exec();
}

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

void TransactionController::handleAddProfileRequested(const QString& name)
{
    if (!profilesRepository.addProfile(currentUserId, name)) {
        QMessageBox::warning(&profileDialog, tr("Profil"),
            tr("Nie udało się dodać profilu"));
        return;
    }

    showProfilesForCurrentUser();
}

void TransactionController::handleRemoveProfileRequested(int profileId)
{
    if (!profilesRepository.removeProfileById(profileId)) {
        QMessageBox::warning(&profileDialog, tr("Profil"),
            tr("Nie udało się usunąć profilu"));
        return;
    }

    showProfilesForCurrentUser();
}

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



void TransactionController::handleAddTransactionRequested()
{
    if (currentProfileId < 0) {
        QMessageBox::warning(&mainWindowView, tr("Błąd"),
            tr("Najpierw wybierz profil."));
        return;
    }

    bool correctData = false;

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

    TransactionType type = (amount > 0.0) ? EXPENSE : INCOME;
    
    int categoryId = askUserForCategoryId();
    if (categoryId < 0) {
        categoryId = 1; // default category
    }

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

void TransactionController::showCategorySelectionDialog()
{

    openCategoryDialog(false);
    /*QVector<Category> categories = categoryRepository.getAllCategories();
    categoryDialog.setCategories(categories);
    categoryDialog.exec();*/
}
void TransactionController::openCategoryDialog(bool withSelectButton)
{
    QVector<Category> categories = categoryRepository.getAllCategories(currentProfileId);
    categoryDialog.setCategories(categories);
    categoryDialog.setSelectCategoryButtonVisible(withSelectButton);
    categoryDialog.exec();
}
void TransactionController::handleCategorySelected(int categoryId)
{
    selectedCategoryIdForTransaction = categoryId;
    categoryDialog.accept();
}
int TransactionController::askUserForCategoryId()
{
    selectedCategoryIdForTransaction = -1;
    openCategoryDialog(true);   

    return selectedCategoryIdForTransaction;   
}
void TransactionController::handleAddCategoryRequested(const QString& categoryName)
{
    if (!categoryRepository.addCategory(categoryName, currentProfileId))
    {
       QMessageBox::warning(&categoryDialog, tr("Error"),
       tr("Couldn't add category"));
    }
    
    refreshCategoryDialogList();
}
void TransactionController::handleDeleteCategoryRequested(int categoryId)
{
    if (categoryId==1 || !categoryRepository.removeCategoryUsingId(categoryId)) {
        QMessageBox::warning(&categoryDialog, tr("Error"),
            tr("Couldn't delete category"));
    }
    
    refreshCategoryDialogList();

    if (currentProfileId >= 0 && mainWindowInitialized) {
        refreshTransactionsView();
    }
}

void TransactionController::refreshCategoryDialogList()
{
    QVector<Category> categories = categoryRepository.getAllCategories(currentProfileId);
    categoryDialog.setCategories(categories);
}

