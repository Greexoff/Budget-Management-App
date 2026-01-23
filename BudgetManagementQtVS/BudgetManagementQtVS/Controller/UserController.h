/**
 * @file UserController.h
 * @brief Header file for the User Controller (Login/Register).
 */
#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/UserRepository.h"

#include "View/LoginDialogView.h"

#include <QPointer>

 /**
  * @class UserController
  * @brief Manages User Authentication (Login and Registration).
  */
class UserController : public BaseController
{
    Q_OBJECT
public:
	UserController(UserRepository& userRepositoryRef, QObject* parent = nullptr);
    void run() override;
signals:
    /** @brief Emitted upon successful login. */
    void userLoggedIn();
private:
	QPointer<LoginDialog> loginDialog;                  
	UserRepository& userRepository;  
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
    void handleRegisterRequest(const QString& username, const QString& password) const;
};