#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/UserRepository.h"

#include "View/LoginDialogView.h"

class UserController : public BaseController
{
    Q_OBJECT
public:
	UserController(LoginDialog& loginDialogRef, UserRepository& userRepositoryRef, QObject* parent = nullptr);
    void run();
signals:
    void userLoggedIn();
private:
	LoginDialog& loginDialog;                   ///< Reference to the login dialog

	UserRepository& userRepository;             ///< Reference to the user repository
private slots:
    // Authentication handlers
    /**
    * @brief Handles user login request
    * @param username User's login name
    * @param password User's password
    */
    void handleLoginRequest(const QString& username, const QString& password);

    /**
     * @brief Handles new user registration request
     * @param username Desired username
     * @param password Desired password
     */
    void handleRegisterRequest(const QString& username, const QString& password);
};