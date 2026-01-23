#include "Controller/ChartsController.h"
#include <QDate>
#include <map>

ChartsController::ChartsController(TransactionRepository& transRepo, CategoryRepository& catRepo,
    QObject* parent)
    : BaseController(parent),
	transactionRepository(transRepo),
    categoryRepository(catRepo),
    view(new ChartsView())
{
    connect(view, &ChartsView::dateRangeChanged, this, &ChartsController::refreshData);
}

void ChartsController::run()
{
    refreshData();
}

QPointer<ChartsView> ChartsController::getView() const
{
    return view;
}

void ChartsController::refreshData() {
    int profileId = getProfileId();
    QDate start = view->getStartDateEdit()->date();
    QDate end = view->getEndDateEdit()->date();

    double income = transactionRepository.getSumByTypeAndDate(profileId, "Income", start, end);
    double expense = transactionRepository.getSumByTypeAndDate(profileId, "Expense", start, end);
    double avg = transactionRepository.getAllTimeMonthlyAverageExpense(profileId);

    view->setIncomeValue(income);
    view->setExpenseValue(expense);
    view->setAverageValue(avg);


    view->updateBarChart(income, expense);



    QMap<int, double> rawCatData = transactionRepository.getExpensesByCategory(profileId, start, end);


    QMap<QString, double> pieChartData;
    for (auto it = rawCatData.begin(); it != rawCatData.end(); ++it) {
        int catId = it.key();
        double amount = it.value();

        QString catName = categoryRepository.getCategoryNameById(catId);
        if (catName.isEmpty()) catName = "Unknown";

        pieChartData.insert(catName, amount);
    }

    view->updatePieChart(pieChartData);
}

