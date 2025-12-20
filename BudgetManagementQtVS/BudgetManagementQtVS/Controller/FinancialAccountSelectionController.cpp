#include <Controller/FinancialAccountSelectionController.h>

FinancialAccountController::FinancialAccountController(FinancialAccountSelectionView& financialAccountSelectionViewRef,
	FinancialAccountRepository& financialAccountRepositoryRef,
	QObject* parent) : BaseController(parent),
	financialAccountDialog(financialAccountSelectionViewRef),
	financialAccountRepository(financialAccountRepositoryRef) {

	connect(&financialAccountDialog, &FinancialAccountSelectionView::addRequestedFinancialAccount, this, &FinancialAccountController::handleFinancialAccountAddRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::deleteRequestedFinancialAccount, this, &FinancialAccountController::handleFinancialAccountDeleteRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::editRequestedFinancialAccount,this, &FinancialAccountController::handleFinancialAccountEditRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::searchTextRequest, this, &FinancialAccountController::handleFinancialAccountFilteringRequest);
	connect(&financialAccountDialog, &FinancialAccountSelectionView::columnSortRequest, this, &FinancialAccountController::handleSortingRequest);

}


//--------------------------Setting up view---------------------------------------------


void FinancialAccountController::setupFinancialAccountWindow() {

	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
	financialAccountDialog.setFinancialAccounts(financialAccounts);
	setFilteringText("");
	financialAccountDialog.clearSearchLineEdit();
	financialAccountDialog.exec();
}

void FinancialAccountController::refreshFinancialAccountDialogList() {
	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
	financialAccounts = executeFilteringFinancialAccount(financialAccounts);
	executeSortingFinancialAccount(financialAccounts);
	financialAccountDialog.setFinancialAccounts(financialAccounts);
}


//----------------Handling actions performed on financial accounts----------------------

//Method responsible for adding financial account based on entered data
void FinancialAccountController::handleFinancialAccountAddRequest(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance) {
	if (!financialAccountRepository.addFinancialAccount(financialAccountName, financialAccountType, financialAccountBalance, getProfileId())) {
		const QString header = tr("New financial account");
		const QString message = tr("Failed to add a financial account.");
		financialAccountDialog.showFinancialAccountMessage(header, message, "error");
	}

	refreshFinancialAccountDialogList();
}

//Method responsible for handling editing of financial account data
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

//Method responsible for handling deletion of financial account
void FinancialAccountController::handleFinancialAccountDeleteRequest(int financialAccountId) {
	if (!financialAccountRepository.removeFinancialAccount(financialAccountId)) {
		const QString header = tr("Delete financial account");
		const QString message = tr("Failed to delete a financial account.");
		financialAccountDialog.showFinancialAccountMessage(header, message, "error");
	}

	refreshFinancialAccountDialogList();

	emit financialAccountDataChanged();
}

//Method that sets up filtering text and calls refresh view method where filtering occurs
void FinancialAccountController::handleFinancialAccountFilteringRequest(const QString& searchText)
{
	setFilteringText(searchText);
	refreshFinancialAccountDialogList();
}

//An actual method for handling filtering specific financial accounts
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

//Method that sets up selected column id on which sorting will occur and calls refresh view method where an actual sorting method is called
void FinancialAccountController::handleSortingRequest(int columnId)
{
	setSelectedColumnId(columnId);
	refreshFinancialAccountDialogList();
}

//An actual method for handling sorting financial account
void FinancialAccountController::executeSortingFinancialAccount(QVector<FinancialAccount>& allFinancialAccounts)
{
	executeSorting(allFinancialAccounts, getSelectedColumnId(), getLastSortingOrder());
}
