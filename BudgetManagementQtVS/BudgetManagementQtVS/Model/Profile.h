/**
 * @file Profile.h
 * @brief Header file for the Profile model class.
 */
#pragma once
#include <QString>

 /**
  * @class Profile
  * @brief Represents a user profile containing specific financial data settings.
  */
class Profile
{
public:
    /**
     * @brief Constructs a Profile object.
     * @param profileId Unique profile ID.
     * @param userId ID of the user owning this profile.
     * @param name Name of the profile.
     */
    Profile(int profileId, int userId, QString name);

    /** @brief Returns profile ID. */
    int getProfileId() const;
    /** @brief Returns connected user ID. */
    int getUserId() const;
    /** @brief Returns profile name. */
    QString getProfileName() const;
private:
    int profileId;          ///< Unique identifier.
    int userId;             ///< Owner User ID.
    QString profileName;    ///< Profile display name.
};