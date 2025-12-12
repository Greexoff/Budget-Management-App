#include "Model/Transaction.h"

/**
 * @brief Constructs a Transaction with all specified properties
 * @param transactionId Unique database identifier
 * @param transactionName Name of transaction
 * @param transactionDate Date of occurrence
 * @param transactionDescription Additional details
 * @param transactionAmount Financial amount
 * @param transactionType INCOME or EXPENSE classification
 * @param categoryId ID of classification category
 * @param associatedProfileId ID of owning budget profile
 */
Transaction::Transaction(int transactionId, QString transactionName, QDate transactionDate, QString transactionDescription, double transactionAmount, TransactionType transactionType, int categoryId, int financialAccountId, int associatedProfileId)
{
	this->transactionId = transactionId;
	this->transactionName = transactionName;
	this->transactionDate = transactionDate;
	this->transactionDescription = transactionDescription;
	this->transactionAmount = transactionAmount;
	this->transactionType = transactionType;
	this->categoryId = categoryId;
	this->financialAccountId = financialAccountId;
	this->associatedProfileId = associatedProfileId;
}

/**
 * @brief Destructor
 */
Transaction::~Transaction() {}


//Getter implementations

/**
 * @brief Returns the transaction's unique identifier
 * @return int Transaction ID
 */
int Transaction::getTransactionId() const
{
	return transactionId;
}

/**
 * @brief Returns the transaction's name
 * @return QString Transaction name
 */
QString Transaction::getTransactionName() const
{
	return transactionName;
}

/**
 * @brief Returns the transaction date
 * @return QDate Date of transaction
 */
QDate Transaction::getTransactionDate() const
{
	return transactionDate;
}

/**
 * @brief Returns the transaction description
 * @return QString Transaction details
 */
QString Transaction::getTransactionDescription() const
{
	return transactionDescription;
}

/**
 * @brief Returns the transaction amount
 * @return double Financial amount (positive = expense, negative = income)
 */
double Transaction::getTransactionAmount() const
{
	return transactionAmount;
}

TransactionType Transaction::getTransactionType() const
{
	return transactionType;
}

/**
 * @brief Returns the ID of the associated category
 * @return int Category ID
 */
int Transaction::getCategoryId() const
{
	return categoryId;
}

int Transaction::getFinancialAccountId() const
{
	return financialAccountId;
}

/**
 * @brief Returns the ID of the owning profile
 * @return int Profile ID
 */
int Transaction::getAssociatedProfileId() const
{
	return associatedProfileId;
}
//Setter implementations

/**
 * @brief Updates the transaction identifier
 * @param id New unique identifier
 */
void Transaction::setTransactionId(int id)
{
	transactionId = id;
}

/**
 * @brief Updates the transaction name
 * @param name New name
 */
void Transaction::setTransactionName(const QString& name)
{
	transactionName = name;
}	

/**
 * @brief Updates the transaction date
 * @param date New date of occurrence
 */
void Transaction::setTransactionDate(const QDate& date)
{
	transactionDate = date;
}

/**
 * @brief Updates the transaction description
 * @param description New details about the transaction
 */
void Transaction::setTransactionDescription(const QString& description)
{
	transactionDescription = description;
}

/**
 * @brief Updates the transaction amount
 * @param amount New financial amount
 */
void Transaction::setTransactionAmount(double amount)
{
	transactionAmount = amount;
}

/**
 * @brief Updates the transaction type
 * @param type New classification (INCOME or EXPENSE)
 */
void Transaction::setTransactionType(TransactionType type)
{
	transactionType = type;
}

/**
 * @brief Updates the associated category ID
 * @param categoryId New category identifier
 */
void Transaction::setCategoryId(int id)
{
	categoryId = id;
}

void Transaction::setFinancialAccountId(int id)
{
	financialAccountId = id;
}

/**
 * @brief Updates the owning profile ID
 * @param associadedProfileId New profile identifier
 */
void Transaction::setAssociatedProfileId(int associadedProfileId) 
{
	this->associatedProfileId = associatedProfileId;
}

/**
 * @brief Updates all transaction properties
 * @param name Updated transaction name
 * @param date Updated transaction date
 * @param description Updated transaction description
 * @param amount Updated transaction amount
 * @param type Updated transaction type
 * @param categoryId Updated category ID
 * @param associatedProfileId Updated profile ID
 */
void Transaction::editTransaction(const QString& name, const QDate& date, const QString& description, double amount, TransactionType type, int categoryId,int financialAccountId, int associatedProfileId)
{
	setTransactionName(name);
	setTransactionDate(date);
	setTransactionDescription(description);
	setTransactionAmount(amount);
	setTransactionType(type);
	setCategoryId(categoryId);
	setFinancialAccountId(financialAccountId);
	setAssociatedProfileId(associatedProfileId);
}


