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
	void displayPieChart(const std::map<int, double>& categorySums, const std::map<int, QString>& categoryNames);
	void displayBarChart(const std::map<int, double>& incomeSums, const std::map<int, double>& expenseSums, int currentMonth);

	void clearLayout(QLayout* layout);

private:
	Ui::ChartsDialogViewClass *ui;
};

