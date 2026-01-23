/**
 * @file FinancialAccountSelectionController.cpp
 * @brief Implementation of the Financial Account Controller.
 */
#include <Controller/FinancialAccountSelectionController.h>
#include <QInputDialog>
#include <QMessageBox>
#include <algorithm>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

 /** @brief Constructor. Initializes view and connections. */
FinancialAccountController::FinancialAccountController(FinancialAccountRepository& repoRef, QObject* parent)
    : BaseController(parent), financialAccountRepository(repoRef)
{
    fAccountView = new FinancialAccountSelectionView();
    if (fAccountView) {
        setupFinancialAccountWindow();
        refreshTable();
    }
}

/** @brief Runs the controller. */
void FinancialAccountController::run()
{
    refreshTable();
}
/** @brief Returns view. */
QWidget* FinancialAccountController::getView()
{
    return fAccountView;
}

/** @brief Connects view signals to slots. */
void FinancialAccountController::setupFinancialAccountWindow()
{
    connect(fAccountView, &FinancialAccountSelectionView::refreshRequest, this, &FinancialAccountController::refreshTable);

    connect(fAccountView, &FinancialAccountSelectionView::addAccountRequest, this, &FinancialAccountController::handleFinancialAccountAddRequest);

    connect(fAccountView, &FinancialAccountSelectionView::editAccountRequest, this, &FinancialAccountController::handleFinancialAccountEditRequest);

    connect(fAccountView, &FinancialAccountSelectionView::deleteAccountRequest, this, [this]() {
        int id = fAccountView->getSelectedAccountId();
        if (id == -1) return; 

        auto reply = QMessageBox::question(fAccountView, "Confirm", "Delete this account?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) handleFinancialAccountDeleteRequest(id);
        });

    connect(fAccountView, &FinancialAccountSelectionView::searchAccountRequest, this, &FinancialAccountController::handleFinancialAccountFilteringRequest);
    connect(fAccountView, &FinancialAccountSelectionView::columnSortRequest, this, &FinancialAccountController::handleSortingRequest);
}
/** @brief Refreshes table with data from repository. */
void FinancialAccountController::refreshTable()
{
    QVector<FinancialAccount> accounts = financialAccountRepository.getAllProfileFinancialAccounts(getUserId()); 

    if (!getFilteringText().isEmpty()) accounts = executeFilteringFinancialAccount(accounts);
    executeSortingFinancialAccount(accounts);

    QVector<QStringList> viewData;
    for (const auto& acc : accounts) {
        QStringList row;
        row << QString::number(acc.getFinancialAccountId());
        row << acc.getFinancialAccountName();
        row << acc.getFinancialAccountType();
        row << QString::number(acc.getFinancialAccountBalance(), 'f', 2) + " PLN";
        row << QString::number(acc.getFinancialAccountCurrentBalance(), 'f', 2) + " PLN";
        viewData.append(row);
    }
    fAccountView->setAccountTabHeaders(viewData);
    emit financialAccountDataChanged();
}

// CRUD Logic implementations
void FinancialAccountController::handleFinancialAccountAddRequest(const QString& name, const QString& type, double balance)
{
    if (financialAccountRepository.addFinancialAccount(name, type, balance, getProfileId())) {
        fAccountView->showMessage("Success", "Account added.", "info");
        refreshTable();
    }
    else {
        fAccountView->showMessage("Error", "Failed to add account.", "error");
    }
}

void FinancialAccountController::handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance)
{
    if (financialAccountRepository.updateFinancialAccount(id, name, type, balance)) {
        fAccountView->showMessage("Success", "Updated.", "info");
        refreshTable();
    }
    else {
        fAccountView->showMessage("Error", "Failed to update.", "error");
    }
}

void FinancialAccountController::handleFinancialAccountDeleteRequest(int id)
{
    if (id == selectedFinancialAccountIdForTransaction) {
        fAccountView->showMessage("Error", "Cannot delete default account.", "error");
        return;
    }
    if (financialAccountRepository.removeFinancialAccount(id)) {
        fAccountView->showMessage("Success", "Deleted.", "info");
        refreshTable();
    }
    else {
        fAccountView->showMessage("Error", "Failed to delete.", "error");
    }
}
// Filter & Sort implementations
void FinancialAccountController::handleFinancialAccountFilteringRequest(const QString& text)
{
    setFilteringText(text);
    refreshTable();
}

void FinancialAccountController::handleSortingRequest(int columnId)
{
    setSelectedColumnId(columnId);
    refreshTable();
}

QVector<FinancialAccount> FinancialAccountController::executeFilteringFinancialAccount(const QVector<FinancialAccount> allAccounts)
{
    return executeFiltering(allAccounts, [this](const FinancialAccount& acc) {
        QString filter = getFilteringText();
        return acc.getFinancialAccountName().contains(filter, Qt::CaseInsensitive) ||
            acc.getFinancialAccountType().contains(filter, Qt::CaseInsensitive);
        });
}

void FinancialAccountController::executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts) 
{
    executeSorting(allAccounts, [this](const FinancialAccount& a, const FinancialAccount& b) {
        switch (getSelectedColumnId()) {
        case 1:
            return a.getFinancialAccountName().toLower() < b.getFinancialAccountName().toLower();
        case 2:
            return a.getFinancialAccountType().toLower() < b.getFinancialAccountType().toLower();
        case 3:
            return a.getFinancialAccountBalance() < b.getFinancialAccountBalance();
        case 4:
            return a.getFinancialAccountCurrentBalance() < b.getFinancialAccountCurrentBalance();
        default:
            return a.getFinancialAccountId() < b.getFinancialAccountId();
        }
        });
}
