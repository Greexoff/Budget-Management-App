#include "Controller/TransactionController.h"
#include <QInputDialog> 

TransactionController::TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent)
    : BaseController(parent), transactionWindow(transactionWindowRef), transactionRepository(transactionRepositoryRef), categoryRepository(categoryRepositoryRef), financialAccountRepository(financialAccountRepositoryRef), profileRepository(profileRepositoryRef) {
}

void TransactionController::startAddingTransactionRequest()
{
    if (getProfileId() < 0) {
        transactionWindow.showTransactionMessage(tr("New transaction"), tr("Please select a profile first."), "error");
        return;
    }

    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    QVector<FinancialAccount> accounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

    if (accounts.isEmpty()) {
        transactionWindow.showTransactionMessage(tr("Warning"), tr("You need to add a Financial Account first!"), "error");
        return;
    }

    AddTransactionDialogView dialog(&transactionWindow);
    dialog.setCategories(categories);
    dialog.setFinancialAccounts(accounts);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        if (name.trimmed().isEmpty()) {
            transactionWindow.showTransactionMessage(tr("Error"), tr("Transaction name cannot be empty."), "error");
            return;
        }

        TransactionBuilder builder;
        builder.withProfileId(getProfileId())
            .withName(name)
            .withAmount(dialog.getAmount())
            .withDate(dialog.getDate())
            .withDescription(dialog.getDescription())
            .withCategoryId(dialog.getSelectedCategoryId())
            .withFinancialAccountId(dialog.getSelectedFinancialAccountId());

        Transaction newTransaction = builder.build();

        if (transactionRepository.addTransaction(newTransaction)) {
            refreshTransactionsView();
        }
        else {
            transactionWindow.showTransactionMessage(tr("Error"), tr("Failed to add transaction to database."), "error");
        }
    }
}

void TransactionController::finalizeTransaction(TransactionBuilder& builder)
{
    Transaction newTrans = builder.build();
    bool success = false;

    if (newTrans.getTransactionId() > 0) {
        success = transactionRepository.updateTransaction(newTrans);
        if (!success) {
            transactionWindow.showTransactionMessage(tr("Edit"), tr("Failed to update."), "error");
        }
    }
    else {
        success = transactionRepository.addTransaction(newTrans);
        if (!success) {
            transactionWindow.showTransactionMessage(tr("New"), tr("Failed to add."), "error");
        }
    }

    if (success) {
        refreshTransactionsView();
    }
}

void TransactionController::handleDeleteTransactionRequest()
{
    int transactionId = transactionWindow.getSelectedTransactionId();
    if (transactionId < 0) {
        transactionWindow.showTransactionMessage(tr("Delete"), tr("No transaction selected."), "error");
        return;
    }

    if (!transactionRepository.removeTransactionById(transactionId))
    {
        transactionWindow.showTransactionMessage(tr("Delete"), tr("Failed to delete transaction."), "error");
        return;
    }
    refreshTransactionsView();
}

void TransactionController::refreshTransactionsView()
{
    if (getProfileId() < 0) return;
    QVector<Transaction> allTransactions = transactionRepository.getAllProfileTransaction(getProfileId());
    executeTransactionSorting(allTransactions);
    QVector<QStringList> tableRows;

    for (const auto& transaction : allTransactions) {
        QStringList rowData;
        rowData << QString::number(transaction.getTransactionId())
            << transaction.getTransactionName()
            << transaction.getTransactionDate().toString("yyyy-MM-dd")
            << transaction.getTransactionDescription()
            << QString::number(transaction.getTransactionAmount(), 'f', 2);
        QString typeString = (transaction.getTransactionAmount() > 0.0) ? "Expense" : "Income";
        rowData << typeString;
        rowData << transaction.getTransactionCategoryName();
       // rowData << categoryRepository.getCategoryNameById(transaction.getCategoryId());
        rowData << financialAccountRepository.getFinancialAccountNameById(transaction.getFinancialAccountId());
        tableRows.append(rowData);
    }
    transactionWindow.setTransactionRows(tableRows);

    QDate current = QDate::currentDate();
    double budgetLimit = profileRepository.getBudgetLimit(getProfileId());

    double monthlySpent = transactionRepository.getMonthlyExpenses(getProfileId(), current.month(), current.year());

    transactionWindow.updateBudgetDisplay(budgetLimit, monthlySpent);
}

void TransactionController::initializeMainWindow()
{
    if (!getMainWindowInitializedAttribute()) {
        connect(&transactionWindow, &TransactionWindow::addTransactionRequest,
            this, &TransactionController::startAddingTransactionRequest);
        connect(&transactionWindow, &TransactionWindow::deleteTransactionRequest,
            this, &TransactionController::handleDeleteTransactionRequest);
        connect(&transactionWindow, &TransactionWindow::showCategoriesRequest,
            this, &TransactionController::handleShowCategoriesRequestFromView);
        connect(&transactionWindow, &TransactionWindow::editTransactionRequest,
            this, &TransactionController::handleEditTransactionRequest);
        connect(&transactionWindow, &TransactionWindow::backToProfileRequested,
            this, &TransactionController::handleBackToProfileRequest);
        connect(&transactionWindow, &TransactionWindow::showFinancialAccountsRequest,
            this, &TransactionController::handleShowFinancialAccountsRequestFromView);
        connect(&transactionWindow, &TransactionWindow::editBudgetRequest, this, &TransactionController::handleEditBudgetRequest);

        setMainWindowInitializedAttribute(true);
        connect(&transactionWindow, &TransactionWindow::columnSortRequest,
            this, &TransactionController::handleSortingRequest);
    }

    refreshTransactionsView();
    transactionWindow.show();
}

void TransactionController::handleShowCategoriesRequestFromView()
{
    emit showCategories(false);
}
void TransactionController::handleCategoriesDataChangeRequest()
{
    if (getProfileId() >= 0 && getMainWindowInitializedAttribute()) {
        refreshTransactionsView();
    }
}

void TransactionController::handleShowFinancialAccountsRequestFromView()
{
    emit showFinancialAccounts(false);
}
void TransactionController::handleFinancialAccountsDataChangeRequest()
{
    if (getProfileId() >= 0 && getMainWindowInitializedAttribute()) {
        refreshTransactionsView();
    }
}

void TransactionController::handleEditTransactionRequest()
{
    int transactionId = transactionWindow.getSelectedTransactionId();
    if (transactionId < 0) {
        transactionWindow.showTransactionMessage(tr("Edit transaction"), tr("No transaction selected."), "error");
        return;
    }

    Transaction currentTrans = transactionRepository.getTransactionById(transactionId);
    if (currentTrans.getTransactionId() == -1) return;

    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    QVector<FinancialAccount> accounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

    AddTransactionDialogView dialog(&transactionWindow);
    dialog.setWindowTitle(tr("Edit Transaction"));

    dialog.setCategories(categories);
    dialog.setFinancialAccounts(accounts);

    dialog.setName(currentTrans.getTransactionName());
    dialog.setAmount(currentTrans.getTransactionAmount());
    dialog.setDate(currentTrans.getTransactionDate());
    dialog.setDescription(currentTrans.getTransactionDescription());
    dialog.setSelectedCategoryId(currentTrans.getCategoryId());
    dialog.setSelectedFinancialAccountId(currentTrans.getFinancialAccountId());

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        if (name.trimmed().isEmpty()) {
            transactionWindow.showTransactionMessage(tr("Error"), tr("Transaction name cannot be empty."), "error");
            return;
        }

        TransactionBuilder builder;
        builder.withId(currentTrans.getTransactionId())
            .withProfileId(getProfileId())
            .withName(name)
            .withAmount(dialog.getAmount())
            .withDate(dialog.getDate())
            .withDescription(dialog.getDescription())
            .withCategoryId(dialog.getSelectedCategoryId())
            .withFinancialAccountId(dialog.getSelectedFinancialAccountId());

        Transaction updatedTransaction = builder.build();

        if (transactionRepository.updateTransaction(updatedTransaction)) {
            refreshTransactionsView();
        }
        else {
            transactionWindow.showTransactionMessage(tr("Error"), tr("Failed to update transaction."), "error");
        }
    }
}

void TransactionController::handleBackToProfileRequest()
{
    transactionWindow.hide();
    emit returnToProfileView();
}

void TransactionController::handleEditBudgetRequest()
{
    bool ok;
    double currentLimit = profileRepository.getBudgetLimit(getProfileId());
    double newLimit = QInputDialog::getDouble(&transactionWindow, tr("Budget"),
        tr("Set Monthly Budget Limit (PLN):"),
        currentLimit, 0, 1000000, 2, &ok);
    if (ok) {
        profileRepository.setBudgetLimit(getProfileId(), newLimit);
        refreshTransactionsView();
    }
}
void TransactionController::handleSortingRequest(int columnId)
{
    int currentColumnId = columnId;
    SortOrder newOrder;
    
    if (currentColumnId == lastSelectedColumn)
    {
        if (lastSortingOrder == SortOrder::ASCENDING)
        {
            newOrder = SortOrder::DESCENDING;
        }
        else
        {
            newOrder = SortOrder::ASCENDING;
        }
    }
    else
    {
        newOrder = SortOrder::ASCENDING;
    }

    lastSelectedColumn = currentColumnId;
    lastSortingOrder = newOrder;

    switch (columnId) {
    case 1://Name
        sortingStrategy = std::make_unique<SortByName>(lastSortingOrder);
        break;
    case 2://Date
        sortingStrategy = std::make_unique<SortByDate>(lastSortingOrder);
        break;
    case 3://Description
        sortingStrategy = std::make_unique<SortByDescription>(lastSortingOrder);
        break;
    case 4://Amount
        sortingStrategy = std::make_unique<SortByAmount>(lastSortingOrder);
        break;
    case 5://Type
        sortingStrategy = std::make_unique<SortByType>(lastSortingOrder);
        break;
    case 6://Category
        sortingStrategy = std::make_unique<SortByCategory>(lastSortingOrder);
        break;
    case 7://Financial Account
        sortingStrategy = std::make_unique<SortByFinancialAccount>(lastSortingOrder);
        break;
    default:
        qDebug() << "Unknown ID of column";
        return;
        break;
    }
    refreshTransactionsView();
}
void TransactionController::executeTransactionSorting(QVector<Transaction>& allTransactions)
{
    if (sortingStrategy != nullptr)
    {
        SortContext sortingContext;
        sortingContext.setStrategy(std::move(sortingStrategy));
        sortingContext.executeStrategy(allTransactions);
    }
}