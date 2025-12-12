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
	return transactionA.getTransactionName() < transactionB.getTransactionName();
}

bool SortByDate::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionDate() < transactionB.getTransactionDate();
}

bool SortByDescription::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionDescription() < transactionB.getTransactionDescription();
}

bool SortByAmount::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionAmount() < transactionB.getTransactionAmount();
}

bool SortByType::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionType() < transactionB.getTransactionType();
}

bool SortByCategory::compare(const Transaction& transactionA, const Transaction& transactionB)
{
	return transactionA.getTransactionCategoryName() < transactionB.getTransactionCategoryName();
}

bool SortByFinancialAccount::compare(const Transaction& transactionA, const Transaction& transactionB)//IN FUTURE: SWITCH TO SORT BY FACCOUNT NAME, NOT ITS ID
{
	return transactionA.getFinancialAccountId() < transactionB.getFinancialAccountId();
}

