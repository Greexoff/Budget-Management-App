/**
 * @file ProfileRepository.cpp
 * @brief Implementation of the Profile Repository.
 */
#include <Model/Repositories/ProfileRepository.h>

 /**
  * @brief Queries the profiles table for all records matching the user ID.
  */
QVector<Profile> ProfilesRepository::getProfilesByUserId(int userId) const
{
    QVector<Profile> foundProfiles;

    QSqlQuery query(database);
    query.prepare("SELECT id, profile_name, user_id FROM profiles WHERE user_id = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::getting profiles that belong to user error:" << query.lastError().text();
        return foundProfiles;
    }

    while (query.next())
    {
        int profileId = query.value(0).toInt();
        QString profileName = query.value(1).toString();
        int profileUserId = query.value(2).toInt();

        Profile profile(profileId, profileUserId, profileName);
        foundProfiles.append(profile);
    }

    return foundProfiles;
}
/**
 * @brief Inserts a new profile record into the database.
 */
bool ProfilesRepository::addProfile(int userId, QString profileName) const
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO profiles (profile_name, user_id) VALUES (:profileName, :userId)");
    query.bindValue(":profileName", profileName);
    query.bindValue(":userId", userId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::adding profile to database error:" << query.lastError().text();
        return false;
    }

    return true;
}
/**
 * @brief Deletes a profile record from the database.
 */
bool ProfilesRepository::removeProfileById(int profileId) const
{
    QSqlQuery query(database);
    query.prepare("DELETE FROM profiles WHERE id = :id");
    query.bindValue(":id", profileId);

    if (!query.exec())
    {
        qDebug() << "ProfilesRepo::removing profile to database error:" << query.lastError().text();
        return false;
    }
    return true;
}
/**
 * @brief Updates the profile_name field for the specified ID.
 */
bool ProfilesRepository::updateProfile(int profileId, const QString& newName) const
{
    QSqlQuery query(database);
    query.prepare("UPDATE profiles SET profile_name = :name WHERE id = :id");
    query.bindValue(":name", newName);
    query.bindValue(":id", profileId);

    if (!query.exec()) {
        qDebug() << "ProfilesRepo::updateProfile error:" << query.lastError().text();
        return false;
    }
    return true;
}
/**
 * @brief Selects the budget_limit value for the profile.
 */
double ProfilesRepository::getBudgetLimit(int profileId) const
{
    QSqlQuery query(database);
    query.prepare("SELECT budget_limit FROM profiles WHERE id = :id");
    query.bindValue(":id", profileId);
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}
/**
 * @brief Updates the 'budget_limit' column in the 'profiles' table.
 */
bool ProfilesRepository::setBudgetLimit(int profileId, double limit) const
{
    QSqlQuery query(database);
    query.prepare("UPDATE profiles SET budget_limit = :limit WHERE id = :id");
    query.bindValue(":limit", limit);
    query.bindValue(":id", profileId);
    return query.exec();
}
