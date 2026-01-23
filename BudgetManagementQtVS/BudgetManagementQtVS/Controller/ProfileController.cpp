#include "Controller/ProfileController.h"
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

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

//TODO: ADD FILTERING AND SORTING OF PROFILES

void ProfileController::run()
{
    refreshProfilesForCurrentUser();
}

//----------------------Setting up view---------------------------------------

//Method responsible for refreshing/setting up profiles in window, used every time a change in list occurs
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


//----------------Handling actions performed on profiles----------------------

//Method responsible for adding profile based on entered data
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

//Method responsible for handling editing of profile
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

//Method responsible for handling deletion of profile
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

//Method responsible for handling profile selection and transition to the transaction window
void ProfileController::handleProfileSelection(int profileId)
{
    setProfileId(profileId);
    if (profileDialog)
    {
        profileDialog->close();
    }
    emit profileSelected();
}

//Method responsible for handling logging out of profile
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

void ProfileController::handleExportDataRequest() const
{
    if (!profileDialog || !dataController)
    {
        return;
    }

    dataController->exportData(getUserId(), profileDialog);
}
