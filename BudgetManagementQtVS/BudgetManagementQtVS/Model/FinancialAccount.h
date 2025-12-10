#pragma once

#include <QString>

class FinancialAccount {
private:
	int financialAccountId;
	QString financialAccountName;
	QString financialAccountType;
	double financialAccountBalance;
	int profileId;

public:
	FinancialAccount(int financialAccountId, QString& financialAccountName, QString& financialAccountType, double financialAccountBalance, int profileId);

	~FinancialAccount();

	int getFinancialAccountId() const;

	QString getFinancialAccountName() const;

	QString getFinancialAccountType() const;

	double getFinancialAccountBalance() const;

	int getIdOfProfileConnectedToFinancialAccount() const;

	void setFinancialAccountId(int id);

	void setFinancialAccountName(const QString& name);

	void setFinancialAccountType(const QString& type);

	void setFinancialAccountBalance(double balance);

	
};