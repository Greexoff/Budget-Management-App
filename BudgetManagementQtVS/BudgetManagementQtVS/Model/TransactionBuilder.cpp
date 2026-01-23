/**
 * @file TransactionBuilder.cpp
 * @brief Implementation of the Transaction Builder.
 */
#include "Model/TransactionBuilder.h"

 /** @brief Sets ID. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withId(int newId)
{
    if (newId < 0)
    {
       return *this;
    }

    id = newId;
    return *this;
}

/** @brief Sets Name. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withName(const QString& newName)
{
    name = newName;
    return *this;
}

/** @brief Sets Date. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withDate(const QDate& newDate)
{
    date = newDate;
    return *this;
}

/** @brief Sets Description. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withDescription(const QString& newDescription)
{
    description = newDescription;
    return *this;
}

/** @brief Sets Amount. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withAmount(double newAmount)
{
    amount = newAmount;
    return *this;
}

/** @brief Sets Type. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withType(const QString& transactionType)
{
    type = transactionType;
    return *this;
}

/** @brief Sets Category ID. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withCategoryId(int newCategoryId)
{
    categoryId = newCategoryId;
    return *this;
}

/** @brief Sets Financial Account ID. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withFinancialAccountId(int newFinancialAccountId)
{
    financialAccountId = newFinancialAccountId;
    return *this;
}

/** @brief Sets Profile ID. Returns builder reference. */
TransactionBuilder& TransactionBuilder::withProfileId(int newProfileId)
{
    profileId = newProfileId;
    return *this;
}

/** @brief Constructs and returns the Transaction object using accumulated values. */
Transaction TransactionBuilder::build() const {
    return Transaction(id, name, date, description, amount, type, categoryId, financialAccountId, profileId);
}