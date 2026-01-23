/**
 * @file Transaction.h
 * @brief Header file for the Transaction model class.
 */
#pragma once
#include <QString>
#include <QDate>

 /**
  * @class Transaction
  * @brief Represents a single financial transaction (Income or Expense).
  */
class Transaction
{
private:
	int transactionId;              ///< Unique ID.
	QString transactionName;        ///< Short name/title.
	QDate transactionDate;          ///< Date of occurrence.
	QString transactionDescription; ///< Detailed description.
	double transactionAmount;       ///< Monetary value.
	QString transactionType;        ///< "Income" or "Expense".
	int categoryId;                 ///< Associated Category ID.
	int financialAccountId;         ///< Associated Financial Account ID.
	int associatedProfileId;        ///< Associated Profile ID.	
public:
    /**
     * @brief Constructs a Transaction object.
     * @param TransactionId Unique ID.
     * @param transactionName Name.
     * @param transactionDate Date.
     * @param transactionDescription Description.
     * @param transactionAmount Amount.
     * @param transactionType Type string.
     * @param categoryId Category ID.
     * @param financialAccountId Account ID.
     * @param associadedProfileId Profile ID.
     */
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
    /** @brief Sets ID. @param id New ID. */
    void setTransactionId(int id);

    /** @brief Sets name. @param name New name. */
    void setTransactionName(const QString& name);

    /** @brief Sets date. @param date New date. */
    void setTransactionDate(const QDate& date);

    /** @brief Sets description. @param description New description. */
    void setTransactionDescription(const QString& description);

    /** @brief Sets amount. @param amount New amount. */
    void setTransactionAmount(double amount);

    /** @brief Sets type. @param type New type. */
    void setTransactionType(QString type);

    /** @brief Sets category ID. @param id New category ID. */
    void setCategoryId(int id);

    /** @brief Sets financial account ID. @param id New account ID. */
    void setFinancialAccountId(int id);

    /** @brief Sets profile ID. @param associadedProfileId New profile ID. */
    void setAssociatedProfileId(int associadedProfileId);
};