#pragma once

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>



class ChartsView : public QWidget
{
    Q_OBJECT

public:
    explicit ChartsView(QWidget* parent = nullptr);
    ~ChartsView() = default;

    void updatePieChart(const QMap<QString, double>& data);
    void updateBarChart(double totalIncome, double totalExpense);

private:
    QChartView* chartViewPie;
    QChartView* chartViewBar;

    void setupUI();
};