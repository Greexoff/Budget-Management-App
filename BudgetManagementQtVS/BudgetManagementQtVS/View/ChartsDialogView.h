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
#include <QDateEdit>



class ChartsView : public QWidget
{
    Q_OBJECT

public:
    explicit ChartsView(QWidget* parent = nullptr);

    QDateEdit* getStartDateEdit() const { return startDateEdit; }
    QDateEdit* getEndDateEdit() const { return endDateEdit; }

    void setIncomeValue(double value);
    void setExpenseValue(double value);
    void setAverageValue(double value);

    void updatePieChart(const QMap<QString, double>& data); 
    void updateBarChart(double totalIncome, double totalExpense); 

signals:
    void dateRangeChanged();

private:
    void setupUI();
    void setupStyle();

    QChartView* pieChartView;
    QChartView* barChartView;

    QLabel* incomeValLabel;
    QLabel* expenseValLabel;
    QLabel* averageValLabel;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
};