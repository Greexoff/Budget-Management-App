#include <Model/SortStrategy.h>

void SortStrategy::sort(QVector<Transaction>& allTransactions)
{
	if (order == SortOrder::ASCENDING)
	{
		std::sort(allTransactions.begin(), allTransactions.end(), [&](const Transaction& transactionA, const Transaction& transactionB) {
			return compare(transactionA, transactionB);
			});
	}
	if (order == SortOrder::DESCENDING)
	{
		std::sort(allTransactions.begin(), allTransactions.end(), [&](const Transaction& transactionA, const Transaction& transactionB) {
			return compare(transactionB, transactionA);
			});
	}
}

void SortContext::setStrategy(std::unique_ptr<SortStrategy> strategy)
{
	this->strategy = std::move(strategy);
} 
void SortContext::executeStrategy(QVector<Transaction>& allTransactions) 
{
	strategy->sort(allTransactions);
}

bool SortByName::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	QString nameA = transactionA.getTransactionName().trimmed().toLower();
	QString nameB = transactionB.getTransactionName().trimmed().toLower();
	return nameA < nameB;
}

bool SortByDate::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionDate() < transactionB.getTransactionDate();
}

bool SortByDescription::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	QString nameA = transactionA.getTransactionDescription().trimmed().toLower();
	QString nameB = transactionB.getTransactionDescription().trimmed().toLower();
	return nameA < nameB;
}

bool SortByAmount::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionAmount() < transactionB.getTransactionAmount();
}

bool SortByType::compare(const Transaction& transactionA, const Transaction& transactionB)//IN PROGRESS: CHANGE LOGIC TO SORT BY NAME NOT VALUE OF ENUM
{
	return transactionA.getTransactionType() < transactionB.getTransactionType();
}

bool SortByCategory::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	QString nameA = categoryRepository.getCategoryNameById(transactionA.getCategoryId()).trimmed().toLower();
	QString nameB = categoryRepository.getCategoryNameById(transactionB.getCategoryId()).trimmed().toLower();
	return nameA < nameB;
}

bool SortByFinancialAccount::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	QString nameA = fAccountRepository.getFinancialAccountNameById(transactionA.getFinancialAccountId()).trimmed().toLower();
	QString nameB = fAccountRepository.getFinancialAccountNameById(transactionB.getFinancialAccountId()).trimmed().toLower();
	return nameA < nameB;
	
}

