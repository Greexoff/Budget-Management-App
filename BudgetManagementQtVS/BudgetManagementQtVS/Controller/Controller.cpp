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
   
}

void TransactionController::run()
{
    loginDialog.show();
}

void TransactionController::handleLoginRequested(const QString& username, const QString& password)
{
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

        QString typeStr = (t.getTransactionAmount() >= 0.0) ? "Przychód" : "Wydatek";
        row << typeStr;

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

    bool ok = false;

    QString name = QInputDialog::getText(
        &mainWindowView,
        tr("Nowa transakcja"),
        tr("Nazwa:"),
        QLineEdit::Normal,
        "",
        &ok
    );
    if (!ok || name.trimmed().isEmpty())
        return;

    double amount = QInputDialog::getDouble(
        &mainWindowView,
        tr("Nowa transakcja"),
        tr("Kwota:"),
        0.0,
        -1e9,
        1e9,
        2,
        &ok
    );
    if (!ok)
        return;

    QString description = QInputDialog::getText(
        &mainWindowView,
        tr("Nowa transakcja"),
        tr("Opis:"),
        QLineEdit::Normal,
        "",
        &ok
    );
    if (!ok)
        return;

    TransactionType type = (amount >= 0.0) ? INCOME : EXPENSE;
    int categoryId = 0;   // TODO: jak będzie logika kategorii

    Transaction t(
        0,                      // id (baza nada AUTOINCREMENT)
        name,
        QDate::currentDate(),
        description,
        amount,
        type,
        categoryId,
        currentProfileId        // powiązanie z profilem
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
