/**
 * @file TransactionController.h
 * @brief Header file for the Transaction Controller.
 */
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

 /**
  * @class TransactionController
  * @brief Manages Transaction logic (CRUD, Filtering, Budgeting).
  */
class TransactionController : public BaseController
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the TransactionController with necessary repositories.
     * @param transactionRepositoryRef Reference to transaction repository.
     * @param categoryRepositoryRef Reference to category repository.
     * @param financialAccountRepositoryRef Reference to financial account repository.
     * @param profileRepositoryRef Reference to profile repository.
     * @param parent Parent QObject.
     */
    TransactionController(TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);

    /** @brief Starts the controller. */
    void run() override;

    /** @brief Returns view widget. */
    QWidget* getView();;

public slots:
    /** @brief Opens dialog to add transaction, handles creation logic. */
    void handleAddTransactionRequest();

    /**
     * @brief Filters transactions based on search text.
     * @param searchText The text to search for (name, type, category, etc.).
     */
    void handleFilteringTransactionRequest(QString searchText);

    /** @brief Opens dialog to update monthly budget limit. */
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
    /**
     * @brief Handles column sort request.
     * @param columnId The column index to sort by.
     */
    void handleSortingRequest(int columnId);
    void executeSortingTransaction(QVector<Transaction>& allTransactions);

    void executeSortingTransaction(QVector<Transaction>& allTransactions) const;

    QVector<Transaction> executeFilteringTransaction(const QVector<Transaction> allTransactions);
};