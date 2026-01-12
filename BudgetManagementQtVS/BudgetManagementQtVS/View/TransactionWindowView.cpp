#include "View/TransactionWindowView.h"
#include "ui_TransactionWindowView.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QHeaderView>  

TransactionWindow::TransactionWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), TableModel(new QStandardItemModel(this))
{
	ui->setupUi(this);

    initializeTransactionTable();
    setWindowTitle("Browse transactions");
	setupConnections();
}

TransactionWindow::~TransactionWindow()
{
	delete ui;
}


//----------------Setting connection (button-method)-------------------------------------------------

void TransactionWindow::setupConnections()
{
    connect(ui->buttonAddTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonAddTransactionClicked);
    connect(ui->buttonDeleteTransaction, &QPushButton::clicked, this, &TransactionWindow::onButtonDeleteTransactionClicked);
    connect(ui->browseCategoriesButton, &QPushButton::clicked, this, &TransactionWindow::onButtonShowCategoriesClicked);
    connect(ui->buttonEdit, &QPushButton::clicked, this, &TransactionWindow::onButtonEditTransactionClicked);
    connect(ui->changeProfileButton, &QPushButton::clicked, this, &TransactionWindow::onButtonChangeProfileClicked);
    connect(ui->browseFinancialAccountsButton, &QPushButton::clicked, this, &TransactionWindow::onButtonShowFinancialAccountsClicked);
    connect(ui->editBudgetButton, &QPushButton::clicked, this, &TransactionWindow::onButtonEditBudgetClicked);
	connect(ui->buttonCharts, &QPushButton::clicked, this, &TransactionWindow::onButtonChartsClicked); //CHARTS!!
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &TransactionWindow::searchTextChanged);

    QHeaderView* header = ui->TransactionTabelView->horizontalHeader();
    connect(header, &QHeaderView::sectionClicked, this, &TransactionWindow::onColumnHeaderClicked);
}


//----------------Setting up view-------------------------------------------------


void TransactionWindow::initializeTransactionTable()
{
    TableModel->setColumnCount(8);
    TableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    TableModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    TableModel->setHeaderData(2, Qt::Horizontal, tr("Date"));
    TableModel->setHeaderData(3, Qt::Horizontal, tr("Description"));
    TableModel->setHeaderData(4, Qt::Horizontal, tr("Amount"));
    TableModel->setHeaderData(5, Qt::Horizontal, tr("Type"));
    TableModel->setHeaderData(6, Qt::Horizontal, tr("Category"));
    TableModel->setHeaderData(7, Qt::Horizontal, tr("Financial Account"));

    ui->TransactionTabelView->setModel(TableModel);

    ui->TransactionTabelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TransactionTabelView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TransactionTabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->TransactionTabelView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TransactionTabelView->setColumnHidden(0, true);
}


void TransactionWindow::setTransactionTabHeaders(const QVector<QStringList>& rows)
{
    TableModel->clear();

    TableModel->setHorizontalHeaderLabels({ "ID", "Name", "Date", "Description", "Amount", "Type", "Category","Financial account" });

    for (const QStringList& row : rows) {
        QList<QStandardItem*> items;
        for (const QString& value : row)
            items.append(new QStandardItem(value));
        TableModel->appendRow(items);
    }

    auto header = ui->TransactionTabelView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    ui->TransactionTabelView->setColumnHidden(0, true);
}


//----------------Pressing buttons actions-------------------------------------------------

//Method that notices clicking on add button
void TransactionWindow::onButtonAddTransactionClicked()
{
	emit addTransactionRequest();
}

//Method that notices clicking on edit button
void TransactionWindow::onButtonEditTransactionClicked()
{
    emit editTransactionRequest();
}

//Method that notices clicking on delete button
void TransactionWindow::onButtonDeleteTransactionClicked()
{
	emit deleteTransactionRequest();
}

//Method that notices clicking on browse category button
void TransactionWindow::onButtonShowCategoriesClicked()
{
    emit showCategoriesRequest();
}

//Method that notices clicking on browse financial accounts button
void TransactionWindow::onButtonShowFinancialAccountsClicked()
{
    emit showFinancialAccountsRequest();
}

//Method that notices clicking on return to profile window button
void TransactionWindow::onButtonChangeProfileClicked()
{
    emit backToProfileRequested();
}

//Method that notices clicking on edit budget button
void TransactionWindow::onButtonEditBudgetClicked()
{
    emit editBudgetRequest();
}

// CHARTS!!
void TransactionWindow::onButtonChartsClicked() 
{
	emit showChartsRequest();
}

//Method that clears search bar while reentering view
void TransactionWindow::clearSearchEdit()
{
    ui->searchEdit->clear();
}

//Method that passes text inserted in search bar
void TransactionWindow::searchTextChanged(QString searchText)
{
    emit searchTextRequest(searchText);
}

//Method that notices clicking on column header
void TransactionWindow::onColumnHeaderClicked(int columnId)
{
    emit columnSortRequest(columnId);
}

//Method that passes id of currently selected transaction
int TransactionWindow::getSelectedTransactionId() const
{
    QModelIndex index = ui->TransactionTabelView->currentIndex();
    if (!index.isValid())
        return -1;

    bool ok = false;
    int id = TableModel->data(TableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}

//Method that updates progress bar of monthly budget
void TransactionWindow::updateBudgetDisplay(double limit, double spent)
{
    ui->budgetProgressBar->setRange(0, 100);

    double percentage = (limit > 0) ? (spent / limit) * 100.0 : 0.0;

    int visualValue = (percentage > 100.0) ? 100 : static_cast<int>(percentage);
    ui->budgetProgressBar->setValue(visualValue);

    if (spent > limit && limit > 0) {
        ui->budgetProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #ff4d4d; }");
    }
    else {
        ui->budgetProgressBar->setStyleSheet("QProgressBar::chunk { background-color: #4CAF50; }"); 
    }

    double remaining = limit - spent;
    ui->budgetLabel->setText(QString("Spent: %1 / Limit: %2 PLN (Remaining: %3 PLN)")
        .arg(QString::number(spent, 'f', 2))
        .arg(QString::number(limit, 'f', 2))
        .arg(QString::number(remaining, 'f', 2)));
}

//Method responsible for displaying error if inserted data is incorrect/is lacking 
void TransactionWindow::showTransactionMessage(QString header, QString message, QString messageType)
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