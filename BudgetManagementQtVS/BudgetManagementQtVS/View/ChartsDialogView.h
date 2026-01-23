/**
 * @file ChartsDialogView.h
 * @brief Header file for the Charts View widget.
 */
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

 /**
  * @class ChartsView
  * @brief Widget responsible for rendering financial charts and statistics.
  */
class ChartsView : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the ChartsView.
     * @param parent The parent widget.
     */
    explicit ChartsView(QWidget* parent = nullptr);

    /** @brief Returns pointer to the start date edit widget. */
    QDateEdit* getStartDateEdit() const { return startDateEdit; }
    /** @brief Returns pointer to the end date edit widget. */
    QDateEdit* getEndDateEdit() const { return endDateEdit; }

    /** @brief Sets the text for the Income label. */
    void setIncomeValue(double value);
    /** @brief Sets the text for the Expense label. */
    void setExpenseValue(double value);
    /** @brief Sets the text for the Average Expense label. */
    void setAverageValue(double value);

    /**
     * @brief Updates the Pie Chart with category expense data.
     * @param data A map where Key is category name and Value is amount.
     */
    void updatePieChart(const QMap<QString, double>& data);

    /**
     * @brief Updates the Bar Chart comparing total income vs total expenses.
     * @param totalIncome Sum of income.
     * @param totalExpense Sum of expenses.
     */
    void updateBarChart(double totalIncome, double totalExpense);

signals:
    /** @brief Emitted when either start or end date is changed. */
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