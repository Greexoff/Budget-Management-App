#include "ProfileDialogView.h"
#include "ui_ProfileDialogView.h"
#include <QInputDialog>
#include <QMessageBox>

ProfileDialog::ProfileDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);

    setupConnections();
}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

void ProfileDialog::setupConnections()
{

    connect(ui->buttonSelect, &QPushButton::clicked, this, &ProfileDialog::onButtonSelectClicked);

    connect(ui->buttonAdd, &QPushButton::clicked, this, &ProfileDialog::onButtonAddClicked);

    connect(ui->buttonRemove, &QPushButton::clicked, this, &ProfileDialog::onButtonRemoveClicked);

    connect(ui->buttonCancel, &QPushButton::clicked, this, &ProfileDialog::onButtonCancelClicked);
}

void ProfileDialog::setProfiles(const QVector<Profile>& profiles)
{
    profilesId = profiles;
    ui->listProfiles->clear();
    for (const auto& p : profilesId) {
        ui->listProfiles->addItem(p.getProfileName());
    }
}

void ProfileDialog::onButtonSelectClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit profileSelected(profilesId[row].getProfileId());
}

void ProfileDialog::onButtonAddClicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(
        this, tr("Nowy profil"), tr("Nazwa profilu:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty())
        return;

    emit addProfileRequested(name);
}

void ProfileDialog::onButtonRemoveClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit removeProfileRequested(profilesId[row].getProfileId());
}

void ProfileDialog::onButtonCancelClicked()
{
    reject();
}
