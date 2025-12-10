#include <Model/Repositories/ProfileRepository.h>

/**
 * @brief Retrieves all profiles belonging to a specific user
 * @param userId ID of the user
 * @return Vector of Profile objects for the user
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
 * @brief Creates a new profile for a user
 *
 * Profile names must be unique across all users. The database trigger
 * `insertDefaultCategory` will automatically create a default "None" category
 * for the new profile.
 *
 * @param userId ID of the owning user
 * @param profileName Name for the new profile
 * @return True if profile created successfully, false otherwise
 */
bool ProfilesRepository::addProfile(int userId, QString profileName)
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
 * @brief Deletes a profile by ID
 * @param profileId ID of profile to delete
 * @return True if profile deleted successfully, false otherwise
 */
bool ProfilesRepository::removeProfileById(int profileId)
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

//edycja profilu
bool ProfilesRepository::updateProfile(int profileId, const QString& newName)
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