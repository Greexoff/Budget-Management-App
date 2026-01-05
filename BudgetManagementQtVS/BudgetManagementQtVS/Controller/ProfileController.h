#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/ProfileRepository.h"

#include "View/ProfileDialogView.h"

#include <QDebug>
class ProfileController : public BaseController
{
    Q_OBJECT
public:
    ProfileController(ProfileDialog& profileDialogRef, ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);
signals:
    void profileSelected();
    void logout();
public slots:

    void refreshProfilesForCurrentUser();
private:
    ProfileDialog& profileDialog;

    ProfilesRepository& profileRepository;

    void handleProfileSelection(int profileId);

    void handleAddProfileRequest(const QString& name);

    void handleRemoveProfileRequest(int profileId);

    void handleEditProfileRequest(int profileId, const QString& newName);

    void handleLogoutRequest();
};

