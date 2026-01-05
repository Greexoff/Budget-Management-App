#pragma once
#include <QString>
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