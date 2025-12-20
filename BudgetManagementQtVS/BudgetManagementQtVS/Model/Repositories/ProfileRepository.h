#pragma once
#include <Model/Repositories/BaseRepository.h>

class ProfilesRepository : public BaseRepository
{
public:
    ProfilesRepository() = default;

    QVector<Profile> getProfilesByUserId(int userId) const;

    bool addProfile(int userId, QString profileName);

    bool removeProfileById(int profileId);

    bool updateProfile(int profileId, const QString& newName);

    double getBudgetLimit(int profileId) const;

    bool setBudgetLimit(int profileId, double limit);
};
