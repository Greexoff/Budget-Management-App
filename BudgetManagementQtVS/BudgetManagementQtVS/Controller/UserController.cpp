#include "Controller/UserController.h"
#include <QDebug>
UserController::UserController(UserRepository& userRepositoryRef, QObject* parent) : BaseController(parent), userRepository(userRepositoryRef)
{
    loginDialog = new LoginDialog();

    connect(loginDialog, &LoginDialog::loginRequested,
        this, &UserController::handleLoginRequest);
    connect(loginDialog, &LoginDialog::registerRequested,
        this, &UserController::handleRegisterRequest);
}


//----------------Setting up view-------------------------------------------------


void UserController::run()
{
    if (loginDialog)
    {
        loginDialog->show();
    }
}


//----------------Handling actions performed on categories-----------------------

//Method responsible for checking if data user inserted exists in database and is correct
void UserController::handleLoginRequest(const QString& username, const QString& password)
{
    if (!loginDialog)
    {
	    return;
    }
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        const QString header = tr("Log in");
        const QString message = tr("Insert username and password.");
        loginDialog->showLoginMessage(header, message, "error");
        return;
    }

    int userId = userRepository.getUserIdBasedOnUsername(username, password);
    if (userId < 0) {
        const QString header = tr("Log in");
        const QString message = tr("Invalid username or password.");
        loginDialog->showLoginMessage(header, message, "error");
        return;
    }
    setUserId(userId);
    loginDialog->hide();
    emit userLoggedIn();
}

//Method responsible for register request and adding correct data to database
void UserController::handleRegisterRequest(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        loginDialog->showLoginMessage("Register", "Username and password cannot be empty.", "error");
        return;
    }

    if (userRepository.checkIfUserExists(username)) {
        loginDialog->showLoginMessage("Register", "User with this name already exists.", "error");
        return;
    }


    if (userRepository.addUser(username, password)) {
        loginDialog->showLoginMessage("Register", "User created successfully!", "info");
    }
    else {
        loginDialog->showLoginMessage("Register", "Failed to create user in database.", "error");
    }
}
