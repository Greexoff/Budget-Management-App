#pragma once

#include "Model/Transaction.h"
#include "Model/Repositories.h"
#include "Model/User&Profiles.h"
#include "Model/Category.h"

#include "View/MainWindowView.h"
#include "View/LoginDialogView.h"
#include "View/ProfileDialogView.h"
#include "View/CategorySelectionView.h"

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

    void handleCategorySelected(int categoryId);
    void handleAddCategoryRequested(const QString& categoryName);
    void handleDeleteCategoryRequested(int categoryId);
    void showCategorySelectionDialog();
private:
    LoginDialog loginDialog;
    ProfileDialog profileDialog;
    MainWindow mainWindowView;
    CategorySelectionView categoryDialog;

    UserRepository userRepository;
    ProfilesRepository profilesRepository;
    TransactionRepository transactionRepository;
    CategoryRepository categoryRepository;

    int currentUserId = -1;
    int currentProfileId = -1;
    bool mainWindowInitialized = false;
    int selectedCategoryIdForTransaction = -1;

    void showProfilesForCurrentUser();
    void refreshTransactionsView();

    void openCategoryDialog(bool withSelectButton);
    int askUserForCategoryId();

};

 