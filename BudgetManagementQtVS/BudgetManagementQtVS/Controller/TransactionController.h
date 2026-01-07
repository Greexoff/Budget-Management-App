#pragma once
#include "Controller/BaseController.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>
#include <Model/Repositories/ProfileRepository.h>
#include <Model/TransactionBuilder.h>

#include "View/TransactionWindowView.h"
#include "View/TransactionEditorDialogView.h"

#include <memory>
class TransactionController : public BaseController
{
    Q_OBJECT
public:
	TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);
signals: 
    void categorySelectionRequest(TransactionBuilder& builder);
    void showCategories();
	void showCharts(); //CHARTS!!
    void financialAccountSelectionRequest(TransactionBuilder& builder);
    void showFinancialAccounts();
    void returnToProfileView();

public slots:

    void handleAddTransactionRequest();
    void setupTransactionWindow();
    void handleDataChangeRequest();
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

    void handleShowChartsRequestFromView(); //CHARTS!!

    void refreshTransactionsView();

    void handleEditTransactionRequest();

    void handleBackToProfileRequest();

    void handleSortingRequest(int columnId);

    void executeSortingTransaction(QVector<Transaction>& allTransactions);

    QVector<Transaction> executeFilteringTransaction(const QVector<Transaction> allTransactions);
};