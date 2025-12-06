#pragma once

#include "Model/Transaction.h"
#include "Model/Repositories.h"
#include "Model/User&Profiles.h"

#include "View/MainWindowView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"

#include <QObject>
#include <QVector>
#include <QStringList>
#include <QDate>



class MainWindow;


class TransactionController : public QObject
{
    Q_OBJECT

public:
    explicit TransactionController(QObject* parent = nullptr);
    
    void run();

private slots:

    void handleLoginRequested(const QString& username, const QString& password);
    void handleRegisterRequested(const QString& username, const QString& password);

    void handleProfileSelected(int profileId);
    void handleAddProfileRequested(const QString& name);
    void handleRemoveProfileRequested(int profileId);

    void handleAddTransactionRequested();
    void handleDeleteTransactionRequested();

private:
    LoginDialog   loginDialog;
    ProfileDialog profileDialog;
    MainWindow mainWindowView;

    UserRepository        userRepository;
    ProfilesRepository    profilesRepository;
    TransactionRepository transactionRepository;

    int currentUserId = -1;
    int currentProfileId = -1;
    bool mainWindowInitialized = false;

    void showProfilesForCurrentUser();
    void refreshTransactionsView();
};
