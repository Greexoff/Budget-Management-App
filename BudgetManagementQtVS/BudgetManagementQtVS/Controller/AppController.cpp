/**
 * @file AppController.cpp
 * @brief Implementation of the main Application Controller.
 */
#include "Controller/AppController.h"

 /** @brief Constructor. Initializes the DataController and connects quit signals. */
AppController::AppController(QObject* parent) : QObject(parent) 
{
    dataController = new DataController(profileRepo, this);
    connect(qApp, &QCoreApplication::aboutToQuit, this, &AppController::onAppAboutToQuit);
}
/** @brief Destructor. Deletes the active sub-controller. */
AppController::~AppController()
{
	delete currentController;
}
/** @brief Initializes the Login flow using UserController. */
void AppController::start()
{
	delete currentController;


	UserController* userCtrl = new UserController(userRepo);
    currentController = userCtrl;

    connect(userCtrl, &UserController::userLoggedIn, this, &AppController::showProfileSelection);

    currentController->run();
}
/** @brief Transitions to the Profile Selection flow using ProfileController. */
void AppController::showProfileSelection()
{
	delete currentController;


	ProfileController* profileCtrl = new ProfileController(profileRepo);
    currentController = profileCtrl;

    connect(profileCtrl, &ProfileController::profileSelected, this, &AppController::showDashboard);
    connect(profileCtrl, &ProfileController::logout, this, &AppController::handleLogout);

    currentController->run();
}
/** @brief Resets user session IDs and restarts the app flow. */
void AppController::handleLogout()
{
    BaseController::setUserId(-1);
    BaseController::setProfileId(-1);
    start();
}
/** @brief Resets only the profile ID and returns to profile selection. */
void AppController::handleSwitchingProfile()
{
    BaseController::setProfileId(-1);
    delete currentController;

    showProfileSelection();
}
/** @brief Transitions to the main Dashboard flow using DashboardController. */
void AppController::showDashboard()
{
	delete currentController;

	DashboardController* dashCtrl = new DashboardController(profileRepo, transactionRepo, categoryRepo, fAccRepo);
    currentController = dashCtrl;

    connect(dashCtrl, &DashboardController::logoutRequested, this, &AppController::handleSwitchingProfile);

    currentController->run();
}
/** @brief Triggered on app exit. Auto-saves data if a user is logged in. */
void AppController::onAppAboutToQuit()
{
    int currentUserId = BaseController::getUserId();

    if (currentUserId != -1) {
        if (dataController) {
            dataController->autoSaveData(currentUserId);
        }
    }
}