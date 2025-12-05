#include "View/MainWindowView.h"
#include "ui_BudgetManagementQtVS.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), TableModel(new QStandardItemModel(this))
{
	ui->setupUi(this);

    setupTableModel();
	setupConnections();


}

MainWindow::~MainWindow()
{
	delete ui;

}

void MainWindow::setupTableModel()
{
    TableModel->setColumnCount(7);
    TableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
    TableModel->setHeaderData(1, Qt::Horizontal, tr("Nazwa"));
    TableModel->setHeaderData(2, Qt::Horizontal, tr("Data"));
    TableModel->setHeaderData(3, Qt::Horizontal, tr("Opis"));
    TableModel->setHeaderData(4, Qt::Horizontal, tr("Kwota"));
    TableModel->setHeaderData(5, Qt::Horizontal, tr("Typ"));
    TableModel->setHeaderData(6, Qt::Horizontal, tr("Kategoria"));

    ui->TransactionTabelView->setModel(TableModel);
    ui->TransactionTabelView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TransactionTabelView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TransactionTabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupConnections() 
{
	connect(ui->buttonAddTransaction, &QPushButton::clicked, this, &MainWindow::onButtonAddTransactionClicked);

	connect(ui->buttonDeleteTransaction, &QPushButton::clicked, this, &MainWindow::onButtonDeleteTransactionClicked);
    

}

void MainWindow::onButtonAddTransactionClicked()
{
	emit addTransactionRequested();
}

void MainWindow::onButtonDeleteTransactionClicked()
{
	emit deleteTransactionRequested();
}

void MainWindow::setTransactionRows(const QVector<QStringList>& rows)
{
    TableModel->removeRows(0, TableModel->rowCount());

    int row = 0;
    for (const auto& r : rows)
    {
        TableModel->insertRow(row);
        for (int col = 0; col < r.size(); ++col)
        {
            TableModel->setData(TableModel->index(row, col), r[col]);
        }
        ++row;
    }

    ui->TransactionTabelView->resizeColumnsToContents();
}

int MainWindow::selectedTranstacionId() const
{
    QModelIndex index = ui->TransactionTabelView->currentIndex();
    if (!index.isValid())
        return -1;

    bool ok = false;
    int id = TableModel->data(TableModel->index(index.row(), 0)).toInt(&ok);
    return ok ? id : -1;
}
