#include "Controller/ChartsController.h"
#include <QDate>
#include <map>

ChartsController::ChartsController(TransactionWindow& windowRef, ChartsView& viewRef,
    TransactionRepository& transRepo, CategoryRepository& catRepo,
    QObject* parent)
    : BaseController(parent),
    window(windowRef),
    view(viewRef),
    transactionRepository(transRepo),
    categoryRepository(catRepo)
{
 
    connect(&window, &TransactionWindow::showChartsRequest, this, &ChartsController::updateCharts);
}

void ChartsController::updateCharts()
{
    setUpPieChart();
    setUptBarChart();
}

QVector<int> ChartsController::getCurrentMonthAndYear()
{
    QDate d = QDate::currentDate();
    return QVector<int>{ d.month(), d.year() };
}

void ChartsController::setUpPieChart()
{

    QVector<Transaction> transactions = transactionRepository.getAllProfileTransaction(getProfileId());
    QMap<QString, double> chartData;

    for (const auto& t : transactions) {
        if (t.getTransactionType() == "Expense") {
            QString catName = categoryRepository.getCategoryNameById(t.getCategoryId());
            chartData[catName] += t.getTransactionAmount();
        }
    }
    view.updatePieChart(chartData);
}

void ChartsController::setUptBarChart()
{
    QVector<int> dateInfo = getCurrentMonthAndYear();
    std::map<int, double> incomeSums, expenseSums;
    getSumsforBarChartByDate(incomeSums, expenseSums, dateInfo[0], dateInfo[1]);

    double totalIncome = 0.0;
    double totalExpense = 0.0;

    for (auto const& [d, val] : incomeSums) totalIncome += val;
    for (auto const& [d, val] : expenseSums) totalExpense += val;

    view.updateBarChart(totalIncome, totalExpense);
}

void ChartsController::getSumsforBarChartByDate(std::map<int, double>& incomeSums, std::map<int, double>& expenseSums, int m, int y)
{
    QVector<Transaction> transactions = transactionRepository.getAllProfileTransaction(getProfileId());
    for (const auto& t : transactions) {
        if (t.getTransactionDate().month() == m && t.getTransactionDate().year() == y) {
            int d = t.getTransactionDate().day();
            if (t.getTransactionType() == "Income") incomeSums[d] += t.getTransactionAmount();
            else if (t.getTransactionType() == "Expense") expenseSums[d] += t.getTransactionAmount();
        }
    }
}