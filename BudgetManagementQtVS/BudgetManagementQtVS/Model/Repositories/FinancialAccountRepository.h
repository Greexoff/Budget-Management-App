#pragma once

#include <Model/Repositories/BaseRepository.h>


class FinancialAccountRepository : public BaseRepository {
public:
	FinancialAccountRepository() = default;

	QVector<FinancialAccount> getAllProfileFinancialAccounts(int profileId) const;

	bool addFinancialAccount(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance, int profileId) const;

	bool removeFinancialAccount(int financialAccountId) const;

	bool updateFinancialAccount(int financialAccountId, const QString& newName, const QString& newType, double newBalance) const;

	QString getFinancialAccountNameById(int financialAccountId) const;
};