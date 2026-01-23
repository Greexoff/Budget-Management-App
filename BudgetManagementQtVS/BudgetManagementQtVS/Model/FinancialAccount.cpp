/**
 * @file FinancialAccount.cpp
 * @brief Implementation of the Financial Account model.
 */
#include "Model/FinancialAccount.h"

 /** @brief Constructor initializing member variables. */
FinancialAccount::FinancialAccount(int financialAccountId, QString& financialAccountName, QString& financialAccountType, double financialAccountBalance, int profileId, double currentBalance)
	: financialAccountId(financialAccountId), financialAccountName(financialAccountName), financialAccountType(financialAccountType), financialAccountBalance(financialAccountBalance), profileId(profileId), currentBalance(currentBalance)
{ }
/** @brief Destructor. */
FinancialAccount::~FinancialAccount() {}

// Getters
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

double FinancialAccount::getFinancialAccountCurrentBalance() const
{
	return currentBalance;
}

int FinancialAccount::getIdOfProfileConnectedToFinancialAccount() const 
{
	return profileId;
}
// Setters
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

void FinancialAccount::setFinancialAccountCurrentBalance(double balance) {
	currentBalance = balance;
}
