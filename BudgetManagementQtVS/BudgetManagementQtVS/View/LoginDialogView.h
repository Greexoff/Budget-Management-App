#pragma once
#include <QDialog>
#include <QMessageBox>

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
public slots:
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
