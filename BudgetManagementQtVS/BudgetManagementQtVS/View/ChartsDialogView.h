#pragma once

#include <QMainWindow>
#include "ui_ChartsDialogView.h"

class ChartsDialogView : public QMainWindow
{
	Q_OBJECT

public:
	ChartsDialogView(QWidget *parent = nullptr);
	~ChartsDialogView();

private:
	Ui::ChartsDialogViewClass ui;
};

