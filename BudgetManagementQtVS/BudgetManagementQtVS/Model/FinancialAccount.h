#pragma once

#include <QString>

class FinancialAccount {
private:
	int financialAccountId;
	QString financialAccountName;
	QString financialAccountType;
	double financialAccountBalance;
	double currentBalance;
	int profileId;

public:
	FinancialAccount(int financialAccountId, QString& financialAccountName, QString& financialAccountType, double financialAccountBalance, int profileId, double currentBalance = 0.0);

	~FinancialAccount();

	int getFinancialAccountId() const;

	QString getFinancialAccountName() const;

	QString getFinancialAccountType() const;

	double getFinancialAccountBalance() const;

	double getFinancialAccountCurrentBalance() const;

	int getIdOfProfileConnectedToFinancialAccount() const;

	void setFinancialAccountId(int id);

	void setFinancialAccountName(const QString& name);

	void setFinancialAccountType(const QString& type);

	void setFinancialAccountBalance(double balance);

	void setFinancialAccountCurrentBalance(double balance) ;

	
};