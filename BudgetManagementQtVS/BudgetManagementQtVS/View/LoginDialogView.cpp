/**
 * @file LoginDialogView.cpp
 * @brief Implementation of the Login Dialog.
 */
#include "LoginDialogView.h"
#include "ui_LoginDialogView.h"
#include <QApplication>

 /** @brief Constructor. Sets up UI, passwords mode, and connections. */
LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setupStyle();

    ui->editPassword->setEchoMode(QLineEdit::Password);
	ui->editPassword->setPlaceholderText("Password");
    ui->editUsername->setPlaceholderText("Username");
	ui->appNameLabel->setText("Budget Management App");
    setWindowTitle("Log in");
    setupConnections();
}
/** @brief Destructor cleaning up UI pointer. */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/** @brief Connects UI buttons to slots. */
void LoginDialog::setupConnections()
{
    connect(ui->buttonLogin, &QPushButton::clicked,this, &LoginDialog::onButtonLoginClicked);
    connect(ui->buttonRegister, &QPushButton::clicked,this, &LoginDialog::onButtonRegisterClicked);
    connect(ui->buttonExit, &QPushButton::clicked, this, &LoginDialog::onButtonCancelClicked);
}

/** @brief Emits loginRequested with entered credentials. */
void LoginDialog::onButtonLoginClicked()
{
    emit loginRequested(ui->editUsername->text(), ui->editPassword->text());
}

/** @brief Emits registerRequested with entered credentials. */
void LoginDialog::onButtonRegisterClicked()
{
    emit registerRequested(ui->editUsername->text(),ui->editPassword->text());
}

/** @brief Quits the application. */
void LoginDialog::onButtonCancelClicked()
{
    QApplication::quit();
}

/** @brief Displays information or warnings using QMessageBox. */
void LoginDialog::showLoginMessage(QString header, QString message, QString messageType)
{
    if (messageType == "error")
    {
      QMessageBox::warning(this, header, message);
    }
    else
    {
        QMessageBox::information(this, header, message);
    }
}
/** @brief Applies CSS styling. */
void LoginDialog::setupStyle() {
    this->setStyleSheet(
        "QDialog { color: white; background-color: #121212; width: 600px; height: 600px;}"
		"QFrame { background-color: #1e1e1e; border-radius: 10px; border: 1px solid #333333; padding: 10px; color: white; }"
        "QPushButton { background-color: #4CAF50; border-radius: 5px; padding: 8px 15px; font-weight: bold; color: white; }"
        "QPushButton#buttonExit { background-color: #c0392b; }"
        "QPushButton#buttonRegister { background-color: #2980b9; }"
        "QLineEdit {padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white;}"
        "QLabel { font-weight: bold; font-size: 20px; border: 0;}"
    );
}
