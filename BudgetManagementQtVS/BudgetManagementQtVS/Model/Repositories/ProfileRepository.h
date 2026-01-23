#pragma once
#include <Model/Repositories/BaseRepository.h>

class ProfilesRepository : public BaseRepository
{
public:
    ProfilesRepository() = default;

    QVector<Profile> getProfilesByUserId(int userId) const;

    bool addProfile(int userId, QString profileName) const;

    bool removeProfileById(int profileId) const;

    bool updateProfile(int profileId, const QString& newName) const;

    double getBudgetLimit(int profileId) const;

    bool setBudgetLimit(int profileId, double limit) const;
};
