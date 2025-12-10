#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/TransactionRepository.h"
#include "Model/Repositories/CategoryRepository.h"
#include <Model/Repositories/FinancialAccountRepository.h>
#include "Model/TransactionBuilder.h"

#include "View/TransactionWindowView.h"
#include "View/AddTransactionDialogView.h"

class TransactionController : public BaseController
{
    Q_OBJECT
public:
	TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, QObject* parent = nullptr);
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
private:
	TransactionWindow& transactionWindow;
	TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;
    FinancialAccountRepository& financialAccountRepository;
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
};