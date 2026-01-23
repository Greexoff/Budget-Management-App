/**
 * @file ProfileRepository.h
 * @brief Header file for the Profile Repository.
 */
#pragma once
#include <Model/Repositories/BaseRepository.h>

 /**
  * @class ProfilesRepository
  * @brief Manages database operations for user profiles.
  */
class ProfilesRepository : public BaseRepository
{
public:
    ProfilesRepository() = default;
    /**
     * @brief Retrieves all profiles associated with a user ID.
     * @param userId The ID of the user.
     * @return A list of Profile objects.
     */
    QVector<Profile> getProfilesByUserId(int userId) const;

    /**
     * @brief Creates a new profile for a user.
     * @param userId The ID of the user.
     * @param profileName The name of the new profile.
     * @return True if successful, false otherwise.
     */
    bool addProfile(int userId, QString profileName) const;

    /**
     * @brief Deletes a profile by its ID.
     * @param profileId The ID of the profile to remove.
     * @return True if successful, false otherwise.
     */
    bool removeProfileById(int profileId) const;

    /**
     * @brief Updates the name of a profile.
     * @param profileId The ID of the profile.
     * @param newName The new name.
     * @return True if successful, false otherwise.
     */
    bool updateProfile(int profileId, const QString& newName) const;

    /**
     * @brief Retrieves the monthly budget limit for a profile.
     * @param profileId The ID of the profile.
     * @return The budget limit amount.
     */
    double getBudgetLimit(int profileId) const;

    /**
     * @brief Sets the monthly budget limit for a profile.
     * @param profileId The ID of the profile.
     * @param limit The new budget limit.
     * @return True if successful, false otherwise.
     */
    bool setBudgetLimit(int profileId, double limit) const;
};
