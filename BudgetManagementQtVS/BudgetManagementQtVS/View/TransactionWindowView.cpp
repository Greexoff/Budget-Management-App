#include "View/TransactionWindowView.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QFrame>

TransactionWindow::TransactionWindow(QWidget* parent)
    : QMainWindow(parent), TableModel(new QStandardItemModel(this))
{
    setupUI();
    setupStyle();

    initializeTransactionTable();
    setWindowTitle("Budget Manager - Dashboard");
    resize(1200, 800);

    setupConnections();
}

TransactionWindow::~TransactionWindow()
{
}

void TransactionWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- 1. SIDEBAR  ---
    QFrame* sidebarFrame = new QFrame();
    sidebarFrame->setObjectName("sidebarFrame");
    sidebarFrame->setFixedWidth(250);

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 20, 0, 20);
    sidebarLayout->setSpacing(10);

    QLabel* appTitle = new QLabel("Budget App");
    appTitle->setObjectName("appTitle");
    appTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addSpacing(30);

    btnNavTransactions = new QPushButton("Transactions");
    btnNavCategories = new QPushButton("Categories");
    btnNavAccounts = new QPushButton("Financial Accounts");
    btnNavCharts = new QPushButton("Charts");

    // Styl początkowy
    btnNavTransactions->setObjectName("navButtonActive");
    btnNavCategories->setObjectName("navButton");
    btnNavAccounts->setObjectName("navButton");
    btnNavCharts->setObjectName("navButton");

    sidebarLayout->addWidget(btnNavTransactions);
    sidebarLayout->addWidget(btnNavCategories);
    sidebarLayout->addWidget(btnNavAccounts);
    sidebarLayout->addWidget(btnNavCharts);

    sidebarLayout->addStretch();
    btnNavProfile = new QPushButton("Log out / Profile");
    btnNavProfile->setObjectName("navButtonLogout");
    sidebarLayout->addWidget(btnNavProfile);

    // --- 2. CONTENT  ---
    stackedWidget = new QStackedWidget();

    
    pageTransactions = createTransactionsPage();
    stackedWidget->addWidget(pageTransactions);

    
    stackedWidget->addWidget(new QWidget());


    QWidget* accountsPlaceholder = new QWidget();
    new QVBoxLayout(accountsPlaceholder); 
    stackedWidget->addWidget(accountsPlaceholder);


    QWidget* chartsPlaceholder = new QWidget();
    new QVBoxLayout(chartsPlaceholder);
    stackedWidget->addWidget(chartsPlaceholder);


    mainLayout->addWidget(sidebarFrame);
    mainLayout->addWidget(stackedWidget);
}


void TransactionWindow::setCategoryView(CategorySelectionView* view)
{

    QWidget* old = stackedWidget->widget(1);
    if (old) {
        stackedWidget->removeWidget(old);
    }

    categoryView = view;

    stackedWidget->insertWidget(1, categoryView);
}

void TransactionWindow::setAccountView(FinancialAccountSelectionView* view)
{


    QWidget* old = stackedWidget->widget(2);
    if (old) {
        stackedWidget->removeWidget(old);

    }

    accountView = view;
    stackedWidget->insertWidget(2, accountView);
}

void TransactionWindow::setChartsView(ChartsView* view)
{
    QWidget* old = stackedWidget->widget(3); 
    if (old) stackedWidget->removeWidget(old);

    chartsView = view;
    stackedWidget->insertWidget(3, chartsView);
}


QWidget* TransactionWindow::createTransactionsPage()
{
    QWidget* pageWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(pageWidget);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // Header
    QHBoxLayout* headerLayout = new QHBoxLayout();
    QLabel* viewLabel = new QLabel("Transactions Overview");
    viewLabel->setObjectName("viewLabel");
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search transactions...");
    searchEdit->setFixedWidth(300);
    headerLayout->addWidget(viewLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(searchEdit);

    // Budget
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

    // Actions
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

    // Table
    transactionTable = new QTableView();
    transactionTable->setAlternatingRowColors(true);
    transactionTable->setShowGrid(false);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->verticalHeader()->setVisible(false);

    contentLayout->addLayout(headerLayout);
    contentLayout->addWidget(budgetFrame);
    contentLayout->addLayout(actionLayout);
    contentLayout->addWidget(transactionTable);

    return pageWidget;
}

void TransactionWindow::setupConnections()
{
    // Nawigacja
    connect(btnNavTransactions, &QPushButton::clicked, [this]() { navigateToPage(0); });
    connect(btnNavCategories, &QPushButton::clicked, [this]() { navigateToPage(1); if (categoryView) categoryView->refreshView(); });
    connect(btnNavAccounts, &QPushButton::clicked, [this]() { navigateToPage(2); if (accountView) accountView->refreshView(); });
    connect(btnNavCharts, &QPushButton::clicked, [this]() {navigateToPage(3); emit showChartsRequest(); });
    connect(btnNavProfile, &QPushButton::clicked, this, &TransactionWindow::onButtonChangeProfileClicked);

    // Transakcje
    connect(btnAddTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonAddTransactionClicked);
    connect(btnEditTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonEditTransactionClicked);
    connect(btnDeleteTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonDeleteTransactionClicked);
    connect(btnEditBudget, &QPushButton::clicked, this, &TransactionWindow::onButtonEditBudgetClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &TransactionWindow::searchTextChanged);
    connect(transactionTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TransactionWindow::onColumnHeaderClicked);
}

void TransactionWindow::navigateToPage(int pageIndex)
{
    stackedWidget->setCurrentIndex(pageIndex);
    updateSidebarStyle(pageIndex);
}

void TransactionWindow::updateSidebarStyle(int activeIndex)
{
    btnNavTransactions->setObjectName("navButton");
    btnNavCategories->setObjectName("navButton");
    btnNavAccounts->setObjectName("navButton");
    btnNavCharts->setObjectName("navButton");


    switch (activeIndex) {
    case 0: btnNavTransactions->setObjectName("navButtonActive"); break;
    case 1: btnNavCategories->setObjectName("navButtonActive"); break;
    case 2: btnNavAccounts->setObjectName("navButtonActive"); break;
    case 3: btnNavCharts->setObjectName("navButtonActive"); break;
    }


    btnNavTransactions->style()->unpolish(btnNavTransactions); btnNavTransactions->style()->polish(btnNavTransactions);
    btnNavCategories->style()->unpolish(btnNavCategories); btnNavCategories->style()->polish(btnNavCategories);
    btnNavAccounts->style()->unpolish(btnNavAccounts); btnNavAccounts->style()->polish(btnNavAccounts);
    btnNavCharts->style()->unpolish(btnNavCharts); btnNavCharts->style()->polish(btnNavCharts);
}

void TransactionWindow::setupStyle()
{
    this->setStyleSheet(
        "QMainWindow { background-color: #121212; color: white; }"
        "QWidget { color: white; }"
        "QFrame#sidebarFrame { background-color: #1e1e1e; border-right: 1px solid #333333; }"
        "QLabel#appTitle { color: white; font-size: 20px; font-weight: bold; margin-bottom: 20px; }"
        "QPushButton#navButton, QPushButton#navButtonLogout { text-align: left; padding: 15px 30px; border: none; color: #b0bec5; font-size: 14px; background: transparent; }"
        "QPushButton#navButton:hover { background-color: #333333; color: white; }"
        "QPushButton#navButtonActive { text-align: left; padding: 15px 30px; border: none; color: white; font-size: 14px; background-color: #2c3e50; border-left: 5px solid #3498db; }"
        "QLabel#viewLabel { font-size: 24px; font-weight: bold; color: white; }"
        "QLineEdit, QDateEdit, QSpinBox, QDoubleSpinBox, QComboBox { padding: 8px; border: 1px solid #444444; border-radius: 5px; background-color: #2d2d2d; color: white; selection-background-color: #3498db; }"
        "QFrame#budgetFrame { background-color: #1e1e1e; border-radius: 10px; border: 1px solid #333333; padding: 10px; }"
        "QFrame#budgetFrame QLabel { color: white; }"
        "QProgressBar { border: none; background-color: #2d2d2d; border-radius: 7px; text-align: center; color: white; }"
        "QProgressBar::chunk { background-color: #2ecc71; border-radius: 7px; }"
        "QPushButton#actionButtonAdd { background-color: #27ae60; color: white; border-radius: 5px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton#actionButtonDelete { background-color: #c0392b; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QPushButton#actionButton { background-color: #2980b9; color: white; border-radius: 5px; padding: 8px 15px; }"
        "QTableView { border: 1px solid #333333; background-color: #1e1e1e; color: white; gridline-color: #333333; selection-background-color: #3498db; selection-color: white; }"
        "QHeaderView::section { background-color: #2d2d2d; padding: 8px; border: 1px solid #333333; font-weight: bold; color: white; }"
        "QDialog, QMessageBox, QInputDialog { background-color: #1e1e1e; color: white; font-size: 14px; }"
        "QDialog QPushButton { background-color: #333333; color: white; border: 1px solid #444444; border-radius: 4px; padding: 6px 15px; }"
    );
}


void TransactionWindow::initializeTransactionTable() {
    TableModel->setColumnCount(8);
    TableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    TableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    TableModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    TableModel->setHeaderData(3, Qt::Horizontal, tr("Description"));
    TableModel->setHeaderData(4, Qt::Horizontal, tr("Amount"));
    TableModel->setHeaderData(5, Qt::Horizontal, tr("Type"));
    TableModel->setHeaderData(6, Qt::Horizontal, tr("Category"));
    TableModel->setHeaderData(7, Qt::Horizontal, tr("Account"));
    transactionTable->setModel(TableModel);
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->setColumnHidden(0, true);
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TransactionWindow::setTransactionTabHeaders(const QVector<QStringList>& rows) {
    TableModel->clear();
    QStringList headers = { "ID", "Name", "Date", "Description", "Amount", "Type", "Category", "Account" };
    TableModel->setHorizontalHeaderLabels(headers);
    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row) items.append(new QStandardItem(value));
        TableModel->appendRow(items);
    }
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    transactionTable->setColumnHidden(0, true);
}

int TransactionWindow::getSelectedTransactionId() const {
    QModelIndex index = transactionTable->currentIndex();
    if (!index.isValid()) return -1;
    bool ok;
    int id = TableModel->data(TableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}

void TransactionWindow::updateBudgetDisplay(double limit, double spent) {
    budgetProgressBar->setRange(0, 100);
    double percentage = (limit > 0) ? (spent / limit) * 100.0 : 0.0;
    budgetProgressBar->setValue(percentage > 100 ? 100 : (int)percentage);
    double remaining = limit - spent;
    budgetLabel->setText(QString("Budget Usage: %1 / %2 PLN (Remaining: %3 PLN)").arg(spent).arg(limit).arg(remaining));
}

void TransactionWindow::clearSearchEdit() { searchEdit->clear(); }
void TransactionWindow::showTransactionMessage(QString header, QString message, QString messageType) {
    if (messageType == "error") QMessageBox::warning(this, header, message);
    else QMessageBox::information(this, header, message);
}

void TransactionWindow::onButtonAddTransactionClicked() { emit addTransactionRequest(); }
void TransactionWindow::onButtonEditTransactionClicked() { emit editTransactionRequest(); }
void TransactionWindow::onButtonDeleteTransactionClicked() { emit deleteTransactionRequest(); }
void TransactionWindow::onButtonEditBudgetClicked() { emit editBudgetRequest(); }
void TransactionWindow::onButtonChangeProfileClicked() { emit backToProfileRequested(); }
void TransactionWindow::searchTextChanged(const QString& text) { emit searchTextRequest(text); }
void TransactionWindow::onColumnHeaderClicked(int columnId) { emit columnSortRequest(columnId); }