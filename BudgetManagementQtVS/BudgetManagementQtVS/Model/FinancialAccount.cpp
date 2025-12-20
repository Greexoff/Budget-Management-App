#include "Model/FinancialAccount.h"

FinancialAccount::FinancialAccount(int financialAccountId, QString& financialAccountName, QString& financialAccountType, double financialAccountBalance, int profileId)
	: financialAccountId(financialAccountId), financialAccountName(financialAccountName), financialAccountType(financialAccountType), financialAccountBalance(financialAccountBalance), profileId(profileId)
{ }

FinancialAccount::~FinancialAccount() {}

int FinancialAccount::getFinancialAccountId() const 
{
	return financialAccountId;
}

QString FinancialAccount::getFinancialAccountName() const 
{
	return financialAccountName;
}

QString FinancialAccount::getFinancialAccountType() const 
{
	return financialAccountType;
}

double FinancialAccount::getFinancialAccountBalance() const 
{
	return financialAccountBalance;
}

int FinancialAccount::getIdOfProfileConnectedToFinancialAccount() const 
{
	return profileId;
}

void FinancialAccount::setFinancialAccountId(int id) 
{
	financialAccountId = id; 
}

void FinancialAccount::setFinancialAccountName(const QString& name) 
{ 
	financialAccountName = name;
}

void FinancialAccount::setFinancialAccountType(const QString& type) 
{
	financialAccountType = type;
}

void FinancialAccount::setFinancialAccountBalance(double balance) 
{
	financialAccountBalance = balance;
}
