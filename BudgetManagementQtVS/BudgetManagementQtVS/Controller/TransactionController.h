#pragma once
#include "Controller/BaseController.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/TransactionBuilder.h>

#include "View/TransactionWindowView.h"
#include "View/TransactionEditorDialogView.h"
#include <QPointer>
#include <memory>
class TransactionController : public BaseController
{
    Q_OBJECT
public:
	TransactionController(TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);
    void run() override;
    QWidget* getView();

public slots:
    void handleAddTransactionRequest();
    void handleFilteringTransactionRequest(QString searchText);
    void handleEditBudgetRequest();

private:
	TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;
    FinancialAccountRepository& financialAccountRepository;
    ProfilesRepository& profileRepository;

    QPointer<TransactionWindow> transactionView;

    void handleDeleteTransactionRequest();

    void refreshTransactionsView();

    void handleEditTransactionRequest();

    void handleSortingRequest(int columnId);

    void executeSortingTransaction(QVector<Transaction>& allTransactions) const;

    QVector<Transaction> executeFilteringTransaction(const QVector<Transaction> allTransactions);
};