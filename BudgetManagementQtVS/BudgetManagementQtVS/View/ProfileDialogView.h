#pragma once
#include <QDialog>
#include "Model/User&Profiles.h"

namespace Ui { class ProfileDialog; }

class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(QWidget* parent = nullptr);
    ~ProfileDialog();

    void setProfiles(const QVector<Profile>& profiles);

signals:
    void profileSelected(int profileId);
    void addProfileRequested(const QString& profileName);
    void removeProfileRequested(int profileId);

private slots:
    void onButtonSelectClicked();
    void onButtonAddClicked();
    void onButtonRemoveClicked();
    void onButtonCancelClicked();

private:
    Ui::ProfileDialog* ui;
    QVector<Profile> profilesId;

    void setupConnections();
    
};
