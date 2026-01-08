#include "View/ChartsDialogView.h"

ChartsDialogView::ChartsDialogView(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ChartsDialogViewClass())
{
	ui->setupUi(this);
}

ChartsDialogView::~ChartsDialogView()
{
	delete ui;
}

