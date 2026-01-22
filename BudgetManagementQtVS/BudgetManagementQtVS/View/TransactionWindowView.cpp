#include "View/TransactionWindowView.h"

TransactionWindow::TransactionWindow(QWidget* parent)
    : QWidget(parent), tableModel(new QStandardItemModel(this))
{
    setupUI();
    setupStyle();
    setupConnections();
}

void TransactionWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* viewLabel = new QLabel("Transactions Overview");
    viewLabel->setObjectName("viewLabel");
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search transactions...");
    searchEdit->setFixedWidth(300);
    headerLayout->addWidget(viewLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchEdit);

    QFrame* budgetFrame = new QFrame();
    budgetFrame->setObjectName("budgetFrame");
    QVBoxLayout* budgetLayout = new QVBoxLayout(budgetFrame);
    QHBoxLayout* budgetHeaderLayout = new QHBoxLayout();
    budgetLabel = new QLabel("Monthly Budget Usage");
    btnEditBudget = new QPushButton("Set Limit");
    btnEditBudget->setFixedWidth(100);
    budgetHeaderLayout->addWidget(budgetLabel);
    budgetHeaderLayout->addStretch();
    budgetHeaderLayout->addWidget(btnEditBudget);
    budgetProgressBar = new QProgressBar();
    budgetProgressBar->setFixedHeight(15);
    budgetProgressBar->setTextVisible(false);
    budgetLayout->addLayout(budgetHeaderLayout);
    budgetLayout->addWidget(budgetProgressBar);

    QHBoxLayout* actionLayout = new QHBoxLayout();
    btnAddTransaction = new QPushButton("+ Add Transaction");
    btnAddTransaction->setObjectName("actionButtonAdd");
    btnEditTransaction = new QPushButton("Edit");
    btnEditTransaction->setObjectName("actionButton");
    btnDeleteTransaction = new QPushButton("Delete");
    btnDeleteTransaction->setObjectName("actionButtonDelete");
    actionLayout->addWidget(btnAddTransaction);
    actionLayout->addWidget(btnEditTransaction);
    actionLayout->addWidget(btnDeleteTransaction);
    actionLayout->addStretch();

    transactionTable = new QTableView();
    initializeTransactionTable();

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(budgetFrame);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(transactionTable);
}

void TransactionWindow::initializeTransactionTable() const
{
    transactionTable->setModel(tableModel);
    QStringList headers = { "ID", "Name", "Date", "Description", "Amount", "Type", "Category", "Account" };
    tableModel->setHorizontalHeaderLabels(headers);
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->setColumnHidden(0, true);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->verticalHeader()->setVisible(false);
    transactionTable->setAlternatingRowColors(true);
}

void TransactionWindow::setTransactionTabHeaders(const QVector<QStringList>& rows) const
{
    tableModel->removeRows(0, tableModel->rowCount());
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row) items.append(new QStandardItem(value));
        tableModel->appendRow(items);
    }
}

void TransactionWindow::updateBudgetDisplay(double limit, double spent) const
{
    double percentage = (limit > 0) ? (spent / limit) * 100.0 : 0.0;
    budgetProgressBar->setValue(percentage > 100 ? 100 : static_cast<int>(percentage));
    budgetLabel->setText(QString("Budget: %1 / %2 PLN (Remaining: %3 PLN)")
        .arg(spent, 0, 'f', 2).arg(limit, 0, 'f', 2).arg(limit - spent, 0, 'f', 2));
}

int TransactionWindow::getSelectedTransactionId() const {
    QModelIndex index = transactionTable->currentIndex();
    if (!index.isValid()) return -1;
    return tableModel->data(tableModel->index(index.row(), 0)).toInt();
}

void TransactionWindow::showTransactionMessage(const QString& header, const QString& message, const QString& messageType) {
    if (messageType == "error") QMessageBox::warning(this, header, message);
    else QMessageBox::information(this, header, message);
}

void TransactionWindow::setupConnections()
{
    connect(btnAddTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonAddTransactionClicked);
    connect(btnEditTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonEditTransactionClicked);
    connect(btnDeleteTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonDeleteTransactionClicked);
    connect(btnEditBudget, &QPushButton::clicked, this, &TransactionWindow::onButtonEditBudgetClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &TransactionWindow::onSearchTextChanged);
    connect(transactionTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TransactionWindow::onColumnHeaderClicked);
}

void TransactionWindow::clearSearchEdit() const { searchEdit->clear(); }

void TransactionWindow::setupStyle()
{
    this->setStyleSheet(
        "QWidget { color: white; background-color: transparent; }"
        "QLabel#viewLabel { font-size: 24px; font-weight: bold; color: white; }"
        "QLineEdit { padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white; }"
        "QFrame#budgetFrame { background-color: #1e1e1e; border-radius: 10px; border: 1px solid #333333; padding: 10px; }"
        "QProgressBar { border: none; background-color: #2d2d2d; border-radius: 7px; text-align: center; }"
        "QProgressBar::chunk { background-color: #2ecc71; border-radius: 7px; }"
        "QPushButton#actionButtonAdd { background-color: #27ae60; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton#actionButtonDelete { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QPushButton#actionButton { background-color: #2980b9; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QTableView { border: 1px solid #333333; background-color: #1e1e1e; color: white; selection-background-color: #3498db; }"
        "QHeaderView::section { background-color: #2d2d2d; padding: 8px; border: 1px solid #333333; font-weight: bold; }"
    );
}