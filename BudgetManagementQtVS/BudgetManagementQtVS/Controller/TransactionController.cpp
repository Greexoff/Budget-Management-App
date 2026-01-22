#include "Controller/TransactionController.h"
#include <QInputDialog> 

TransactionController::TransactionController(TransactionRepository& transactionRepositoryRef, CategoryRepository& categoryRepositoryRef, FinancialAccountRepository& financialAccountRepositoryRef, ProfilesRepository& profileRepositoryRef, QObject* parent)
	: BaseController(parent), transactionRepository(transactionRepositoryRef),
	  categoryRepository(categoryRepositoryRef), financialAccountRepository(financialAccountRepositoryRef),
	  profileRepository(profileRepositoryRef)
{
	transactionView = new TransactionWindow();
	
	    if (transactionView) {
	        connect(transactionView, &TransactionWindow::addTransactionRequest,
	            this, &TransactionController::handleAddTransactionRequest);
	        connect(transactionView, &TransactionWindow::deleteTransactionRequest,
	            this, &TransactionController::handleDeleteTransactionRequest);
	        connect(transactionView, &TransactionWindow::editTransactionRequest,
	            this, &TransactionController::handleEditTransactionRequest);
	        connect(transactionView, &TransactionWindow::editBudgetRequest,
	            this, &TransactionController::handleEditBudgetRequest);
	        connect(transactionView, &TransactionWindow::searchTextRequest,
	            this, &TransactionController::handleFilteringTransactionRequest);
	        connect(transactionView, &TransactionWindow::columnSortRequest,
	            this, &TransactionController::handleSortingRequest);
	    }
}

void TransactionController::run()
{
    refreshTransactionsView();
}

QWidget* TransactionController::getView()
{
    return transactionView;
}

void TransactionController::refreshTransactionsView()
{
    if (!transactionView || getProfileId() < 0) return;
    QVector<Transaction> allTransactions = transactionRepository.getAllProfileTransaction(getProfileId());

    allTransactions = executeFilteringTransaction(allTransactions);
    executeSortingTransaction(allTransactions);

    QVector<QStringList> tableRows;
    for (const auto& transaction : allTransactions) {
        QStringList rowData;
        rowData << QString::number(transaction.getTransactionId())
            << transaction.getTransactionName()
            << transaction.getTransactionDate().toString("yyyy-MM-dd")
            << transaction.getTransactionDescription()
            << QString::number(transaction.getTransactionAmount(), 'f', 2)
            << transaction.getTransactionType()
            << categoryRepository.getCategoryNameById(transaction.getCategoryId())
            << financialAccountRepository.getFinancialAccountNameById(transaction.getFinancialAccountId());
        tableRows.append(rowData);
    }
    transactionView->setTransactionTabHeaders(tableRows);

    QDate current = QDate::currentDate();
    double budgetLimit = profileRepository.getBudgetLimit(getProfileId());
    double monthlySpent = transactionRepository.getMonthlyExpenses(getProfileId(), current.month(), current.year());

    transactionView->updateBudgetDisplay(budgetLimit, monthlySpent);
}


void TransactionController::handleAddTransactionRequest()
{
    if (getProfileId() < 0) {return;}

    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    QVector<FinancialAccount> accounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

    if (accounts.isEmpty()) {
        transactionView->showTransactionMessage(tr("Warning"), tr("You need to add a Financial Account first!"), "error");
        return;
    }

    TransactionEditorDialogView dialog(transactionView);
    dialog.setCategories(categories);
    dialog.setFinancialAccounts(accounts);
    dialog.setTransactionTypes();

    connect(&dialog, &TransactionEditorDialogView::addCategoryRequested, this,
        [&](const QString& name) {
            if (categoryRepository.addCategory(name, getProfileId())) {

                QVector<Category> newCats = categoryRepository.getAllProfileCategories(getProfileId());

                int newId = -1;
                for (const auto& c : newCats) if (c.getCategoryName() == name) newId = c.getCategoryId();

                dialog.refreshCategories(newCats, newId);
            }
            else {
                transactionView->showTransactionMessage("Error", "Failed to add category", "error");
            }
        });

    connect(&dialog, &TransactionEditorDialogView::addFinancialAccountRequested, this,
        [&](const QString& name, const QString& type, double balance) {
            if (financialAccountRepository.addFinancialAccount(name, type, balance, getProfileId())) {
                QVector<FinancialAccount> newAccs = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

   
                int newId = -1;
                for (const auto& a : newAccs) if (a.getFinancialAccountName() == name) newId = a.getFinancialAccountId();

                dialog.refreshFinancialAccounts(newAccs, newId);
            }
            else {
                transactionView->showTransactionMessage("Error", "Failed to add account", "error");
            }
        });

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        if (name.trimmed().isEmpty()) {
            transactionView->showTransactionMessage(tr("Error"), tr("Transaction name cannot be empty."), "error");
            return;
        }

        TransactionBuilder builder;
        builder.withProfileId(getProfileId())
            .withName(name)
            .withAmount(dialog.getAmount())
            .withDate(dialog.getDate())
            .withDescription(dialog.getDescription())
            .withCategoryId(dialog.getSelectedCategoryId())
            .withType(dialog.getType())
            .withFinancialAccountId(dialog.getSelectedFinancialAccountId());

        Transaction newTransaction = builder.build();

        if (transactionRepository.addTransaction(newTransaction)) {
            refreshTransactionsView();
        }
        else {
            transactionView->showTransactionMessage(tr("Error"), tr("Failed to add transaction to database."), "error");
        }
    }
}

//Method responsible for handling transaction edition
void TransactionController::handleEditTransactionRequest()
{
    int transactionId = transactionView->getSelectedTransactionId();
    if (transactionId < 0) {
        transactionView->showTransactionMessage(tr("Edit transaction"), tr("No transaction selected."), "error");
        return;
    }

    Transaction currentTrans = transactionRepository.getTransactionById(transactionId);
    if (currentTrans.getTransactionId() == -1) return;

    QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
    QVector<FinancialAccount> accounts = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

    TransactionEditorDialogView dialog(transactionView);
    dialog.setWindowTitle(tr("Edit Transaction"));

    dialog.setCategories(categories);
    dialog.setFinancialAccounts(accounts);
    dialog.setTransactionTypes();

    dialog.setName(currentTrans.getTransactionName());
    dialog.setAmount(currentTrans.getTransactionAmount());
    dialog.setDate(currentTrans.getTransactionDate());
    dialog.setType(currentTrans.getTransactionType());
    dialog.setDescription(currentTrans.getTransactionDescription());
    dialog.setSelectedCategoryId(currentTrans.getCategoryId());
    dialog.setSelectedFinancialAccountId(currentTrans.getFinancialAccountId());

    connect(&dialog, &TransactionEditorDialogView::addCategoryRequested, this,
        [&](const QString& name) {
            if (categoryRepository.addCategory(name, getProfileId())) {
                QVector<Category> newCats = categoryRepository.getAllProfileCategories(getProfileId());

                int newId = -1;
                for (const auto& c : newCats) if (c.getCategoryName() == name) newId = c.getCategoryId();

                dialog.refreshCategories(newCats, newId);
            }
            else {
                transactionView->showTransactionMessage("Error", "Failed to add category", "error");
            }
        });
    connect(&dialog, &TransactionEditorDialogView::addFinancialAccountRequested, this,
        [&](const QString& name, const QString& type, double balance) {
            if (financialAccountRepository.addFinancialAccount(name, type, balance, getProfileId())) {
                QVector<FinancialAccount> newAccs = financialAccountRepository.getAllProfileFinancialAccounts(getProfileId());

                int newId = -1;
                for (const auto& a : newAccs) if (a.getFinancialAccountName() == name) newId = a.getFinancialAccountId();

                dialog.refreshFinancialAccounts(newAccs, newId);
            }
            else {
                transactionView->showTransactionMessage("Error", "Failed to add account", "error");
            }
        });

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        if (name.trimmed().isEmpty()) {
            transactionView->showTransactionMessage(tr("Error"), tr("Transaction name cannot be empty."), "error");
            return;
        }

        TransactionBuilder builder;
        builder.withId(currentTrans.getTransactionId())
            .withProfileId(getProfileId())
            .withName(name)
            .withAmount(dialog.getAmount())
            .withType(dialog.getType())
            .withDate(dialog.getDate())
            .withDescription(dialog.getDescription())
            .withCategoryId(dialog.getSelectedCategoryId())
            .withFinancialAccountId(dialog.getSelectedFinancialAccountId());

        Transaction updatedTransaction = builder.build();

        if (transactionRepository.updateTransaction(updatedTransaction)) {
            refreshTransactionsView();
        }
        else {
            transactionView->showTransactionMessage(tr("Error"), tr("Failed to update transaction."), "error");
        }
    }
}

//Method responsible for handling deletion of transaction
void TransactionController::handleDeleteTransactionRequest()
{
    int transactionId = transactionView->getSelectedTransactionId();
    if (transactionId < 0) {
        transactionView->showTransactionMessage(tr("Delete"), tr("No transaction selected."), "error");
        return;
    }

    if (!transactionRepository.removeTransactionById(transactionId))
    {
        transactionView->showTransactionMessage(tr("Delete"), tr("Failed to delete transaction."), "error");
        return;
    }
    refreshTransactionsView();
}

//Method responsible for changing amount assigned to monthly budget
void TransactionController::handleEditBudgetRequest()
{
    bool ok;
    double currentLimit = profileRepository.getBudgetLimit(getProfileId());
    double newLimit = QInputDialog::getDouble(transactionView, tr("Budget"),
        tr("Set Monthly Budget Limit (PLN):"),
        currentLimit, 0, 1000000, 2, &ok);
    if (ok) {
        profileRepository.setBudgetLimit(getProfileId(), newLimit);
        refreshTransactionsView();
    }
}

//Method that sets up filtering text and calls refresh view method where filtering occurs
void TransactionController::handleFilteringTransactionRequest(QString searchText)
{
    setFilteringText(searchText);
    refreshTransactionsView();
}

//An actual method for handling filtering specific transactions
QVector<Transaction> TransactionController::executeFilteringTransaction(const QVector<Transaction> allTransactions)
{
    auto matchFound = [&](const Transaction& trans) -> bool
        {
            QString filter = getFilteringText();
            bool nameMatches = trans.getTransactionName().contains(filter, Qt::CaseInsensitive);
            bool descriptionMatches = trans.getTransactionDescription().contains(filter, Qt::CaseInsensitive);
            bool categoryMatches = categoryRepository.getCategoryNameById(trans.getCategoryId()).contains(filter, Qt::CaseInsensitive);
            bool financialAccountMatches = financialAccountRepository.getFinancialAccountNameById(trans.getFinancialAccountId()).contains(filter, Qt::CaseInsensitive);
            bool typeMatches = trans.getTransactionType().contains(filter, Qt::CaseInsensitive);
            if (nameMatches || descriptionMatches || categoryMatches || financialAccountMatches || typeMatches)
            {
                return true;
            }
            return false;
        };
    return executeFiltering(allTransactions, matchFound);
}

//Method that sets up selected column id on which sorting will occur and calls refresh view method where an actual sorting method is called
void TransactionController::handleSortingRequest(int columnId)
{
    setSelectedColumnId(columnId);
    refreshTransactionsView();
}

//An actual method for handling sorting transactions
void TransactionController::executeSortingTransaction(QVector<Transaction>& allTransactions) const
{
    auto compareValues = [&](const auto& valA, const auto& valB) -> bool
        {
            if (getLastSortingOrder() == Qt::AscendingOrder)
            {
                return valA < valB;
            }
            else
            {
                return valA > valB;
            }
        };
    auto comparator = [&](const Transaction& itemA, const Transaction& itemB) -> bool
        {
            switch (getSelectedColumnId())
            {
            case 1://Name
                return compareValues(itemA.getTransactionName().trimmed().toLower(), itemB.getTransactionName().trimmed().toLower());
                break;
            case 2://Date
                return compareValues(itemA.getTransactionDate(), itemB.getTransactionDate());
                break;
            case 3://Description
                return compareValues(itemA.getTransactionDescription().trimmed().toLower(), itemB.getTransactionDescription().trimmed().toLower());
                break;
            case 4://Amount
                return compareValues(itemA.getTransactionAmount(), itemB.getTransactionAmount());
                break;
            case 5://Type
                return compareValues(itemA.getTransactionType(), itemB.getTransactionType());
                break;
            case 6://Category
                return compareValues(categoryRepository.getCategoryNameById(itemA.getCategoryId()).trimmed().toLower(), categoryRepository.getCategoryNameById(itemB.getCategoryId()).trimmed().toLower());
                break;
            case 7://fAccount
                return compareValues(financialAccountRepository.getFinancialAccountNameById(itemA.getFinancialAccountId()).trimmed().toLower(), financialAccountRepository.getFinancialAccountNameById(itemB.getFinancialAccountId()).trimmed().toLower());
                break;
            default:
                return compareValues(itemA.getTransactionId(), itemB.getTransactionId());
                break;
            }
        };
    std::sort(allTransactions.begin(), allTransactions.end(), comparator);
}
