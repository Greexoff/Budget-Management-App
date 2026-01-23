/**
 * @file ProfileController.cpp
 * @brief Implementation of the Profile Controller.
 */
#include "Controller/ProfileController.h"
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

 /** @brief Constructor. Creates view, DataController, and connections. */
ProfileController::ProfileController(ProfilesRepository& profileRepositoryRef, QObject* parent) : BaseController(parent), profileRepository(profileRepositoryRef)
{
    dataController = new DataController(profileRepository, this);
    profileDialog = new ProfileDialog();
    if (profileDialog)
    {
        profileDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
    connect(profileDialog, &ProfileDialog::profileSelected,
        this, &ProfileController::handleProfileSelection);
    connect(profileDialog, &ProfileDialog::addProfileRequested,
        this, &ProfileController::handleAddProfileRequest);
    connect(profileDialog, &ProfileDialog::removeProfileRequested,
        this, &ProfileController::handleRemoveProfileRequest);
    connect(profileDialog, &ProfileDialog::editProfileRequested,
        this, &ProfileController::handleEditProfileRequest);
    connect(profileDialog, &ProfileDialog::logoutRequested,
        this, &ProfileController::handleLogoutRequest);
    connect(profileDialog, &ProfileDialog::exportDataRequested,
        this, &ProfileController::handleExportDataRequest);
}

/** @brief Runs the controller. */
void ProfileController::run()
{
    refreshProfilesForCurrentUser();
}

/** @brief Fetches profiles for current user ID and updates view. */
void ProfileController::refreshProfilesForCurrentUser() const
{
    if (!profileDialog)
    {
        return;
    }
    QVector<Profile> profiles = profileRepository.getProfilesByUserId(getUserId());
    profileDialog->setProfiles(profiles);
    profileDialog->show();
}

/** @brief Adds profile and refreshes list. */
void ProfileController::handleAddProfileRequest(const QString& name) const
{
    if (!profileDialog)
    {
        return;
    }
    if (!profileRepository.addProfile(getUserId(), name)) {
        const QString header = tr("New profile");
        const QString message = tr("Failed to add a profile.");
        profileDialog->showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

/** @brief Updates profile name. */
void ProfileController::handleEditProfileRequest(int profileId, const QString& newName) const
{
    if (!profileDialog)
    {
        return;
    }
    if (!profileRepository.updateProfile(profileId, newName)) {
        const QString header = tr("Edit Profile");
        const QString message = tr("Failed to update profile.");
        profileDialog->showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

/** @brief Removes profile. */
void ProfileController::handleRemoveProfileRequest(int profileId) const
{
    if (!profileDialog)
    {
        return;
    }
    if (!profileRepository.removeProfileById(profileId)) {
        const QString header = tr("Delete profile");
        const QString message = tr("Failed to delete a profile.");
        profileDialog->showProfileMessage(header, message, "error");
        return;
    }
    refreshProfilesForCurrentUser();
}

/** @brief Sets global Profile ID, closes dialog, emits success signal. */
void ProfileController::handleProfileSelection(int profileId)
{
    setProfileId(profileId);
    if (profileDialog)
    {
        profileDialog->close();
    }
    emit profileSelected();
}

/** @brief Resets IDs and emits logout signal. */
void ProfileController::handleLogoutRequest()
{
    setUserId(-1);
    setProfileId(-1);
    if (profileDialog)
    {
        profileDialog->close();
    }
    emit logout();
}

/** @brief Delegates export to DataController. */
void ProfileController::handleExportDataRequest() const
{
    if (!profileDialog || !dataController)
    {
        return;
    }

    dataController->exportData(getUserId(), profileDialog);
}
