#include "Model/User&Profiles.h"

User::User(int userId, QString username, QString password)
{
	this->userId = userId;
	this->username = username;
	this->password = password;
}

int User::getUserId() const
{
	return userId;
}

QString User::getUsername() const
{
	return username;
}

QString User::getPassword() const
{
	return password;
}

Profile::Profile(int profileId, int userId, QString profileName)
{
	this->profileId = profileId;
	this->userId = userId;
	this->profileName = profileName;
}

int Profile::getProfileId() const
{
	return profileId;
}

int Profile::getUserId() const
{
	return userId;
}

QString Profile::getProfileName() const
{
	return profileName;
}