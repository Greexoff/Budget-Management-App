/**
 * @file LoginDialogView.h
 * @brief Header file for the Login Dialog.
 */
#pragma once
#include <QDialog>
#include <QMessageBox>

namespace Ui { class LoginDialog; }

/**
 * @class LoginDialog
 * @brief Handles user authentication and registration inputs.
 */
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    /** @brief Constructs the LoginDialog. */
    explicit LoginDialog(QWidget* parent = nullptr);
    /** @brief Destructor. */
    ~LoginDialog();

signals:
    /** @brief Emitted when Login button is clicked. */
    void loginRequested(const QString& username, const QString& password);
    /** @brief Emitted when Register button is clicked. */
    void registerRequested(const QString& username, const QString& password);
public slots:
    /** @brief Shows success or error messages regarding auth status. */
    void showLoginMessage(QString header, QString message, QString messageType);

private slots:
    void onButtonLoginClicked();
    void onButtonRegisterClicked();
    void onButtonCancelClicked();

private:
    Ui::LoginDialog* ui;     

    void setupConnections();
    void setupStyle();
};
