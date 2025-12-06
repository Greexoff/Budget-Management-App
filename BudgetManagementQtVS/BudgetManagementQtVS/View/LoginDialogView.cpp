#include "LoginDialogView.h"
#include "ui_LoginDialogView.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->editPassword->setEchoMode(QLineEdit::Password);
    setupConnections();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setupConnections()
{

    connect(ui->buttonLogin, &QPushButton::clicked,this, &LoginDialog::onButtonLoginClicked);
    connect(ui->buttonRegister, &QPushButton::clicked,this, &LoginDialog::onButtonRegisterClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &LoginDialog::onButtonCancelClicked);
}

void LoginDialog::onButtonLoginClicked()
{
    emit loginRequested(ui->editUsername->text(), ui->editPassword->text());
}

void LoginDialog::onButtonRegisterClicked()
{
    emit registerRequested(ui->editUsername->text(),ui->editPassword->text());
}

void LoginDialog::onButtonCancelClicked()
{
    reject();
}
