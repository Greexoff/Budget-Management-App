#pragma once
#include<QString>
class User
{
public:
	User(int userId, QString username, QString password);
	int getUserId() const;
	QString getPassword() const;
	QString getUsername() const;

private:
	int userId;
	QString username;
	QString password;
};

class Profile
{
public:
	Profile(int profileId, int userId, QString name);
	int getProfileId() const;
	int getUserId() const;
	QString getProfileName() const;
private:
	int profileId;
	int userId;
	QString profileName;
};