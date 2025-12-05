#include "BudgetManagementQtVS.h"
#include "ui_BudgetManagementQtVS.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setupConnections();


}

MainWindow::~MainWindow()
{
	delete ui;

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
