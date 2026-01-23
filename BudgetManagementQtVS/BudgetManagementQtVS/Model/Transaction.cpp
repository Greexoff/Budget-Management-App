/**
 * @file Transaction.cpp
 * @brief Implementation of the Transaction model.
 */
#include "Model/Transaction.h"

 /** @brief Constructor initializing all fields. */
Transaction::Transaction(int transactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, QString transactionType, int categoryId, int financialAccountId, int associatedProfileId)
{
	this->transactionId = transactionId;
	this->transactionName = transactionName;
	this->transactionDate = transactionDate;
	this->transactionDescription = transactionDescription;
	this->transactionAmount = transactionAmount;
	this->transactionType = transactionType;
	this->categoryId = categoryId;
	this->financialAccountId = financialAccountId;
	this->associatedProfileId = associatedProfileId;
}
/** @brief Destructor. */
Transaction::~Transaction() {}


//Getter implementations
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

QString Transaction::getTransactionType() const
{
	return transactionType;
}

int Transaction::getCategoryId() const
{
	return categoryId;
}

int Transaction::getFinancialAccountId() const
{
	return financialAccountId;
}

int Transaction::getAssociatedProfileId() const
{
	return associatedProfileId;
}

//Setter implementations
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

void Transaction::setTransactionType(QString type)
{
	transactionType = type;
}

void Transaction::setCategoryId(int id)
{
	categoryId = id;
}

void Transaction::setFinancialAccountId(int id)
{
	financialAccountId = id;
}

void Transaction::setAssociatedProfileId(int associadedProfileId) 
{
	this->associatedProfileId = associatedProfileId;
}



