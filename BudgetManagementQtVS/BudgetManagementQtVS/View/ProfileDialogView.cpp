/**
 * @file ProfileDialogView.cpp
 * @brief Implementation of the Profile Selection Dialog.
 */
#include "ProfileDialogView.h"
#include "ui_ProfileDialogView.h"
#include <QInputDialog>
#include <QMessageBox>

 /** @brief Constructor. Initializes UI and connections. */
ProfileDialog::ProfileDialog(QWidget* parent)
    : QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    setupStyle();
    ui->setupUi(this);
    setWindowTitle("Select profile");
    setupConnections();
}

/** @brief Destructor. */
ProfileDialog::~ProfileDialog()
{
    delete ui;
}
/** @brief Connects buttons to internal slots. */
void ProfileDialog::setupConnections()
{
    connect(ui->buttonSelect, &QPushButton::clicked, this, &ProfileDialog::onButtonSelectClicked);
    connect(ui->buttonAdd, &QPushButton::clicked, this, &ProfileDialog::onButtonAddClicked);
    connect(ui->buttonRemove, &QPushButton::clicked, this, &ProfileDialog::onButtonRemoveClicked);
    connect(ui->buttonExit, &QPushButton::clicked, this, &ProfileDialog::onButtonCancelClicked);
    connect(ui->buttonEdit, &QPushButton::clicked, this, &ProfileDialog::onButtonEditClicked);
    connect(ui->buttonLogout, &QPushButton::clicked, this, &ProfileDialog::onButtonLogoutClicked);
    connect(ui->buttonExport, &QPushButton::clicked, this, &ProfileDialog::onButtonExportClicked);
}
/** @brief Clears and repopulates the profile list widget. */
void ProfileDialog::setProfiles(const QVector<Profile>& profiles)
{
    profilesId = profiles;
    ui->listProfiles->clear();
    for (const auto& p : profilesId) {
        ui->listProfiles->addItem(p.getProfileName());
    }
}
/** @brief Emits profileSelected based on current list row. */
void ProfileDialog::onButtonSelectClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit profileSelected(profilesId[row].getProfileId());
}
/** @brief Shows input dialog for name and emits addProfileRequested. */
void ProfileDialog::onButtonAddClicked()
{
    bool ok = false;
    QString name = QInputDialog::getText(
        this, tr("New profile"), tr("Profile name:"), QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty())
        return;

    emit addProfileRequested(name);
}
/** @brief Emits removeProfileRequested for selected row. */
void ProfileDialog::onButtonRemoveClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size())
        return;

    emit removeProfileRequested(profilesId[row].getProfileId());
}
/** @brief Closes the dialog. */
void ProfileDialog::onButtonCancelClicked()
{
    reject();
}
/** @brief Displays QMessageBox. */
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
/** @brief Shows input dialog and emits editProfileRequested. */
void ProfileDialog::onButtonEditClicked()
{
    int row = ui->listProfiles->currentRow();
    if (row < 0 || row >= profilesId.size()) return;

    int id = profilesId[row].getProfileId();
    QString currentName = profilesId[row].getProfileName();

    bool ok = false;
    QString newName = QInputDialog::getText(this, tr("Edit Profile"), tr("New Name:"),
        QLineEdit::Normal, currentName, &ok);

    if (ok && !newName.trimmed().isEmpty()) {
        emit editProfileRequested(id, newName);
    }
}
/** @brief Emits logoutRequested. */
void ProfileDialog::onButtonLogoutClicked()
{
    emit logoutRequested();
}
/** @brief Emits exportDataRequested. */
void ProfileDialog::onButtonExportClicked()
{
    emit exportDataRequested();
}
/** @brief Sets CSS styling. */
void ProfileDialog::setupStyle()
{
    this->setStyleSheet(
        "QDialog { color: white; background-color: #121212; width: 600px; height: 600px;}"
		"QVBoxLayout#MainLayout { background-color: #121212; padding: 15px; }"
		"QPushButton { background-color: #4CAF50; color: white; border-radius: 5px; padding: 10px; font-weight: bold; }"
		"QPushButton#buttonRemove { background-color: #c0392b; }"
        "QPushButton#buttonEdit { background-color: #2980b9; }"
        "QPushButton#buttonLogout { background-color: #2980b9; }"
        "QPushButton#buttonExport { background-color: #2980b9; }"
        "QPushButton#buttonExit { background-color: #c0392b; }" 
        "QListWidget { background-color: #2d2d2d; font-weight: bold; padding: 10px; border: 1px solid #444444; }"
		"QListWidget::item { height: 20px; color: white; font-weight: bold; padding: 10px; }"
    );
}