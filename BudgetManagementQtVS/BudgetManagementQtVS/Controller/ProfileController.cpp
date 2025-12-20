#include "Controller/ProfileController.h"
#include <QApplication>

ProfileController::ProfileController(ProfileDialog& profileDialogRef, ProfilesRepository& profileRepositoryRef, QObject* parent) : BaseController(parent), profileDialog(profileDialogRef), profileRepository(profileRepositoryRef)
{
    connect(&profileDialog, &ProfileDialog::profileSelected,
        this, &ProfileController::handleProfileSelection);
    connect(&profileDialog, &ProfileDialog::addProfileRequested,
        this, &ProfileController::handleAddProfileRequest);
    connect(&profileDialog, &ProfileDialog::removeProfileRequested,
        this, &ProfileController::handleRemoveProfileRequest);
    connect(&profileDialog, &ProfileDialog::editProfileRequested,
        this, &ProfileController::handleEditProfileRequest);
    connect(&profileDialog, &ProfileDialog::logoutRequested,
        this, &ProfileController::handleLogoutRequest);
}

//TODO: ADD FILTERING AND SORTING OF PROFILES


//----------------------Setting up view---------------------------------------

//Method responsible for refreshing/setting up profiles in window, used every time a change in list occurs
void ProfileController::refreshProfilesForCurrentUser()
{
    QVector<Profile> profiles = profileRepository.getProfilesByUserId(getUserId());
    profileDialog.setProfiles(profiles);
    if (profileDialog.exec() == QDialog::Rejected) {
        QApplication::quit();
    }
}


//----------------Handling actions performed on profiles----------------------

//Method responsible for adding profile based on entered data
void ProfileController::handleAddProfileRequest(const QString& name)
{
    if (!profileRepository.addProfile(getUserId(), name)) {
        const QString header = tr("New profile");
        const QString message = tr("Failed to add a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

//Method responsible for handling editing of profile
void ProfileController::handleEditProfileRequest(int profileId, const QString& newName)
{
    if (!profileRepository.updateProfile(profileId, newName)) {
        const QString header = tr("Edit Profile");
        const QString message = tr("Failed to update profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

//Method responsible for handling deletion of profile
void ProfileController::handleRemoveProfileRequest(int profileId)
{
    if (!profileRepository.removeProfileById(profileId)) {
        const QString header = tr("Delete profile");
        const QString message = tr("Failed to delete a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

//Method responsible for handling profile selection and transition to the transaction window
void ProfileController::handleProfileSelection(int profileId)
{
    setProfileId(profileId);
    profileDialog.accept();

    emit profileSelected();
}

//Method responsible for handling logging out of profile
void ProfileController::handleLogoutRequest()
{
    setUserId(-1);
    setProfileId(-1);

    profileDialog.accept();

    emit logout();
}