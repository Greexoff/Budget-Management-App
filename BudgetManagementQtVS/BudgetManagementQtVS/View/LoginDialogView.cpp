#include "LoginDialogView.h"
#include "ui_LoginDialogView.h"

/**
 * @brief Constructs the login dialog with password masking
 * @param parent Parent widget
 */
LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Configure password field to hide entered characters
    ui->editPassword->setEchoMode(QLineEdit::Password);
    setWindowTitle("Log in");
    setupConnections();
}

/**
 * @brief Destructor - cleans up UI resources
 */
LoginDialog::~LoginDialog()
{
    delete ui;
}

/**
 * @brief Connects UI buttons to their handler methods
 */
void LoginDialog::setupConnections()
{

    connect(ui->buttonLogin, &QPushButton::clicked,this, &LoginDialog::onButtonLoginClicked);
    connect(ui->buttonRegister, &QPushButton::clicked,this, &LoginDialog::onButtonRegisterClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &LoginDialog::onButtonCancelClicked);
}

/**
 * @brief Handles click on the Login button
 */
void LoginDialog::onButtonLoginClicked()
{
    emit loginRequested(ui->editUsername->text(), ui->editPassword->text());
}

/**
 * @brief Handles click on the Register button
 */
void LoginDialog::onButtonRegisterClicked()
{
    emit registerRequested(ui->editUsername->text(),ui->editPassword->text());
}

/**
 * @brief Handles click on the Cancel button
 */
void LoginDialog::onButtonCancelClicked()
{
    reject();
}
void LoginDialog::displayLoginMessage(QString header, QString message, QString messageType)
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

