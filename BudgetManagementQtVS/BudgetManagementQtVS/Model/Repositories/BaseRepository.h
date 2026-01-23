/**
 * @file BaseRepository.h
 * @brief Header file for the base repository class.
 */
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
  * @brief Abstract base class for all repositories.
  * Provides a common connection to the database instance for all derived repositories.
  */
class BaseRepository {
public:
    /**
     * @brief Constructs the BaseRepository and retrieves the database connection.
     */
    BaseRepository();

    /**
     * @brief Virtual destructor.
     */
    virtual ~BaseRepository() = default;

protected:
    QSqlDatabase& database; ///< Reference to the active database connection.
};
