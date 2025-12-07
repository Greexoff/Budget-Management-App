#pragma once
#include <QDialog>
#include <QMessageBox>

namespace Ui { class LoginDialog; }

/**
 * @class LoginDialog
 * @brief Dialog for user authentication (login and registration)
 *
 * This dialog provides the entry point for user authentication,
 * offering both login for existing users and registration for new users.
 * It collects credentials and emits signals for the controller to handle.
 */
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the login dialog
     * @param parent Parent widget (optional)
     */
    explicit LoginDialog(QWidget* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~LoginDialog();

signals:
    /**
     * @brief Emitted when user attempts to log in
     * @param username Entered username
     * @param password Entered password
     */
    void loginRequested(const QString& username, const QString& password);

    /**
     * @brief Emitted when user attempts to register
     * @param username Desired username
     * @param password Desired password
     */
    void registerRequested(const QString& username, const QString& password);
public slots:
    void displayLoginMessage(QString header, QString message, QString messageType);

private slots:
    void onButtonLoginClicked();
    void onButtonRegisterClicked();
    void onButtonCancelClicked();

private:
    Ui::LoginDialog* ui;        ///< Pointer to UI elements

    /**
     * @brief Sets up connections for UI buttons
     */
    void setupConnections();
};
