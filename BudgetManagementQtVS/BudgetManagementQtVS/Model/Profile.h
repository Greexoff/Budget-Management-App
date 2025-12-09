#pragma once
#include <QString>
class Profile
{
public:
	/**
	 * @brief Constructs a Profile object
	 * @param profileId Unique profile identifier
	 * @param userId ID of owning user
	 * @param name Display name of the profile
	 */
	Profile(int profileId, int userId, QString name);

	/**
	 * @brief Returns the profile's unique identifier
	 * @return int Profile ID
	 */
	int getProfileId() const;

	/**
	 * @brief Returns the ID of the owning user
	 * @return int User ID
	 */
	int getUserId() const;

	/**
	 * @brief Returns the profile's display name
	 * @return QString Profile name
	 */
	QString getProfileName() const;
private:
	int profileId;			///< Unique identifier
	int userId;				///< ID of owning user
	QString profileName;	///< Display name
};