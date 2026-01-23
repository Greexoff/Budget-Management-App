/**
 * @file TransactionBuilder.h
 * @brief Header file for the Transaction Builder pattern class.
 */
#pragma once
#include <QString>
#include <QDate>
#include "Model/Transaction.h"

 /**
  * @class TransactionBuilder
  * @brief Implements the Builder Pattern to facilitate the creation of complex Transaction objects.
  */
class TransactionBuilder
{
public:
    /** @brief Default constructor. */
    TransactionBuilder() = default;

    /** @brief Sets ID. @param newId The ID. */
    TransactionBuilder& withId(int newId);

    /** @brief Sets Name. @param newName The name. */
    TransactionBuilder& withName(const QString& newName);

    /** @brief Sets Date. @param newDate The date. */
    TransactionBuilder& withDate(const QDate& newDate);

    /** @brief Sets Description. @param newDescription The description. */
    TransactionBuilder& withDescription(const QString& newDescription);

    /** @brief Sets Amount. @param newAmount The amount. */
    TransactionBuilder& withAmount(double newAmount);

    /** @brief Sets Category ID. @param newCategoryId The category ID. */
    TransactionBuilder& withCategoryId(int newCategoryId);

    /** @brief Sets Account ID. @param newFinancialAccountId The account ID. */
    TransactionBuilder& withFinancialAccountId(int newFinancialAccountId);

    /** @brief Sets Profile ID. @param newProfileId The profile ID. */
    TransactionBuilder& withProfileId(int newProfileId);

    /** @brief Sets Type. @param transactionType The type string. */
    TransactionBuilder& withType(const QString& transactionType);

    /**
     * @brief Finalizes construction and returns the Transaction object.
     * @return Constructed Transaction instance.
     */
    Transaction build() const;
private:
    int id = 0;                         ///< Default ID.
    QString name;                       ///< Name buffer.
    QDate date = QDate::currentDate();  ///< Date buffer (defaults to today).
    QString description;                ///< Description buffer.
    double amount = 0.0;                ///< Amount buffer.
    QString type = "EXPENSE";           ///< Type buffer (defaults to Expense).
    int categoryId = 1;                 ///< Category ID buffer (defaults to 1).
    int financialAccountId = 1;         ///< Account ID buffer (defaults to 1).
    int profileId = -1;                 ///< Profile ID buffer.               
};