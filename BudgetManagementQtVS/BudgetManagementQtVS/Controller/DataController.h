/**
 * @file DataController.h
 * @brief Header file for the Data Controller.
 */
#pragma once
#include <QObject>
#include "Model/Repositories/ProfileRepository.h"
#include "View/ProfileDialogView.h"

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>
#include <Model/Repositories/FinancialAccountRepository.h>

 /**
  * @class DataController
  * @brief Handles data export operations (e.g., CSV export).
  */
class DataController : public QObject
{
    Q_OBJECT
public:
    explicit DataController(ProfilesRepository& profileRepo, QObject* parent = nullptr);

    /**
     * @brief Initiates manual data export (Save File Dialog).
     * @param userId The user ID to export data for.
     * @param dialog Parent widget for the file dialog.
     */
    void exportData(int userId, ProfileDialog* dialog);

    /**
     * @brief Automatically saves data to a predefined file (e.g., on exit).
     * @param userId The user ID to save data for.
     */
    void autoSaveData(int userId);

private:
    ProfilesRepository& profileRepository;
    /**
     * @brief Internal helper to write database content to a CSV file.
     * @param userId Target user ID.
     * @param filePath Path to output file.
     * @param errorMessage Output parameter for error strings.
     * @return True if successful.
     */
    bool writeCsvToFile(int userId, const QString& filePath, QString& errorMessage);
};