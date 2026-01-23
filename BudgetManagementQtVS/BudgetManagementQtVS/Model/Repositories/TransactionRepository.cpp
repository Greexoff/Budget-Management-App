#include <Model/Repositories/TransactionRepository.h>


QVector<Transaction> TransactionRepository::getAllProfileTransaction(int profileId) const
{
    QVector<Transaction> result;
    QSqlQuery query(database);

    query.prepare(
        "SELECT id, name, date, description, amount, type, category_id, financialAccount_id, profile_id "
        "FROM transactions WHERE profile_id = :profileId"
    );
    query.bindValue(":profileId", profileId);

    if (!query.exec())
    {
        qDebug() << "Transaction retrieval for profile failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString type = query.value(5).toString();
        int categoryId = query.value(6).toInt();
        int financialAccountId = query.value(7).toInt();
        int profileId = query.value(8).toInt();

        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");

        Transaction transaction(id, name, date, description, amount, type, categoryId, financialAccountId, profileId);
        result.append(transaction);
    }

    return result;
}

QVector<Transaction> TransactionRepository::getAll() const
{
    QVector<Transaction> result;

    QSqlQuery query(database);

    if (!query.exec("SELECT id, name, date, description, amount, type, profile_id, category_id, financialAccount_id FROM transactions"))
    {
        qDebug() << "TransactionRepository::getAll error:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString dateStr = query.value(2).toString();
        QString description = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString type = query.value(5).toString();
        int associatedProfileId = query.value(6).toInt();
        int categoryId = query.value(7).toInt();
        int financialAccountId = query.value(8).toInt();
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        
        Transaction transaction(id, name, date, description, amount, type, categoryId,financialAccountId, associatedProfileId);
        result.append(transaction);
    }

    return result;
}

bool TransactionRepository::addTransaction(const Transaction& transaction) const
{
    QSqlQuery query(database);
    query.prepare(
        "INSERT INTO transactions (name, type, date, description, amount, category_id ,financialAccount_id, profile_id) "
        "VALUES (:name, :type, :date, :description, :amount, :category_id, :financialAccount_id, :profile_id)"
    );

    query.bindValue(":name", transaction.getTransactionName());
    query.bindValue(":type", transaction.getTransactionType());
    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd"));
    query.bindValue(":description", transaction.getTransactionDescription());
    query.bindValue(":amount", transaction.getTransactionAmount());
    query.bindValue(":profile_id", transaction.getAssociatedProfileId());
    query.bindValue(":category_id", transaction.getCategoryId());
    query.bindValue(":financialAccount_id", transaction.getFinancialAccountId());

    if (!query.exec()) {
        qDebug() << "TransactionRepository::add error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool TransactionRepository::removeTransactionById(int id) const
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "TransactionRepository::removeById error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool TransactionRepository::updateTransaction(const Transaction& transaction) const
{
    QSqlQuery query(database);

    query.prepare(
        "UPDATE transactions SET "
        "name=:name, "
        "description=:desc, "
        "date=:date, "          
        "amount=:amount, "
        "category_id=:catId, "
        "type=:type, "
        "financialAccount_id=:financialAccountId "
        "WHERE id=:id"
    );

    query.bindValue(":name", transaction.getTransactionName());
    query.bindValue(":desc", transaction.getTransactionDescription());
    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd")); 
    query.bindValue(":amount", transaction.getTransactionAmount());
    query.bindValue(":catId", transaction.getCategoryId());
    query.bindValue(":type", transaction.getTransactionType());
    query.bindValue(":id", transaction.getTransactionId());
    query.bindValue(":financialAccountId", transaction.getFinancialAccountId());

    if (!query.exec()) {
        qDebug() << "TransactionRepo::update error:" << query.lastError().text();
        return false;
    }
    return true;
}

Transaction TransactionRepository::getTransactionById(int id) const
{
    QSqlQuery query(database);
    query.prepare("SELECT id, name, date, description, amount, type, category_id, profile_id, financialAccount_id FROM transactions WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        int tId = query.value(0).toInt();
        QString name = query.value(1).toString();
        QDate date = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        QString desc = query.value(3).toString();
        double amount = query.value(4).toDouble();
        QString type = query.value(5).toString();
        int catId = query.value(6).toInt();
        int profId = query.value(7).toInt();
        int financialAccountId = query.value(8).toInt();

        return Transaction(tId, name, date, desc, amount, type, catId, financialAccountId, profId);
    }
    return Transaction(-1, "", QDate(), "", 0, "Expense", 1, -1, 1);
}

double TransactionRepository::getMonthlyExpenses(int profileId, int month, int year) const
{
    QSqlQuery query(database);
    query.prepare(
        "SELECT SUM(amount) "
        "FROM transactions "
        "WHERE profile_id = :id "
        "AND type = 'Expense'"
        "AND strftime('%m', date) = :month AND strftime('%Y', date) = :year"
    );

    query.bindValue(":id", profileId);
    query.bindValue(":month", QString("%1").arg(month, 2, 10, QChar('0')));
    query.bindValue(":year", QString::number(year));

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double TransactionRepository::getSumByTypeAndDate(int profileId, const QString& type, const QDate& start, const QDate& end) const
{
    QSqlQuery query(database);


    query.prepare(
        "SELECT SUM(amount) FROM transactions "
        "WHERE profile_id = :pid AND UPPER(type) = UPPER(:type) "
        "AND date >= :start AND date <= :end"
    );
    query.bindValue(":pid", profileId);
    query.bindValue(":type", type);
    query.bindValue(":start", start.toString("yyyy-MM-dd"));
    query.bindValue(":end", end.toString("yyyy-MM-dd"));

    if (query.exec() && query.next()) {

        return query.value(0).toDouble();
    }

    return 0.0;
}

double TransactionRepository::getAllTimeMonthlyAverageExpense(int profileId) const
{

    QSqlQuery sumQuery(database);
    sumQuery.prepare("SELECT SUM(amount) FROM transactions WHERE profile_id = :pid AND type = 'Expense'");
    sumQuery.bindValue(":pid", profileId);

    double totalExpense = 0.0;
    if (sumQuery.exec() && sumQuery.next()) {
        totalExpense = sumQuery.value(0).toDouble();
    }

 
    QSqlQuery countQuery(database);
 
    countQuery.prepare("SELECT COUNT(DISTINCT strftime('%Y-%m', date)) FROM transactions WHERE profile_id = :pid AND type = 'Expense'");
    countQuery.bindValue(":pid", profileId); 

    int monthCount = 1;
    if (countQuery.exec() && countQuery.next()) {
        monthCount = countQuery.value(0).toInt();
    }
    if (monthCount == 0) monthCount = 1;

    return totalExpense / monthCount;
}

QMap<int, double> TransactionRepository::getExpensesByCategory(int profileId, const QDate& start, const QDate& end) const
{
    QMap<int, double> results;
    QSqlQuery query(database);

    query.prepare("SELECT category_id, SUM(amount) FROM transactions "
        "WHERE profile_id = :pid AND type = 'Expense' "
        "AND date >= :start AND date <= :end "
        "GROUP BY category_id");

    query.bindValue(":pid", profileId);
    query.bindValue(":start", start.toString("yyyy-MM-dd"));
    query.bindValue(":end", end.toString("yyyy-MM-dd"));

    if (query.exec()) {
        while (query.next()) {
            results.insert(query.value(0).toInt(), query.value(1).toDouble());
        }
    }
    else {
        qDebug() << "Error getting expenses by category:" << query.lastError().text();
    }
    return results;
}