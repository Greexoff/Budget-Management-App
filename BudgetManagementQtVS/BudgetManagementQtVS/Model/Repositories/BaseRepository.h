#pragma once

#include <QVector.h>
#include <QString.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include <qsqldatabase.h>
#include <Model/Transaction.h>
#include <Model/DatabaseManager.h>
#include <Model/User.h>
#include <Model/Profile.h>
#include <Model/Category.h>
#include <Model/FinancialAccount.h>

/**
 * @class BaseRepository
 * @brief Abstract base class providing database connection to derived repositories
 *
 * This class ensures all repository classes have access to the shared
 * database connection managed by the DatabaseManager singleton.
 */
class BaseRepository {
public:
    BaseRepository();
    virtual ~BaseRepository() = default;

protected:
    QSqlDatabase& database;  ///< Reference to the shared database connection
};
