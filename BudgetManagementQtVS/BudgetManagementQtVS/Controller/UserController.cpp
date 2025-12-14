#include "Controller/UserController.h"
#include <QDebug>
UserController::UserController(LoginDialog& loginDialogRef, UserRepository& userRepositoryRef, QObject* parent) : BaseController(parent), loginDialog(loginDialogRef), userRepository(userRepositoryRef)
{
    connect(&loginDialog, &LoginDialog::loginRequested,
        this, &UserController::handleLoginRequest);
    connect(&loginDialog, &LoginDialog::registerRequested,
        this, &UserController::handleRegisterRequest);
}
void UserController::run()
{
    loginDialog.show();
}
/**
 * @brief Handles user login attempt
 * @param username User's login name
 * @param password User's password
 */
void UserController::handleLoginRequest(const QString& username, const QString& password)
{
    if (username.trimmed().isEmpty() || password.isEmpty()) {
        const QString header = tr("Log in");
        const QString message = tr("Insert username and password.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }

    int userId = userRepository.getUserIdBasedOnUsername(username, password);
    if (userId < 0) {
        const QString header = tr("Log in");
        const QString message = tr("Invalid username or password.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }
    setUserId(userId);
    loginDialog.hide();
    emit userLoggedIn();
}

/**
 * @brief Handles new user registration
 * @param username Desired username
 * @param password Desired password
 */
void UserController::handleRegisterRequest(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        loginDialog.showLoginMessage("Register", "Username and password cannot be empty.", "error");
        return;
    }

    if (userRepository.checkIfUserExists(username)) {
        loginDialog.showLoginMessage("Register", "User with this name already exists.", "error");
        return;
    }


    if (userRepository.addUser(username, password)) {
        loginDialog.showLoginMessage("Register", "User created successfully!", "info");
    }
    else {
        loginDialog.showLoginMessage("Register", "Failed to create user in database.", "error");
    }
}
