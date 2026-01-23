/**
 * @file FinancialAccountRepository.h
 * @brief Header file for the Financial Account Repository.
 */
#pragma once

#include <Model/Repositories/BaseRepository.h>

 /**
  * @class FinancialAccountRepository
  * @brief Manages database operations for financial accounts (e.g., Bank, Cash).
  */
class FinancialAccountRepository : public BaseRepository {
public:
	FinancialAccountRepository() = default;
    /**
     * @brief Retrieves all financial accounts for a profile.
     * @param profileId The ID of the profile.
     * @return A list of FinancialAccount objects.
     */
    QVector<FinancialAccount> getAllProfileFinancialAccounts(int profileId) const;

    /**
     * @brief Adds a new financial account to the database.
     * @param financialAccountName The name of the account.
     * @param financialAccountType The type of the account (e.g., "Cash", "Bank").
     * @param financialAccountBalance The initial balance.
     * @param profileId The profile ID associated with the account.
     * @return True if successful, false otherwise.
     */
    bool addFinancialAccount(const QString& financialAccountName, const QString& financialAccountType, double financialAccountBalance, int profileId) const;

    /**
     * @brief Removes a financial account by its ID.
     * @param financialAccountId The ID of the account to remove.
     * @return True if successful, false otherwise.
     */
    bool removeFinancialAccount(int financialAccountId) const;

    /**
     * @brief Updates the details of an existing financial account.
     * @param financialAccountId The ID of the account to update.
     * @param newName The new name.
     * @param newType The new type.
     * @param newBalance The new balance.
     * @return True if successful, false otherwise.
     */
    bool updateFinancialAccount(int financialAccountId, const QString& newName, const QString& newType, double newBalance) const;

    /**
     * @brief Retrieves the name of a financial account by its ID.
     * @param financialAccountId The ID of the account.
     * @return The name of the account.
     */
    QString getFinancialAccountNameById(int financialAccountId) const;
};