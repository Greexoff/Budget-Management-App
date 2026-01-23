#pragma once
#include <QObject>
#include "Model/Repositories/ProfileRepository.h"
#include "View/ProfileDialogView.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

class DataController : public QObject
{
    Q_OBJECT
public:
    explicit DataController(ProfilesRepository& profileRepo, QObject* parent = nullptr);

    void exportData(int userId, ProfileDialog* dialog);

    void autoSaveData(int userId);

private:
    ProfilesRepository& profileRepository;

    bool writeCsvToFile(int userId, const QString& filePath, QString& errorMessage);
};