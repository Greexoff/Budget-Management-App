#pragma once
#include <QDialog>

namespace Ui { class LoginDialog; }

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

signals:
    void loginRequested(const QString& username, const QString& password);
    void registerRequested(const QString& username, const QString& password);

private slots:
    void onButtonLoginClicked();
    void onButtonRegisterClicked();
    void onButtonCancelClicked();

private:
    Ui::LoginDialog* ui;

    void setupConnections();
};
