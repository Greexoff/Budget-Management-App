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

FinancialAccountController::FinancialAccountController(FinancialAccountRepository& repoRef, QObject* parent)
    : BaseController(parent), financialAccountRepository(repoRef)
{
    fAccountView = new FinancialAccountSelectionView();
    if (fAccountView) {
        setupFinancialAccountWindow();
        refreshTable();
    }
}


void FinancialAccountController::run()
{
    refreshTable();
}

QWidget* FinancialAccountController::getView()
{
    return fAccountView;
}


void FinancialAccountController::setupFinancialAccountWindow()
{
    // 1. Refresh
    connect(fAccountView, &FinancialAccountSelectionView::refreshRequest, this, &FinancialAccountController::refreshTable);

    // 2. ADD ACCOUNT
    connect(fAccountView, &FinancialAccountSelectionView::addAccountRequest, this, &FinancialAccountController::handleFinancialAccountAddRequest);

    // 3. EDIT ACCOUNT 
    connect(fAccountView, &FinancialAccountSelectionView::editAccountRequest, this, &FinancialAccountController::handleFinancialAccountEditRequest);

    // 4. DELETE ACCOUNT
    connect(fAccountView, &FinancialAccountSelectionView::deleteAccountRequest, this, [this]() {
        int id = fAccountView->getSelectedAccountId();
        if (id == -1) return; 

        auto reply = QMessageBox::question(fAccountView, "Confirm", "Delete this account?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) handleFinancialAccountDeleteRequest(id);
        });

    // 5. SEARCH & SORT
    connect(fAccountView, &FinancialAccountSelectionView::searchAccountRequest, this, &FinancialAccountController::handleFinancialAccountFilteringRequest);
    connect(fAccountView, &FinancialAccountSelectionView::columnSortRequest, this, &FinancialAccountController::handleSortingRequest);
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
    fAccountView->setAccountTabHeaders(viewData);
    emit financialAccountDataChanged();
}

// Logika
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

void FinancialAccountController::executeSortingFinancialAccount(QVector<FinancialAccount>& allAccounts) const
{
    std::sort(allAccounts.begin(), allAccounts.end(), [this](const FinancialAccount& a, const FinancialAccount& b) {
        bool asc = (getLastSortingOrder() == Qt::AscendingOrder);
        if (getSelectedColumnId() == 2) {
            return asc ? a.getFinancialAccountBalance() < b.getFinancialAccountBalance() : a.getFinancialAccountBalance() > b.getFinancialAccountBalance();
        }
        return asc ? a.getFinancialAccountName() < b.getFinancialAccountName() : a.getFinancialAccountName() > b.getFinancialAccountName();
        });
}
