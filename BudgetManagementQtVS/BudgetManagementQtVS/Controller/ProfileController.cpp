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
    if (!profileDialog)
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(
        profileDialog,
        tr("Export Data"),
        "",
        tr("CSV Files (*.csv);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        profileDialog->showProfileMessage(tr("Export Error"), tr("Could not open file for writing"), "error");
        return;
    }

    QTextStream out(&file);

    out << "Profile,Transaction ID,Name,Date,Description,Amount,Type,Category,Account,Account Type\n";

    TransactionRepository transRepo;
    CategoryRepository catRepo;
    FinancialAccountRepository accountRepo;

    int userId = getUserId();
    QVector<Profile> profiles = profileRepository.getProfilesByUserId(userId);

    auto escape = [](QString s) {
        if (s.contains(',') || s.contains('"') || s.contains('\n')) {
            s.replace("\"", "\"\"");
            return "\"" + s + "\"";
        }
        return s;
    };

    for (const auto& profile : profiles) {
        int profileId = profile.getProfileId();
        QString profileName = profile.getProfileName();

        QVector<Transaction> transactions = transRepo.getAllProfileTransaction(profileId);
        QVector<FinancialAccount> accounts = accountRepo.getAllProfileFinancialAccounts(profileId);

        for (const auto& trans : transactions) {
            QString categoryName = catRepo.getCategoryNameById(trans.getCategoryId());
            
            QString accountName = "Unknown";
            QString accountType = "Unknown";
            
            for(const auto& acc : accounts) {
                if(acc.getFinancialAccountId() == trans.getFinancialAccountId()) {
                    accountName = acc.getFinancialAccountName();
                    accountType = acc.getFinancialAccountType();
                    break;
                }
            }

            out << escape(profileName) << ","
                << trans.getTransactionId() << ","
                << escape(trans.getTransactionName()) << ","
                << trans.getTransactionDate().toString("yyyy-MM-dd") << ","
                << escape(trans.getTransactionDescription()) << ","
                << trans.getTransactionAmount() << ","
                << escape(trans.getTransactionType()) << ","
                << escape(categoryName) << ","
                << escape(accountName) << ","
                << escape(accountType) << "\n";
        }
    }

    file.close();
    profileDialog->showProfileMessage(tr("Export Success"), tr("Data exported successfully to ") + fileName, "info");
}
