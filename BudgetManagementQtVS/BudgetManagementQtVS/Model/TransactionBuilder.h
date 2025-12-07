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
	TransactionBuilder& withProfileId(int newProfileId);
	Transaction build() const;
private:
	int id = 0;
	QString name;
	QDate date = QDate::currentDate();
	QString description;
	double amount = 0.0;
	TransactionType type = EXPENSE;
	int categoryId = 1;
	int profileId = -1;
};