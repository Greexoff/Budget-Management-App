/**
 * @file FinancialAccountSelectionView.cpp
 * @brief Implementation of the Financial Account Selection View.
 */
#include "View/FinancialAccountSelectionView.h"
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

 /** @brief Constructor. Initializes UI and Style. */
FinancialAccountSelectionView::FinancialAccountSelectionView(QWidget* parent)
    : QWidget(parent), tableModel(new QStandardItemModel(this))
{
    setupUI();
    setupStyle();
}
/** @brief Builds the layout, table, buttons and search bar. */
void FinancialAccountSelectionView::setupUI()
{
    QVBoxLayout* contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* viewLabel = new QLabel("Financial Accounts");
    viewLabel->setObjectName("viewLabel"); 

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search accounts...");
    searchEdit->setFixedWidth(300);

    headerLayout->addWidget(viewLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchEdit);

    QHBoxLayout* actionLayout = new QHBoxLayout();
    btnAdd = new QPushButton("+ Add Account");
    btnAdd->setObjectName("actionButtonAdd");

    btnEdit = new QPushButton("Edit");
    btnEdit->setObjectName("actionButton");

    btnDelete = new QPushButton("Delete");
    btnDelete->setObjectName("actionButtonDelete");

    actionLayout->addWidget(btnAdd);
    actionLayout->addWidget(btnEdit);
    actionLayout->addWidget(btnDelete);
    actionLayout->addStretch();

    accountTable = new QTableView();
    accountTable->setModel(tableModel);
    accountTable->setAlternatingRowColors(true);
    accountTable->setShowGrid(false);
    accountTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    accountTable->setSelectionMode(QAbstractItemView::SingleSelection);
    accountTable->verticalHeader()->setVisible(false);
    accountTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableModel->setColumnCount(5);
    tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Account Name"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Account Type"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Inital Balance"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("Current Balance"));

    accountTable->setColumnHidden(0, true);
    accountTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    contentLayout->addLayout(headerLayout);
    contentLayout->addLayout(actionLayout);
    contentLayout->addWidget(accountTable);

    connect(btnAdd, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonAddClicked);
    connect(btnEdit, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonEditClicked);
    connect(btnDelete, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonDeleteClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &FinancialAccountSelectionView::onSearchTextChanged);
    connect(accountTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &FinancialAccountSelectionView::onHeaderClicked);
}
/** @brief Rebuilds the table model with new account data. */
void FinancialAccountSelectionView::setAccountTabHeaders(const QVector<QStringList>& rows) const
{
    tableModel->removeRows(0, tableModel->rowCount());
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row)
            items.append(new QStandardItem(value));
        tableModel->appendRow(items);
    }
}
/** @brief Helper to get the ID from the hidden first column of the selected row. */
int FinancialAccountSelectionView::getSelectedAccountId() const
{
    QModelIndex index = accountTable->currentIndex();
    if (!index.isValid()) return -1;
    bool ok;
    int id = tableModel->data(tableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}
/** @brief Display helper for QMessageBox. */
void FinancialAccountSelectionView::showMessage(QString header, QString message, QString messageType)
{
    if (messageType == "error") QMessageBox::warning(this, header, message);
    else QMessageBox::information(this, header, message);
}
/** @brief Returns search edit text. */
QString FinancialAccountSelectionView::getSearchText() const { return searchEdit->text(); }

/**
 * @brief Creates a local QDialog to collect new account details and emits addAccountRequest.
 */
void FinancialAccountSelectionView::onButtonAddClicked()
{

    QDialog dlg(this);
    dlg.setWindowTitle("New Financial Account");
    dlg.setMinimumWidth(300);

    QFormLayout* form = new QFormLayout(&dlg);

    QLineEdit* nameEdit = new QLineEdit(&dlg);
    nameEdit->setPlaceholderText("e.g. mBank, Wallet");

    QComboBox* typeCombo = new QComboBox(&dlg);
    typeCombo->addItems({ "Cash", "Bank Account", "Savings", "Credit Card" });

    QDoubleSpinBox* balanceSpin = new QDoubleSpinBox(&dlg);
    balanceSpin->setRange(-1000000, 1000000);
    balanceSpin->setSuffix(" PLN");

    form->addRow("Name:", nameEdit);
    form->addRow("Type:", typeCombo);
    form->addRow("Balance:", balanceSpin);

    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btns);

    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString name = nameEdit->text();
        if (!name.trimmed().isEmpty()) {

            emit addAccountRequest(name, typeCombo->currentText(), balanceSpin->value());
        }
        else {
            showMessage("Warning", "Account name cannot be empty.", "error");
        }
    }
}
/**
 * @brief Creates a local QDialog populated with current data to edit an account.
 * Emits editAccountRequest upon acceptance.
 */
void FinancialAccountSelectionView::onButtonEditClicked()
{


    int row = accountTable->currentIndex().row();
    if (row < 0) {
        showMessage("Warning", "Select an account to edit.", "error");
        return;
    }


    int id = tableModel->data(tableModel->index(row, 0)).toInt(); 
    QString currentName = tableModel->data(tableModel->index(row, 1)).toString(); 



    QDialog dlg(this);
    dlg.setWindowTitle("Edit Financial Account");
    dlg.setMinimumWidth(300);
    QFormLayout* form = new QFormLayout(&dlg);

    QLineEdit* nameEdit = new QLineEdit(&dlg);
    nameEdit->setText(currentName);

    QComboBox* typeCombo = new QComboBox(&dlg);
    typeCombo->addItems({ "Cash", "Bank Account", "Savings", "Credit Card" });


    QDoubleSpinBox* balanceSpin = new QDoubleSpinBox(&dlg);
    balanceSpin->setRange(-1000000, 1000000);
    balanceSpin->setSuffix(" PLN");


    form->addRow("Name:", nameEdit);
    form->addRow("Type:", typeCombo);
    form->addRow("Balance:", balanceSpin);

    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btns);

    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString name = nameEdit->text();
        if (!name.trimmed().isEmpty()) {
            emit editAccountRequest(id, name, typeCombo->currentText(), balanceSpin->value());
        }
    }
}
/** @brief Sets CSS styling. */
void FinancialAccountSelectionView::setupStyle()
{
    this->setStyleSheet(
        "QLabel#viewLabel { font-size: 24px; font-weight: bold; color: white; }"
        "QLineEdit{ padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white; selection-background-color: #3498db; }"
        "QPushButton#actionButtonAdd { background-color: #27ae60; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton#actionButtonDelete { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QPushButton#actionButton { background-color: #2980b9; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QTableView { border: 1px solid #333333; background-color: #1e1e1e; color: white; gridline-color: #333333; selection-background-color: #3498db; selection-color: white; }"
        "QHeaderView::section { background-color: #2d2d2d; padding: 8px; border: 1px solid #333333; font-weight: bold; color: white; }"
        "QDialog, QMessageBox, QInputDialog { background-color: #1e1e1e; color: white; font-size: 14px; }"
        "QDialog QPushButton { background-color: #333333; color: white; border: 1px solid #444444; border-radius: 4px; padding: 6px 15px; }");
}
/** @brief Emits deleteAccountRequest for the selected row. */
void FinancialAccountSelectionView::onButtonDeleteClicked() { emit deleteAccountRequest(); }
/** @brief Emits searchAccountRequest. */
void FinancialAccountSelectionView::onSearchTextChanged(const QString& text) { emit searchAccountRequest(text); }
/** @brief Emits columnSortRequest. */
void FinancialAccountSelectionView::onHeaderClicked(int index) { emit columnSortRequest(index); }