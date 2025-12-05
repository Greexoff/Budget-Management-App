#include "TransactionRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

TransactionRepository::TransactionRepository(): database(DatabaseManager::instance().database())
{
}

QVector<Transaction> TransactionRepository::getAll() const
{
    QVector<Transaction> result;

    QSqlQuery query(database);


    if (!query.exec("SELECT id, name, date, description, amount, type FROM transactions"))
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
        QString typeStr = query.value(5).toString();

        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");


        TransactionType type = (typeStr == "INCOME") ? INCOME : EXPENSE;
 
        int categoryId = 0;     

        Transaction transaction(id, name, date, description, amount, type, categoryId);
        result.append(transaction);
    }

    return result;
}

bool TransactionRepository::add(const Transaction& transaction)
{
    QSqlQuery query(database);

    query.prepare(
        "INSERT INTO transactions (name, type, date, description, amount) "
        "VALUES (:name, :type, :date, :description, :amount)");

    query.bindValue(":name", transaction.getTransactionName());


    QString typeStr = (transaction.getTransactionAmount() >= 0.0) ? "INCOME" : "EXPENSE";
    query.bindValue(":type", typeStr);

    query.bindValue(":date", transaction.getTransactionDate().toString("yyyy-MM-dd"));
    query.bindValue(":description", transaction.getTransactionDescription());
    query.bindValue(":amount", transaction.getTransactionAmount());

    if (!query.exec())
    {
        qDebug() << "TransactionRepository::add error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool TransactionRepository::removeById(int id)
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