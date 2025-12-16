#pragma once
#include "Controller/BaseController.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/TransactionBuilder.h>

#include "View/TransactionWindowView.h"
#include "View/AddTransactionDialogView.h"

#include <memory>
class TransactionController : public BaseController
{
    Q_OBJECT
public:
	TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);
signals: 
    void categorySelectionRequest(TransactionBuilder& builder);
    void showCategories(bool selectButtonVisibility);
    void financialAccountSelectionRequest(TransactionBuilder& builder);
    void showFinancialAccounts(bool selectButtonVisibility);
    void returnToProfileView();
public slots:

    void startAddingTransactionRequest();
    void initializeMainWindow();
    void handleCategoriesDataChangeRequest();
    void handleFinancialAccountsDataChangeRequest();
    void finalizeTransaction(TransactionBuilder& builder);
    void handleFilteringTransactionRequest(QString searchText);
    void handleEditBudgetRequest();
private:
	TransactionWindow& transactionWindow;
	TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;
    FinancialAccountRepository& financialAccountRepository;
    ProfilesRepository& profileRepository;

    void handleDeleteTransactionRequest();

    void handleShowCategoriesRequestFromView();

    void handleShowFinancialAccountsRequestFromView();

    void refreshTransactionsView();

    void handleEditTransactionRequest();

    void handleBackToProfileRequest();

    void handleSortingRequest(int columnId);

    void executeTransactionSorting(QVector<Transaction>& allTransactions);

    QVector<Transaction> executeFilteringTransaction(const QVector<Transaction> allTransactions);
};