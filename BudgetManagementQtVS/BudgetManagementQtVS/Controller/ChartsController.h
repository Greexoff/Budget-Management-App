#pragma once
#include <Controller/BaseController.h>

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>

#include <View/TransactionWindowView.h>
#include "View/ChartsDialogView.h"


class ChartsController : public BaseController
{
    Q_OBJECT

public:
    ChartsController(TransactionWindow& windowRef, ChartsView& viewRef,
        TransactionRepository& transRepo, CategoryRepository& catRepo,
        QObject* parent = nullptr);

public slots:
    void updateCharts();

private:
    TransactionWindow& window;
    ChartsView& view;
    TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;

    void setUpPieChart();
    void setUptBarChart();
    QVector<int> getCurrentMonthAndYear();
    void getSumsforBarChartByDate(std::map<int, double>& incomeSums, std::map<int, double>& expenseSums, const int currentMonth, const int currentYear);
};