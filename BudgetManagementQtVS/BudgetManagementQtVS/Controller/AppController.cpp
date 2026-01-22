#include "Controller/AppController.h"

AppController::AppController(QObject* parent) : QObject(parent) {}

AppController::~AppController()
{
	delete currentController;
}

void AppController::start()
{
	delete currentController;


	UserController* userCtrl = new UserController(userRepo);
    currentController = userCtrl;

    connect(userCtrl, &UserController::userLoggedIn, this, &AppController::showProfileSelection);

    currentController->run();
}

void AppController::showProfileSelection()
{
	delete currentController;


	ProfileController* profileCtrl = new ProfileController(profileRepo);
    currentController = profileCtrl;

    connect(profileCtrl, &ProfileController::profileSelected, this, &AppController::showDashboard);
    connect(profileCtrl, &ProfileController::logout, this, &AppController::handleLogout);

    currentController->run();
}

void AppController::handleLogout()
{
    BaseController::setUserId(-1);
    BaseController::setProfileId(-1);
    start();
}
void AppController::handleSwitchingProfile()
{
    BaseController::setProfileId(-1);
    delete currentController;

    showProfileSelection();
}
void AppController::showDashboard()
{
	delete currentController;


	DashboardController* dashCtrl = new DashboardController();
    currentController = dashCtrl;

    connect(dashCtrl, &DashboardController::logoutRequested, this, &AppController::handleSwitchingProfile);

    currentController->run();
}