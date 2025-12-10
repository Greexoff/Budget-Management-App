#include "Controller/ProfileController.h"
#include <QApplication>

ProfileController::ProfileController(ProfileDialog& profileDialogRef, ProfilesRepository& profileRepositoryRef, QObject* parent) : BaseController(parent), profileDialog(profileDialogRef), profileRepository(profileRepositoryRef)
{
    // Connect profile dialog signals
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
/**
 * @brief Displays profiles associated with the current user
 */
void ProfileController::showProfilesForCurrentUser()
{
    QVector<Profile> profiles = profileRepository.getProfilesByUserId(getUserId());
    profileDialog.setProfiles(profiles);
    if (profileDialog.exec() == QDialog::Rejected) {
        QApplication::quit();
    }
}

/**
 * @brief Handles selection of a profile
 *
 * Sets the current profile and initializes the main window
 * with the profile's transaction data.
 *
 * @param profileId ID of the selected profile
 */
void ProfileController::handleProfileSelection(int profileId)
{
    setProfileId(profileId);
    profileDialog.accept();

    emit profileSelected();
}

/**
 * @brief Handles creation of a new profile
 * @param name Name for the new profile
 */
void ProfileController::handleAddProfileRequest(const QString& name)
{
    if (!profileRepository.addProfile(getUserId(), name)) {
        const QString header = tr("New profile");
        const QString message = tr("Failed to add a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    showProfilesForCurrentUser();
}

/**
 * @brief Handles deletion of a profile
 * @param profileId ID of the profile to delete
 */
void ProfileController::handleRemoveProfileRequest(int profileId)
{
    if (!profileRepository.removeProfileById(profileId)) {
        const QString header = tr("Delete profile");
        const QString message = tr("Failed to delete a profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    showProfilesForCurrentUser();
}

void ProfileController::handleEditProfileRequest(int profileId, const QString& newName)
{
    if (!profileRepository.updateProfile(profileId, newName)) {
        const QString header = tr("Edit Profile");
        const QString message = tr("Failed to update profile.");
        profileDialog.showProfileMessage(header, message, "error");
        return;
    }
    showProfilesForCurrentUser();
}

void ProfileController::handleLogoutRequest()
{
    setUserId(-1);
    setProfileId(-1);

    profileDialog.hide();

    emit logout();
}