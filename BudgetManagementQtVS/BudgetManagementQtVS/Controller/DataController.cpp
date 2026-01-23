#include "Controller/DataController.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


DataController::DataController(ProfilesRepository& profileRepo, QObject* parent)
    : QObject(parent), profileRepository(profileRepo)
{
}

bool DataController::writeCsvToFile(int userId, const QString& filePath, QString& errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorMessage = "Could not open file for writing.";
        return false;
    }

    QTextStream out(&file);
    out << "Profile,Transaction ID,Name,Date,Description,Amount,Type,Category,Account,Account Type\n";

    TransactionRepository transRepo;
    CategoryRepository catRepo;
    FinancialAccountRepository accountRepo;

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

            for (const auto& acc : accounts) {
                if (acc.getFinancialAccountId() == trans.getFinancialAccountId()) {
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
    return true;
}

void DataController::exportData(int userId, ProfileDialog* dialog)
{
    if (!dialog) return;

    QString fileName = QFileDialog::getSaveFileName(dialog, tr("Export Data"), "", tr("CSV Files (*.csv);;All Files (*)"));
    if (fileName.isEmpty()) return;

    QString error;
    if (writeCsvToFile(userId, fileName, error)) {
        dialog->showProfileMessage(tr("Export Success"), tr("Data exported successfully."), "info");
    }
    else {
        dialog->showProfileMessage(tr("Export Error"), error, "error");
    }
}

void DataController::autoSaveData(int userId)
{

    QString fileName = QString("autosave_user_%1.csv").arg(userId);

    QString error;
    if (writeCsvToFile(userId, fileName, error)) {
        qDebug() << "Auto-save successful: " << fileName;
    }
    else {
        qDebug() << "Auto-save failed: " << error;
    }
}