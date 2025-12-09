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
    if (!userRepository.addUser(username, password)) {
        const QString header = tr("Registration");
        const QString message = tr("Failed to create user.");
        loginDialog.showLoginMessage(header, message, "error");
        return;
    }
    const QString header = tr("Registration");
    const QString message = tr("Account created successfully.");
    loginDialog.showLoginMessage(header, message, "message");
}
