#include "View/ChartsDialogView.h"

#include <QtCharts>

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


void ChartsDialogView::displayPieChart(const std::map<int, double>& categorySums, const std::map<int, QString>& categoryNames)
{	
    QPieSeries* pieSeries = new QPieSeries();

    for (const auto& pair : categorySums) {
        int categoryId = pair.first;
        double sum = pair.second;

        if (sum > 0.0) {
            // Use .find() to avoid accidental insertions in categoryNames
            auto it = categoryNames.find(categoryId);
            QString categoryName = (it != categoryNames.end()) ? it->second : "Unknown";

            QPieSlice* slice = pieSeries->append(categoryName, sum);
            slice->setLabelVisible();
        }
    }

    QChart* pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Expense Distribution by Category");
    pieChart->legend()->setAlignment(Qt::AlignRight); // Better visual layout

    QChartView* pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
	//pieChartView->setMinimumSize(800, 600);

	ui->verticalLayout->addWidget(pieChartView);
}

void ChartsDialogView::displayBarChart(const std::map<int, double>& incomeSums, const std::map<int, double>& expenseSums, int currentMonth) {
    const std::map<int, QString> monthMap = {
        {{1}, {"January"}},
        {{2}, {"February"}},
        {{3}, {"March"}},
        {{4}, {"April"}},
        {{5}, {"May"}},
        {{6}, {"June"}},
        {{7}, {"July"}},
        {{8}, {"August"}},
        {{9}, {"September"}},
        {{10}, {"October"}},
        {{11}, {"November"}},
        {{12}, {"December"} },
    };

    QStringList months;
    
    double min = 0.0;
    double max = 0.0;

    QBarSet *incomeValue = new QBarSet("Income value");
    QBarSet *expenseValue = new QBarSet("Expense value");

    incomeValue->setColor("RED");
    expenseValue->setColor("GREEN");

    int month = 1;
    while (month <= currentMonth) {
        months.append(monthMap.at(month));
        if (incomeSums.count(month) != 0) {
			incomeValue->append(incomeSums.at(month));
        }
        else {
            incomeValue->append(0);
        }

        if (expenseSums.count(month) != 0) {
            expenseValue->append(expenseSums.at(month));
        }
        else {
            expenseValue->append(0);
        }
        month++;
    }

    /*for (const auto& pair : incomeSums) {
        int month = 1;
        incomeValue->append(pair.second);
        max = (pair.second > max) ? pair.second : max;
    }
    for (const auto& pair : expenseSums) {
        expenseValue->append(pair.second);
        min = (pair.second > min) ? pair.second : min;
    }*/

    QBarSeries *series = new QBarSeries();
    series->append(incomeValue);
    series->append(expenseValue);

    QChart* barChart = new QChart();
    barChart->addSeries(series);
    barChart->setTitle("Monthly incomes & expenses");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(months);
    axisX->setTitleText("Month");

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(min, max);
    axisY->setTitleText("Value");
    

    barChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    barChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    QChartView* barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    ui->verticalLayout_2->addWidget(barChartView);

    this->show();

    connect(this, &QDialog::finished, this, [this]() {
        clearLayout(ui->verticalLayout);
        clearLayout(ui->verticalLayout_2);
        });
}

void ChartsDialogView::clearLayout(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();   // delete the widget
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout); // recursive cleanup
        }
        delete item; // delete the layout item itself
    }
}

