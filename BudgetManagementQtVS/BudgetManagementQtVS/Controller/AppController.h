#pragma once

#include <QObject>
#include <QPointer>

#include "Controller/BaseController.h"
#include "Controller/UserController.h"
#include "Controller/ProfileController.h"
#include "Controller/DashboardController.h"
#include "Model/Repositories/UserRepository.h"
#include "Model/Repositories/ProfileRepository.h"

class AppController : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject* parent = nullptr);
	~AppController() override;

    void start();

private slots:
    void showProfileSelection();

    void showDashboard();

    void handleLogout();

    void handleSwitchingProfile();
private:
    UserRepository userRepo;
    ProfilesRepository profileRepo;
    TransactionRepository transactionRepo;
    CategoryRepository categoryRepo;
    FinancialAccountRepository fAccRepo;

    QPointer<BaseController> currentController;
};