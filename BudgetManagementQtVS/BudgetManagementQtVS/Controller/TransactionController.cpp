#include "Controller/TransactionController.h"
TransactionController::TransactionController(TransactionWindow& transactionWindowRef, TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, QObject* parent) : BaseController(parent), transactionWindow(transactionWindowRef), transactionRepository(transactionRepositoryRef), categoryRepository(categoryRepositoryRef) {}
/**
 * @brief Handles creation of a new transaction
 */
void TransactionController::startAddingTransactionRequest()
{
    if (getProfileId() < 0) {
        const QString header = tr("New transaction");
        const QString message = tr("Please select a profile first.");
        transactionWindow.showTransactionMessage(header, message, "error");
        return;
    }

    bool correctData = false;

    TransactionBuilder builder;
    builder.withProfileId(getProfileId());

    QString name = transactionWindow.getTransactionNameFromInput(correctData);
    if (!correctData || name.trimmed().isEmpty()) { return; }
    builder.withName(name);

    double amount = transactionWindow.getTransactionAmountFromInput(correctData);
    builder.withAmount(amount);

    QString description = transactionWindow.getTransactionDescriptionFromInput(correctData);
    builder.withDescription(description);
    emit categorySelectionRequest(builder);
}
void TransactionController::finalizeAddingTransaction(TransactionBuilder& builder)
{
    bool correctData = false;
    Transaction newTransaction = builder.build();
    if (!transactionRepository.addTransaction(newTransaction))
    {
        const QString header = tr("New transaction");
        const QString message = tr("Failed to add transaction.");
        transactionWindow.showTransactionMessage(header, message, "error");
        return;
    }
    refreshTransactionsView();
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

        // Determine transaction type based on amount
        QString typeString = (transaction.getTransactionAmount() > 0.0) ? "Expense" : "Income";
        rowData << typeString;

        // Resolve category name from ID
        rowData << categoryRepository.getCategoryNameById(transaction.getCategoryId());
        tableRows.append(rowData);
    }

    transactionWindow.setTransactionRows(tableRows);
}

void TransactionController::initializeMainWindow()
{
    connect(&transactionWindow, &TransactionWindow::addTransactionRequest,
        this, &TransactionController::startAddingTransactionRequest);
    connect(&transactionWindow, &TransactionWindow::deleteTransactionRequest,
        this, &TransactionController::handleDeleteTransactionRequest);
    connect(&transactionWindow, &TransactionWindow::showCategoriesRequest,
        this, &TransactionController::handleShowCategoriesRequestFromView);

    setMainWindowInitializedAttribute(true);

     refreshTransactionsView();
     transactionWindow.show();
}
void TransactionController::handleShowCategoriesRequestFromView()
{
    emit showCategories(false);
}
void TransactionController::handleCategoriesDataChangeRequest()
{
    // Refresh transaction view to update any category name displays
    if (getProfileId() >= 0 && getMainWindowInitializedAttribute()) {
        refreshTransactionsView();
    }
}