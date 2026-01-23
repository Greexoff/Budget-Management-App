/**
 * @file AppController.h
 * @brief Header file for the main Application Controller.
 */
#pragma once

#include <QObject>
#include <QPointer>

#include "Controller/BaseController.h"
#include "Controller/UserController.h"
#include "Controller/ProfileController.h"
#include "Controller/DashboardController.h"
#include "Controller/DataController.h"
#include "Model/Repositories/UserRepository.h"
#include "Model/Repositories/ProfileRepository.h"

 /**
  * @class AppController
  * @brief The main entry point controller that manages the application lifecycle and navigation between major states.
  */
class AppController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the AppController.
     * @param parent The parent QObject.
     */
    explicit AppController(QObject* parent = nullptr);

    /** @brief Destructor. Cleans up current controller. */
    ~AppController() override;

    /**
     * @brief Starts the application.
     */
    void start();

private slots:
    /** @brief Switches context to the Profile Selection screen. */
    void showProfileSelection();

    /** @brief Switches context to the Main Dashboard. */
    void showDashboard();

    /** @brief Handles user logout request, resetting session and returning to start. */
    void handleLogout();

    /** @brief Handles switching profiles within the same user session. */
    void handleSwitchingProfile();

    /** @brief Slot triggered when the application is about to close (e.g., for auto-save). */
    void onAppAboutToQuit();
private:
    UserRepository userRepo;
    ProfilesRepository profileRepo;
    TransactionRepository transactionRepo;
    CategoryRepository categoryRepo;
    FinancialAccountRepository fAccRepo;

    QPointer<BaseController> currentController; ///< Points to the currently active sub-controller.
    DataController* dataController;             ///< Helper controller for data persistence operations.
};