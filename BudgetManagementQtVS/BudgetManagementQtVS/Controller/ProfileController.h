#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/ProfileRepository.h"

#include "View/ProfileDialogView.h"

#include <QPointer>
class ProfileController : public BaseController
{
    Q_OBJECT
public:
    ProfileController(ProfilesRepository& profileRepositoryRef, QObject* parent = nullptr);
    void run() override;
signals:
    void profileSelected();
    void logout();
public slots:

    void refreshProfilesForCurrentUser() const;
private:
    QPointer<ProfileDialog> profileDialog;

    ProfilesRepository& profileRepository;

    void handleProfileSelection(int profileId);

    void handleAddProfileRequest(const QString& name) const;

    void handleRemoveProfileRequest(int profileId) const;

    void handleEditProfileRequest(int profileId, const QString& newName) const;

    void handleLogoutRequest();

    void handleExportDataRequest() const;
};

