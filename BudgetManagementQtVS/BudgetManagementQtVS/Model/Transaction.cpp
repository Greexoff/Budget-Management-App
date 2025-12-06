#include "Model/Transaction.h"

Transaction::Transaction(int transactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, TransactionType transactionType, int categoryId, int associatedProfileId)
{
	this->transactionId = transactionId;
	this->transactionName = transactionName;
	this->transactionDate = transactionDate;
	this->transactionDescription = transactionDescription;
	this->transactionAmount = transactionAmount;
	this->transactionType = transactionType;
	this->categoryId = categoryId;
	this->associatedProfileId = associatedProfileId;
}

Transaction::~Transaction() {}


//getters

int Transaction::getTransactionId() const
{
	return transactionId;
}

QString Transaction::getTransactionName() const
{
	return transactionName;
}

QDate Transaction::getTransactionDate() const
{
	return transactionDate;
}

QString Transaction::getTransactionDescription() const
{
	return transactionDescription;
}

double Transaction::getTransactionAmount() const
{
	return transactionAmount;
}

int Transaction::getCategoryId() const
{
	return categoryId;
}
int Transaction::getAssociatedProfileId() const
{
	return associatedProfileId;
}

//setters

void Transaction::setTransactionId(int id)
{
	transactionId = id;
}

void Transaction::setTransactionName(const QString& name)
{
	transactionName = name;
}	

void Transaction::setTransactionDate(const QDate& date)
{
	transactionDate = date;
}

void Transaction::setTransactionDescription(const QString& description)
{
	transactionDescription = description;
}

void Transaction::setTransactionAmount(double amount)
{
	transactionAmount = amount;
}

void Transaction::setTransactionType(TransactionType type)
{
	transactionType = type;
}

void Transaction::setCategoryId(int id)
{
	categoryId = id;
}
void Transaction::setAssociatedProfileId(int associadedProfileId) 
{
	this->associatedProfileId = associatedProfileId;
}
void Transaction::editTransaction(const QString& name, const QDate& date, const QString& description, double amount, TransactionType type, int categoryId, int associatedProfileId)
{
	setTransactionName(name);
	setTransactionDate(date);
	setTransactionDescription(description);
	setTransactionAmount(amount);
	setTransactionType(type);
	setCategoryId(categoryId);
	setAssociatedProfileId(associatedProfileId);
}


