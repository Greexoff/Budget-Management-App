#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/TransactionRepository.h"
#include "Model/Repositories/CategoryRepository.h"
#include "Model/TransactionBuilder.h"

#include "View/TransactionWindowView.h"

class TransactionController : public BaseController
{
    Q_OBJECT
public:
	TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, QObject* parent = nullptr);
signals: 
    void categorySelectionRequest(TransactionBuilder& builder);
    void showCategories(bool selectButtonVisibility);
public slots:
    /**
   * @brief Handles request to add a new transaction
   */
    void startAddingTransactionRequest();
    void finalizeAddingTransaction(TransactionBuilder& builder);
    void initializeMainWindow();
    void handleCategoriesDataChangeRequest();
private:
	TransactionWindow& transactionWindow;
	TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;
    /**
    * @brief Handles request to delete a transaction
    */
    void handleDeleteTransactionRequest();

    void handleShowCategoriesRequestFromView();
    /**
    * @brief Refreshes the transaction display in the main window
    */
    void refreshTransactionsView();

};