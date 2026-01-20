#include "View/ChartsDialogView.h"

ChartsView::ChartsView(QWidget* parent) : QWidget(parent)
{
    setupUI();
}

void ChartsView::setupUI()
{
    QVBoxLayout* contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(30, 30, 30, 30);
    contentLayout->setSpacing(20);

    // Nagłówek
    QLabel* viewLabel = new QLabel("Statistics & Analytics");
    viewLabel->setObjectName("viewLabel"); 
    contentLayout->addWidget(viewLabel);

    // Layout na wykresy
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    // --- 1. WYKRES KOŁOWY ---
    QChart* chartPie = new QChart();
    chartPie->setTitle("Expenses by Category");
    chartPie->setAnimationOptions(QChart::SeriesAnimations);
    chartPie->setTheme(QChart::ChartThemeDark); 
    chartPie->setBackgroundVisible(false); 

    chartPie->legend()->setVisible(true);
    chartPie->legend()->setAlignment(Qt::AlignRight);
    chartPie->legend()->setLabelColor(Qt::white);

    chartViewPie = new QChartView(chartPie);
    chartViewPie->setRenderHint(QPainter::Antialiasing);
    chartViewPie->setObjectName("chartPlaceholder");

    // --- 2. WYKRES SŁUPKOWY ---
    QChart* chartBar = new QChart();
    chartBar->setTitle("Income vs Expenses");
    chartBar->setAnimationOptions(QChart::SeriesAnimations);
    chartBar->setTheme(QChart::ChartThemeDark);
    chartBar->setBackgroundVisible(false);

    chartBar->legend()->setVisible(true);
    chartBar->legend()->setAlignment(Qt::AlignBottom);
    chartBar->legend()->setLabelColor(Qt::white);

    chartViewBar = new QChartView(chartBar);
    chartViewBar->setRenderHint(QPainter::Antialiasing);
    chartViewBar->setObjectName("chartPlaceholder");

    chartsLayout->addWidget(chartViewPie);
    chartsLayout->addWidget(chartViewBar);

    contentLayout->addLayout(chartsLayout);
}

void ChartsView::updatePieChart(const QMap<QString, double>& data)
{
    QChart* chart = chartViewPie->chart();
    chart->removeAllSeries();

    if (data.isEmpty()) return;

    QPieSeries* series = new QPieSeries();
    for (auto it = data.begin(); it != data.end(); ++it) {
        series->append(it.key(), it.value());
    }

    // Etykiety
    for (auto slice : series->slices()) {
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
    }

    chart->addSeries(series);
}

void ChartsView::updateBarChart(double totalIncome, double totalExpense)
{
    QChart* chart = chartViewBar->chart();
    chart->removeAllSeries();


    auto axes = chart->axes();
    for (auto axis : axes) {
        chart->removeAxis(axis);
        delete axis;
    }

    QBarSet* setIncome = new QBarSet("Income");
    QBarSet* setExpense = new QBarSet("Expenses");

    *setIncome << totalIncome;
    *setExpense << totalExpense;

    setIncome->setColor(QColor("#2ecc71"));
    setExpense->setColor(QColor("#e74c3c"));

    QBarSeries* series = new QBarSeries();
    series->append(setIncome);
    series->append(setExpense);

    chart->addSeries(series);


    QStringList categories;
    categories << "Current Period";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(Qt::white);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setLabelsColor(Qt::white);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}