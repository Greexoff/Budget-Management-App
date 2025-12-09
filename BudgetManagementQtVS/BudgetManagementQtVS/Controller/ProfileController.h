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
public slots:
    /**
    brief Displays profiles associated with the current user
    */
    void showProfilesForCurrentUser();
private:
    ProfileDialog& profileDialog;

    ProfilesRepository& profileRepository;

    /**
     * @brief Handles profile selection
     * @param profileId ID of the selected profile
     */
    void handleProfileSelection(int profileId);

    /**
     * @brief Handles request to add a new profile
     * @param name Name for the new profile
     */
    void handleAddProfileRequest(const QString& name);

    /**
     * @brief Handles request to remove a profile
     * @param profileId ID of the profile to delete
     */
    void handleRemoveProfileRequest(int profileId);

};
