#include <Controller/FinancialAccountSelectionController.h>

FinancialAccountController::FinancialAccountController(FinancialAccountSelectionView& financialAccountSelectionViewRef,
	FinancialAccountRepository& financialAccountRepositoryRef,
	QObject* parent) : BaseController(parent),
	financialAccountDialog(financialAccountSelectionViewRef),
	financialAccountRepository(financialAccountRepositoryRef) {

	connect(&financialAccountDialog, &FinancialAccountSelectionView::selectRequestedFinancialAccount, this, &FinancialAccountController::handleFinancialAccountSelection);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::addRequestedFinancialAccount, this, &FinancialAccountController::handleFinancialAccountAddRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::deleteRequestedFinancialAccount, this, &FinancialAccountController::handleFinancialAccountDeleteRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::editRequestedFinancialAccount,this, &FinancialAccountController::handleFinancialAccountEditRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::searchTextRequest, this, &FinancialAccountController::handleFinancialAccountFilteringRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::columnSortRequest, this, &FinancialAccountController::handleSortingRequest);

}

void FinancialAccountController::showFinancialAccountDialog(bool withSelectButton) {

	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
	financialAccountDialog.setFinancialAccounts(financialAccounts);
	financialAccountDialog.setSelectFinancialAccountButtonVisible(withSelectButton);
	setFilteringText("");
	financialAccountDialog.clearSearchLineEdit();
	financialAccountDialog.exec();
}

void FinancialAccountController::handleFinancialAccountSelection(int financialAccountId) {
	selectedFinancialAccountIdForTransaction = financialAccountId;
	financialAccountDialog.accept();
}

void FinancialAccountController::handleFinancialAccountAddRequest(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance) {
	if (!financialAccountRepository.addFinancialAccount(financialAccountName, financialAccountType, financialAccountBalance, getProfileId())) {
		const QString header = tr("New financial account");
		const QString message = tr("Failed to add a financial account.");
		financialAccountDialog.showFinancialAccountMessage(header, message, "error");
	}

	refreshFinancialAccountDialogList();
}

void FinancialAccountController::handleFinancialAccountDeleteRequest(int financialAccountId) {
	if (!financialAccountRepository.removeFinancialAccount(financialAccountId)) {
		const QString header = tr("Delete financial account");
		const QString message = tr("Failed to delete a financial account.");
		financialAccountDialog.showFinancialAccountMessage(header, message, "error");
	}

	refreshFinancialAccountDialogList();

	emit financialAccountDataChanged();
}

void FinancialAccountController::refreshFinancialAccountDialogList() {
	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
	financialAccounts = executeFilteringFinancialAccount(financialAccounts);
	executeSortingFinancialAccount(financialAccounts);
	financialAccountDialog.setFinancialAccounts(financialAccounts);
}

void FinancialAccountController::handleFinancialAccountSelectionWhileAddingTransactionRequest(TransactionBuilder& builder) {
	showFinancialAccountDialog(true);
	builder.withFinancialAccountId(selectedFinancialAccountIdForTransaction);
	emit financialAccountSelected(builder);
}

void FinancialAccountController::handleFinancialAccountSelectionFromTransactionWindow(bool selectButtonVisibility) {
	showFinancialAccountDialog(selectButtonVisibility);
}

void FinancialAccountController::handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance)
{
	if (financialAccountRepository.updateFinancialAccount(id, name, type, balance)) {
		refreshFinancialAccountDialogList();

		emit financialAccountDataChanged();
	}
	else {
		financialAccountDialog.showFinancialAccountMessage(tr("Edit Account"), tr("Failed to update account."), "error");
	}
}
void FinancialAccountController::handleFinancialAccountFilteringRequest(const QString& searchText)
{
	setFilteringText(searchText);
	refreshFinancialAccountDialogList();
}

QVector<FinancialAccount> FinancialAccountController::executeFilteringFinancialAccount(const QVector<FinancialAccount> allFinancialAccounts)
{
	auto matchFound = [&](const FinancialAccount& financialAccount) -> bool
		{
			bool finAccountNameMatches = financialAccount.getFinancialAccountName().contains(getFilteringText(), Qt::CaseInsensitive);
			bool finAccountTypeMatches = financialAccount.getFinancialAccountType().contains(getFilteringText(), Qt::CaseInsensitive);
			return finAccountNameMatches || finAccountTypeMatches;
		};
	return executeFiltering(allFinancialAccounts, matchFound);
}
void FinancialAccountController::handleSortingRequest(int columnId)
{
	setSelectedColumnId(columnId);
	refreshFinancialAccountDialogList();
}
void FinancialAccountController::executeSortingFinancialAccount(QVector<FinancialAccount>& allFinancialAccounts)
{
	executeSorting(allFinancialAccounts, getSelectedColumnId(), getLastSortingOrder());
}
