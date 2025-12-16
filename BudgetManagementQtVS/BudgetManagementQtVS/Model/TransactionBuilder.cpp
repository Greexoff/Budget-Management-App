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
    return *this;
}

TransactionBuilder& TransactionBuilder::withType(const QString& transactionType)
{
    type = transactionType;
    return *this;
}
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


TransactionBuilder& TransactionBuilder::withProfileId(int newProfileId)
{
    profileId = newProfileId;
    return *this;
}


Transaction TransactionBuilder::build() const {
    return Transaction(id, name, date, description, amount, type, categoryId, financialAccountId, profileId);
}