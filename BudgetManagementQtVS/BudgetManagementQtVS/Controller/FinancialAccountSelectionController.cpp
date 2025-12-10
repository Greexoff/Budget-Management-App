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

}

void FinancialAccountController::showFinancialAccountDialog(bool withSelectButton) {

	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
	financialAccountDialog.setFinancialAccounts(financialAccounts);
	financialAccountDialog.setSelectFinancialAccountButtonVisible(withSelectButton);
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

	refresFinancialAccountDialogList();
}

void FinancialAccountController::handleFinancialAccountDeleteRequest(int financialAccountId) {
	if (!financialAccountRepository.removeFinancialAccount(financialAccountId)) {
		const QString header = tr("Delete financial account");
		const QString message = tr("Failed to delete a financial account.");
		financialAccountDialog.showFinancialAccountMessage(header, message, "error");
	}

	refresFinancialAccountDialogList();

	emit financialAccountDataChanged();
}

void FinancialAccountController::refresFinancialAccountDialogList() {
	QVector<FinancialAccount> financialAccounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());
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
		refresFinancialAccountDialogList();

		emit financialAccountDataChanged();
	}
	else {
		financialAccountDialog.showFinancialAccountMessage(tr("Edit Account"), tr("Failed to update account."), "error");
	}
}