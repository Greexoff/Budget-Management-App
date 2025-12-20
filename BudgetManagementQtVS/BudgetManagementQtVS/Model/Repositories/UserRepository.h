#pragma once
#include <QCryptographicHash>
#include <QUuid>
#include <Model/Repositories/BaseRepository.h>

class UserRepository : public BaseRepository
{
public:
    UserRepository() = default;

    int getUserIdBasedOnUsername(QString username, QString password) const;

    bool addUser(QString username, QString password);

    bool removeUserById(int userId);

    bool checkIfUserExists(const QString& username);
};