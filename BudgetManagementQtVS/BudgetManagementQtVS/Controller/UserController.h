#pragma once
#include "Controller/BaseController.h"

#include "Model/Repositories/UserRepository.h"

#include "View/LoginDialogView.h"

#include <QPointer>

class UserController : public BaseController
{
    Q_OBJECT
public:
	UserController(UserRepository& userRepositoryRef, QObject* parent = nullptr);
    void run() override;
signals:
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
    void handleRegisterRequest(const QString& username, const QString& password);
};