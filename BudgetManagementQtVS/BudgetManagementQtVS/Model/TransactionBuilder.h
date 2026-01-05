#pragma once
#include <QString>
#include <QDate>
#include "Model/Transaction.h"


class TransactionBuilder
{
public:

	TransactionBuilder() = default;

	TransactionBuilder& withId(int newId);

	TransactionBuilder& withName(const QString& newName);

	TransactionBuilder& withDate(const QDate& newDate);

	TransactionBuilder& withDescription(const QString& newDescription);

	TransactionBuilder& withAmount(double newAmount);

	TransactionBuilder& withCategoryId(int newCategoryId);

	TransactionBuilder& withFinancialAccountId(int newFinancialAccountId);

	TransactionBuilder& withProfileId(int newProfileId);

	TransactionBuilder& withType(const QString& transactionType);

	Transaction build() const;
private:
	int id = 0;                         
	QString name;                      
	QDate date = QDate::currentDate();  
	QString description;               
	double amount = 0.0;                
	QString type = "EXPENSE";			
	int categoryId = 1;                 
	int financialAccountId = 1;
	int profileId = -1;                 
};