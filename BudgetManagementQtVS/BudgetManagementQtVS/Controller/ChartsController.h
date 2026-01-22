#pragma once
#include <Controller/BaseController.h>

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>

#include <View/TransactionWindowView.h>
#include "View/ChartsDialogView.h"
#include <QPointer>


class ChartsController : public BaseController
{
    Q_OBJECT

public:
    ChartsController(TransactionRepository& transRepo, CategoryRepository& catRepo, QObject* parent = nullptr);
    void run();
    QWidget* getView();

public slots:
    void updateCharts();

private:
    QPointer<ChartsView> chartsView;
    TransactionRepository& transactionRepository;
    CategoryRepository& categoryRepository;

    void setUpPieChart() const;
    void setUptBarChart();
    QVector<int> getCurrentMonthAndYear();
    void getSumsforBarChartByDate(std::map<int, double>& incomeSums, std::map<int, double>& expenseSums, const int currentMonth, const int currentYear) const;
};