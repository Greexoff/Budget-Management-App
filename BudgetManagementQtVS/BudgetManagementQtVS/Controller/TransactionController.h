#pragma once
#include "Controller/BaseController.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/TransactionBuilder.h>
#include <Model/SortStrategy.h>

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
    /**
   * @brief Handles request to add a new transaction
   */
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

    std::unique_ptr<SortStrategy> sortingStrategy;
    SortOrder lastSortingOrder = SortOrder::DESCENDING;
    int lastSelectedColumn = -1;

    /**
    * @brief Handles request to delete a transaction
    */
    void handleDeleteTransactionRequest();

    void handleShowCategoriesRequestFromView();

    void handleShowFinancialAccountsRequestFromView();
    /**
    * @brief Refreshes the transaction display in the main window
    */
    void refreshTransactionsView();

    void handleEditTransactionRequest();

    void handleBackToProfileRequest();

    void handleSortingRequest(int columnId);

    void executeTransactionSorting(QVector<Transaction>& allTransactions);

    QVector<Transaction> executeFilteringTransaction(const QVector<Transaction> allTransactions);
};