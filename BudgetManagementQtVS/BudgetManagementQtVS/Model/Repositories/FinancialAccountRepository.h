#pragma once

#include <Model/Repositories/BaseRepository.h>


class FinancialAccountRepository : public BaseRepository {
public:
	FinancialAccountRepository() = default;

	QVector<FinancialAccount> getAllProfileFinancialAccounts(int profileId) const;

	bool addFinancialAccount(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance, int profileId);

	bool removeFinancialAccount(int financialAccountId);

	bool updateFinancialAccount(int financialAccountId, const QString& newName, const QString& newType, double newBalance);

	QString getFinancialAccountNameById(int financialAccountId) const;
};