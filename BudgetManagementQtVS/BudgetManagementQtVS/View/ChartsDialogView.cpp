#include "View/ChartsDialogView.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieSeries>

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

void ChartsDialogView::displayCharts(const std::map<int, double>& categorySums, const std::map<int, QString>& categoryNames)
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

    // Pass 'this' as the parent to prevent memory leaks
    QChartView* pieChartView = new QChartView(pieChart, this);
    pieChartView->setRenderHint(QPainter::Antialiasing);
	//pieChartView->setMinimumSize(800, 600);

    // Assuming your Dialog has a layout (e.g., QVBoxLayout* mainLayout)
    // You need to add the view to the UI to actually see it!
    /*if (this->layout()) {
        this->layout()->addWidget(pieChartView);
    }*/
	ui->verticalLayout->addWidget(pieChartView);

    this->show();
}

