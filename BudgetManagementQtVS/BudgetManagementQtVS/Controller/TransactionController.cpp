#include "Controller/TransactionController.h"
TransactionController::TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, QObject* parent)
    : BaseController(parent), transactionWindow(transactionWindowRef), transactionRepository(transactionRepositoryRef), categoryRepository(categoryRepositoryRef), financialAccountRepository(financialAccountRepositoryRef) {}
/**
 * @brief Handles creation of a new transaction
 */
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
    Transaction transaction = builder.build();
    bool success = false;

    if (transaction.getTransactionId() > 0) {
        success = transactionRepository.updateTransaction(transaction);
        if (!success) {
            transactionWindow.showTransactionMessage(tr("Edit transaction"), tr("Failed to update transaction."), "error");
        }
    }
    else {
        success = transactionRepository.addTransaction(transaction);
        if (!success) {
            transactionWindow.showTransactionMessage(tr("New transaction"), tr("Failed to add transaction."), "error");
        }
    }

    if (success) {
        refreshTransactionsView();
    }
}

/**
 * @brief Handles deletion of a transaction
 */
void TransactionController::handleDeleteTransactionRequest()
{
    int transactionId = transactionWindow.getSelectedTransactionId();
    if (transactionId < 0) {
        const QString header = tr("Delete transaction");
        const QString message = tr("No transaction selected.");
        transactionWindow.showTransactionMessage(header, message, "error");
        return;
    }

    if (!transactionRepository.removeTransactionById(transactionId))
    {
        const QString header = tr("Delete transaction");
        const QString message = tr("Failed to delete transaction.");
        transactionWindow.showTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
}


/**
 * @brief Refreshes the transaction display in the main window
 *
 * Retrieves all transactions for the current profile and updates
 * the main window's table view with formatted data including
 * category names resolved from category IDs.
 */
void TransactionController::refreshTransactionsView()
{
    if (getProfileId() < 0) {
        return;
    }

    QVector<Transaction> allTransactions = transactionRepository.getAllProfileTransaction(getProfileId());
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

        rowData << categoryRepository.getCategoryNameById(transaction.getCategoryId());
        rowData << financialAccountRepository.getFinancialAccountNameById(transaction.getFinancialAccountId());
        tableRows.append(rowData);
    }

    transactionWindow.setTransactionRows(tableRows);
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

        setMainWindowInitializedAttribute(true);
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
    // Refresh transaction view to update any financial account name displays
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