#include "View/FinancialAccountSelectionView.h"
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

FinancialAccountSelectionView::FinancialAccountSelectionView(QWidget* parent)
    : QWidget(parent), tableModel(new QStandardItemModel(this))
{
    setupUI();
}

void FinancialAccountSelectionView::setupUI()
{
    QVBoxLayout* contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // 1. Nagłówek
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* viewLabel = new QLabel("Financial Accounts");
    viewLabel->setObjectName("viewLabel"); 

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search accounts...");
    searchEdit->setFixedWidth(300);

    headerLayout->addWidget(viewLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchEdit);

    // 2. Akcje
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

    // 3. Tabela
    accountTable = new QTableView();
    accountTable->setModel(tableModel);
    accountTable->setAlternatingRowColors(true);
    accountTable->setShowGrid(false);
    accountTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    accountTable->setSelectionMode(QAbstractItemView::SingleSelection);
    accountTable->verticalHeader()->setVisible(false);
    accountTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Inicjalizacja nagłówków
    tableModel->setColumnCount(3);
    tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Account Name"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Balance"));

    accountTable->setColumnHidden(0, true);
    accountTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    contentLayout->addLayout(headerLayout);
    contentLayout->addLayout(actionLayout);
    contentLayout->addWidget(accountTable);

    // Connecty
    connect(btnAdd, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonAddClicked);
    connect(btnEdit, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonEditClicked);
    connect(btnDelete, &QPushButton::clicked, this, &FinancialAccountSelectionView::onButtonDeleteClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &FinancialAccountSelectionView::onSearchTextChanged);
    connect(accountTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &FinancialAccountSelectionView::onHeaderClicked);
}

// --- Metody ---

void FinancialAccountSelectionView::setAccountTabHeaders(const QVector<QStringList>& rows)
{
    tableModel->removeRows(0, tableModel->rowCount());
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row)
            items.append(new QStandardItem(value));
        tableModel->appendRow(items);
    }
}

int FinancialAccountSelectionView::getSelectedAccountId() const
{
    QModelIndex index = accountTable->currentIndex();
    if (!index.isValid()) return -1;
    bool ok;
    int id = tableModel->data(tableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}

void FinancialAccountSelectionView::showMessage(QString header, QString message, QString messageType)
{
    if (messageType == "error") QMessageBox::warning(this, header, message);
    else QMessageBox::information(this, header, message);
}

QString FinancialAccountSelectionView::getSearchText() const { return searchEdit->text(); }

// Sloty
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
void FinancialAccountSelectionView::onButtonDeleteClicked() { emit deleteAccountRequest(); }
void FinancialAccountSelectionView::onSearchTextChanged(const QString& text) { emit searchAccountRequest(text); }
void FinancialAccountSelectionView::onHeaderClicked(int index) { emit columnSortRequest(index); }