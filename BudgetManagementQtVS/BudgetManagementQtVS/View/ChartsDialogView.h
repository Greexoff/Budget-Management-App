#pragma once

#include <QDialog>
#include "ui_ChartsDialogView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChartsDialogViewClass; };
QT_END_NAMESPACE

class ChartsDialogView : public QDialog
{
	Q_OBJECT

public:
	ChartsDialogView(QWidget *parent = nullptr);
	~ChartsDialogView();
	void displayCharts(const std::map<int, double>& categorySums, const std::map<int, QString>& categoryNames);

private:
	Ui::ChartsDialogViewClass *ui;
};

