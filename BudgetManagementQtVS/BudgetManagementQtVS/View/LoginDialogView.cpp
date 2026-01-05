#include "LoginDialogView.h"
#include "ui_LoginDialogView.h"
#include <QApplication>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->editPassword->setEchoMode(QLineEdit::Password);
    setWindowTitle("Log in");
    setupConnections();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


//----------------Setting connection (button-method)-------------------------------------------------


void LoginDialog::setupConnections()
{
    connect(ui->buttonLogin, &QPushButton::clicked,this, &LoginDialog::onButtonLoginClicked);
    connect(ui->buttonRegister, &QPushButton::clicked,this, &LoginDialog::onButtonRegisterClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &LoginDialog::onButtonCancelClicked);
}


//----------------Pressing buttons actions-------------------------------------------------


//Method that notices clicking on log in button
void LoginDialog::onButtonLoginClicked()
{
    emit loginRequested(ui->editUsername->text(), ui->editPassword->text());
}

//Method that notices clicking on register button
void LoginDialog::onButtonRegisterClicked()
{
    emit registerRequested(ui->editUsername->text(),ui->editPassword->text());
}

//Method that notices clicking on cancel button
void LoginDialog::onButtonCancelClicked()
{
    QApplication::quit();
}

//Method responsible for displaying error if inserted data is incorrect/is lacking 
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

