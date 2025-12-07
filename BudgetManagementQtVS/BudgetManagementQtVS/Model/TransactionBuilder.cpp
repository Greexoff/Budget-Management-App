#include "Model/TransactionBuilder.h"


TransactionBuilder& TransactionBuilder::withId(int newId)
{
    if (newId < 0)
    {
       return *this;
    }

    id = newId;
    return *this;
}

TransactionBuilder& TransactionBuilder::withName(const QString& newName)
{
    name = newName;
    return *this;
}

TransactionBuilder& TransactionBuilder::withDate(const QDate& newDate)
{
    date = newDate;
    return *this;
}

TransactionBuilder& TransactionBuilder::withDescription(const QString& newDescription)
{
    description = newDescription;
    return *this;
}

TransactionBuilder& TransactionBuilder::withAmount(double newAmount)
{
    amount = newAmount;
    type = (amount > 0.0) ? EXPENSE : INCOME;//Remove it later
    return *this;
}

TransactionBuilder& TransactionBuilder::withCategoryId(int newCategoryId)
{
    categoryId = newCategoryId;
    return *this;
}

TransactionBuilder& TransactionBuilder::withProfileId(int newProfileId)
{
    profileId = newProfileId;
    return *this;
}

Transaction TransactionBuilder::build() const {
    return Transaction(id, name, date, description, amount, type, categoryId, profileId);
}