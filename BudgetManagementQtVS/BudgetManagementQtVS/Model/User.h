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
