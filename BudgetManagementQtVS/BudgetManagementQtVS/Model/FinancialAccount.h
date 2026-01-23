/**
 * @file FinancialAccount.h
 * @brief Header file for the Financial Account model class.
 */
#pragma once

#include <QString>

 /**
  * @class FinancialAccount
  * @brief Represents a financial account (e.g., Bank, Wallet) with balance tracking.
  */
class FinancialAccount {
private:
	int financialAccountId;			///< Unique ID of the account.
	QString financialAccountName;	///< Name of the account.
	QString financialAccountType;	///< Type of account (e.g., Cash, Bank).
	double financialAccountBalance; ///< Initial/Base balance.
	double currentBalance;			///< Calculated balance including transactions.
	int profileId;					///< Connected profile ID.

public:
    /**
     * @brief Constructs a FinancialAccount object.
     * @param financialAccountId Unique ID.
     * @param financialAccountName Name.
     * @param financialAccountType Type.
     * @param financialAccountBalance Initial balance.
     * @param profileId Profile ID.
     * @param currentBalance Current calculated balance (default 0.0).
     */
    FinancialAccount(int financialAccountId, QString& financialAccountName, QString& financialAccountType, double financialAccountBalance, int profileId, double currentBalance = 0.0);

    ~FinancialAccount();

    /** @brief Returns account ID. */
    int getFinancialAccountId() const;
    /** @brief Returns account name. */
    QString getFinancialAccountName() const;
    /** @brief Returns account type. */
    QString getFinancialAccountType() const;
    /** @brief Returns the base/initial balance. */
    double getFinancialAccountBalance() const;
    /** @brief Returns the current calculated balance. */
    double getFinancialAccountCurrentBalance() const;
    /** @brief Returns the ID of the connected profile. */
    int getIdOfProfileConnectedToFinancialAccount() const;

    /** @brief Sets ID. @param id New ID. */
    void setFinancialAccountId(int id);

    /** @brief Sets Name. @param name New name. */
    void setFinancialAccountName(const QString& name);

    /** @brief Sets Type. @param type New type. */
    void setFinancialAccountType(const QString& type);

    /** @brief Sets Initial Balance. @param balance New balance. */
    void setFinancialAccountBalance(double balance);

    /** @brief Sets Current Balance. @param balance New current balance. */
    void setFinancialAccountCurrentBalance(double balance);
};