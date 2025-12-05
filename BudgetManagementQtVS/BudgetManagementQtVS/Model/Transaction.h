#pragma once
#include <QString>
#include <QDate>

enum TransactionType
{
	INCOME,
	EXPENSE
};

class Transaction
{
private:
	//id,nazwe,date,kat,opis,kwota,(typ-klasa)
	int transactionId;
	QString transactionName;
	QDate transactionDate;
	QString transactionDescription;
	double transactionAmount;
	TransactionType transactionType;
	int categoryId;
public:
	//add,rem,edit
	//Transaction();
	//~Transaction();


	// constructor
	Transaction(int TransactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, TransactionType transactionType, int categoryId);

	// destructor
	~Transaction();


	// getters	
	int getTransactionId() const;
	QString getTransactionName() const;
	QDate getTransactionDate() const;
	QString getTransactionDescription() const;
	double getTransactionAmount() const;
	int getCategoryId() const;

	// setters	
	void setTransactionId(int id);
	void setTransactionName(const QString& name);
	void setTransactionDate(const QDate& date);
	void setTransactionDescription(const QString& description);
	void setTransactionAmount(double amount);
	void setTransactionType(TransactionType type);
	void setCategoryId(int id);

	void editTransaction(const QString& name, const QDate& date, const QString& description, double amount, TransactionType type, int categoryId);

};