#pragma once

#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/FinancialAccount.h>
#include <Model/TransactionBuilder.h>

#include <Controller/BaseController.h>

#include <View/FinancialAccountSelectionView.h>

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>

class FinancialAccountController : public BaseController {
	Q_OBJECT

public:
	FinancialAccountController(FinancialAccountSelectionView& financialAccountSelectionViewRef, FinancialAccountRepository& financialAccountRepositoryRef, QObject* parent = nullptr);

signals:
	void financialAccountDataChanged();
	void financialAccountSelected(TransactionBuilder& builder);

public slots:
	void handleFinancialAccountSelectionWhileAddingTransactionRequest(TransactionBuilder& builder);
	void handleFinancialAccountSelection(int financialAccountId);
	void handleFinancialAccountSelectionFromTransactionWindow(bool selectButtonVisibility);
	void handleFinancialAccountAddRequest(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance);
	void handleFinancialAccountDeleteRequest(int financialAccountId);
	void handleFinancialAccountEditRequest(int id, const QString& name, const QString& type, double balance);
	void handleFinancialAccountFilteringRequest(const QString& searchText);

private:
	FinancialAccountSelectionView& financialAccountDialog;
	FinancialAccountRepository& financialAccountRepository;

	int selectedFinancialAccountIdForTransaction = 1;

	void refreshFinancialAccountDialogList();
	void showFinancialAccountDialog(bool withSelectButton);
	QVector<FinancialAccount> executeFilteringFinancialAccount(const QVector<FinancialAccount> allFinancialAccounts);

};