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

FinancialAccountController::FinancialAccountController(FinancialAccountSelectionView& viewRef, FinancialAccountRepository& repoRef, QObject* parent)
    : BaseController(parent), view(viewRef), financialAccountRepository(repoRef)
{
    setupFinancialAccountWindow();
    refreshTable();
}

void FinancialAccountController::setupFinancialAccountWindow()
{
    // 1. Refresh
    connect(&view, &FinancialAccountSelectionView::refreshRequest, this, &FinancialAccountController::refreshTable);

    // 2. ADD ACCOUNT
    connect(&view, &FinancialAccountSelectionView::addAccountRequest, this, &FinancialAccountController::handleFinancialAccountAddRequest);

    // 3. EDIT ACCOUNT 
    connect(&view, &FinancialAccountSelectionView::editAccountRequest, this, &FinancialAccountController::handleFinancialAccountEditRequest);

    // 4. DELETE ACCOUNT
    connect(&view, &FinancialAccountSelectionView::deleteAccountRequest, this, [this]() {
        int id = view.getSelectedAccountId();
        if (id == -1) return; 

        auto reply = QMessageBox::question(&view, "Confirm", "Delete this account?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) handleFinancialAccountDeleteRequest(id);
        });

    // 5. SEARCH & SORT
    connect(&view, &FinancialAccountSelectionView::searchAccountRequest, this, &FinancialAccountController::handleFinancialAccountFilteringRequest);
    connect(&view, &FinancialAccountSelectionView::columnSortRequest, this, &FinancialAccountController::handleSortingRequest);
}

void FinancialAccountController::showAccounts() { refreshTable(); }

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
        row << QString::number(acc.getFinancialAccountBalance(), 'f', 2) + " PLN";
        viewData.append(row);
    }
    view.setAccountTabHeaders(viewData);
    emit financialAccountDataChanged();
}

// Logika
void FinancialAccountController::handleFinancialAccountAddRequest(const QString& name, const QString& type, double balance)
{
    if (financialAccountRepository.addFinancialAccount(name, type, balance, getProfileId())) {
        view.showMessage("Success", "Account added.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to add account.", "error");
    }
}

void FinancialAccountController::handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance)
{
    if (financialAccountRepository.updateFinancialAccount(id, name, type, balance)) {
        view.showMessage("Success", "Updated.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to update.", "error");
    }
}

void FinancialAccountController::handleFinancialAccountDeleteRequest(int id)
{
    if (id == selectedFinancialAccountIdForTransaction) {
        view.showMessage("Error", "Cannot delete default account.", "error");
        return;
    }
    if (financialAccountRepository.removeFinancialAccount(id)) {
        view.showMessage("Success", "Deleted.", "info");
        refreshTable();
    }
    else {
        view.showMessage("Error", "Failed to delete.", "error");
    }
}

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
    QVector<FinancialAccount> filtered;
    for (const auto& acc : allAccounts) {
        if (acc.getFinancialAccountName().contains(getFilteringText(), Qt::CaseInsensitive))
            filtered.append(acc);
    }
    return filtered;
}

void FinancialAccountController::executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts)
{
    std::sort(allAccounts.begin(), allAccounts.end(), [this](const FinancialAccount& a, const FinancialAccount& b) {
        bool asc = (getLastSortingOrder() == Qt::AscendingOrder);
        if (getSelectedColumnId() == 2) {
            return asc ? a.getFinancialAccountBalance() < b.getFinancialAccountBalance() : a.getFinancialAccountBalance() > b.getFinancialAccountBalance();
        }
        return asc ? a.getFinancialAccountName() < b.getFinancialAccountName() : a.getFinancialAccountName() > b.getFinancialAccountName();
        });
}