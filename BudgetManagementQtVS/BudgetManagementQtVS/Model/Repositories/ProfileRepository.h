#pragma once
#include <Model/Repositories/BaseRepository.h>
/**
 * @class ProfilesRepository
 * @brief Handles database operations for budget profile management
 */
class ProfilesRepository : public BaseRepository
{
public:
    ProfilesRepository() = default;

    /**
     * @brief Retrieves all profiles belonging to a specific user
     * @param userId ID of the user
     * @return Vector of Profile objects for the user
     */
    QVector<Profile> getProfilesByUserId(int userId) const;

    /**
     * @brief Creates a new profile for a user
     * @param userId ID of the owning user
     * @param profileName Name for the new profile
     * @return True if profile created successfully, false otherwise
     */
    bool addProfile(int userId, QString profileName);

    /**
     * @brief Deletes a profile by ID
     * @param profileId ID of profile to delete
     * @return True if profile deleted successfully, false otherwise
     */
    bool removeProfileById(int profileId);

    //edycja profilu
    bool updateProfile(int profileId, const QString& newName);
};
