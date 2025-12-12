#include "Model/TransactionBuilder.h"

/**
 * @brief Sets the transaction identifier
 * @param newId New transaction ID
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withId(int newId)
{
    if (newId < 0)
    {
       return *this;
    }

    id = newId;
    return *this;
}

/**
 * @brief Sets the transaction name
 * @param newName New transaction name
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withName(const QString& newName)
{
    name = newName;
    return *this;
}

/**
 * @brief Sets the transaction date
 * @param newDate New transaction date
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withDate(const QDate& newDate)
{
    date = newDate;
    return *this;
}

/**
 * @brief Sets the transaction description
 * @param newDescription New transaction description
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withDescription(const QString& newDescription)
{
    description = newDescription;
    return *this;
}

/**
 * @brief Sets the transaction amount
 * @param newAmount New transaction amount
 * @return Reference to this builder for method chaining
 *
 * Automatically determines transaction type based on amount sign:
 * Positive amount: EXPENSE 
 * Negative amount: INCOME 
 */
TransactionBuilder& TransactionBuilder::withAmount(double newAmount)
{
    amount = newAmount;
    type = (amount > 0.0) ? EXPENSE : INCOME;//Remove it later
    return *this;
}

/**
 * @brief Sets the category identifier
 * @param newCategoryId New category ID
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withCategoryId(int newCategoryId)
{
    categoryId = newCategoryId;
    return *this;
}

TransactionBuilder& TransactionBuilder::withFinancialAccountId(int newFinancialAccountId)
{
    financialAccountId = newFinancialAccountId;
    return *this;
}

/**
 * @brief Sets the profile identifier
 * @param newProfileId New profile ID
 * @return Reference to this builder for method chaining
 */
TransactionBuilder& TransactionBuilder::withProfileId(int newProfileId)
{
    profileId = newProfileId;
    return *this;
}
TransactionBuilder& TransactionBuilder::withCategoryName(QString newCategoryName)
{
    categoryName = newCategoryName;
    return *this;
}
/**
 * @brief Constructs the final Transaction object
 * @return Completed Transaction object with all configured properties
 */
Transaction TransactionBuilder::build() const {
    return Transaction(id, name, date, description, amount, type, categoryId, financialAccountId, profileId, categoryName);
}