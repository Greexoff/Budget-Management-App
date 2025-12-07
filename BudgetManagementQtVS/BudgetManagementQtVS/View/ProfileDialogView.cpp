#include "ProfileDialogView.h"
#include "ui_ProfileDialogView.h"
#include <QInputDialog>
#include <QMessageBox>

/**
 * @brief Constructs the profile selection dialog
 * @param parent Parent widget
 */
ProfileDialog::ProfileDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);
    setWindowTitle("Select profile");
    setupConnections();
}

/**
 * @brief Destructor - cleans up UI resources
 */
ProfileDialog::~ProfileDialog()
{
    delete ui;
}

/**
 * @brief Connects UI buttons to their handler methods
 */
void ProfileDialog::setupConnections()
{
    connect(ui->buttonSelect, &QPushButton::clicked, this, &ProfileDialog::onButtonSelectClicked);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &ProfileDialog::onButtonAddClicked);
    connect(ui->buttonRemove, &QPushButton::clicked, this, &ProfileDialog::onButtonRemoveClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &ProfileDialog::onButtonCancelClicked);
}

/**
 * @brief Loads profiles into the list widget
 * @param profiles List of Profile objects to display
 */
void ProfileDialog::setProfiles(const QVector<Profile>& profiles)
{
    profilesId = profiles;
    ui->listProfiles->clear();
    for (const auto& p : profilesId) {
        ui->listProfiles->addItem(p.getProfileName());
    }
}

/**
 * @brief Handles click on the Select button
 */
void ProfileDialog::onButtonSelectClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit profileSelected(profilesId[row].getProfileId());
}

/**
 * @brief Handles click on the Add button
 */
void ProfileDialog::onButtonAddClicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(
        this, tr("New profile"), tr("Profile name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty())
        return;

    emit addProfileRequested(name);
}

/**
 * @brief Handles click on the Remove button
 */
void ProfileDialog::onButtonRemoveClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit removeProfileRequested(profilesId[row].getProfileId());
}

/**
 * @brief Handles click on the Cancel button
 */
void ProfileDialog::onButtonCancelClicked()
{
    reject();
}
void ProfileDialog::showProfileMessage(QString header, QString message, QString messageType)
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
