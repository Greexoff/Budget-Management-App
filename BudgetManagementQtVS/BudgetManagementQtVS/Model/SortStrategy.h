#pragma once
#include <Model/Transaction.h>
#include <QVector>
#include <algorithm>
#include <memory>

enum class SortOrder {ASCENDING, DESCENDING};

class SortStrategy {
public:
	SortStrategy(SortOrder order) : order(order) {}
	virtual ~SortStrategy() = default;

	void sort(QVector<Transaction>& allTransactions);
protected:
	virtual bool compare(const Transaction& transactionA, const Transaction& transactionB) = 0;
private:
	SortOrder order;
};

class SortContext {
public:
	void setStrategy(std::unique_ptr<SortStrategy> strategy);

	void executeStrategy(QVector<Transaction>& allTransactions);
private:
	std::unique_ptr<SortStrategy> strategy = nullptr;
};

class SortByName : public SortStrategy
{
public:
	SortByName(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByDate : public SortStrategy
{
public:
	SortByDate(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByDescription : public SortStrategy
{
public:
	SortByDescription(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByAmount : public SortStrategy
{
public:
	SortByAmount(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByType : public SortStrategy
{
public:
	SortByType(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByCategory : public SortStrategy
{
public:
	SortByCategory(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

class SortByFinancialAccount : public SortStrategy
{
public:
	SortByFinancialAccount(SortOrder order) : SortStrategy(order) {}
protected:
	bool compare(const Transaction& transactionA, const Transaction& transactionB) override;
};

