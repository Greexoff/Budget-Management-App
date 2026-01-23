#pragma once
#include <QString>
#include <QDate>

class Transaction
{
private:
	int transactionId;						
	QString transactionName;				
	QDate transactionDate;					
	QString transactionDescription;			
	double transactionAmount;				
	QString transactionType;				
	int categoryId;							
	int financialAccountId;					
	int associatedProfileId;				
public:
	Transaction(int TransactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, QString transactionType, int categoryId, int financialAccountId, int associadedProfileId);
	~Transaction();

	// Getters	
	int getTransactionId() const;
	QString getTransactionName() const;
	QDate getTransactionDate() const;
	QString getTransactionDescription() const;
	double getTransactionAmount() const;
	int getCategoryId() const;
	int getFinancialAccountId() const;
	int getAssociatedProfileId() const;
	QString getTransactionType() const;

	// Setters	
	void setTransactionId(int id);
	void setTransactionName(const QString& name);
	void setTransactionDate(const QDate& date);
	void setTransactionDescription(const QString& description);
	void setTransactionAmount(double amount);
	void setTransactionType(QString type);
	void setCategoryId(int id);
	void setFinancialAccountId(int id);
	void setAssociatedProfileId(int associadedProfileId);
};