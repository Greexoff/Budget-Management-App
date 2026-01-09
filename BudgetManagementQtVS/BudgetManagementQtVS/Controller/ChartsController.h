#pragma once
#include <Controller/BaseController.h>

#include <Model/Repositories/TransactionRepository.h>
#include <Model/Repositories/CategoryRepository.h>

#include <View/ChartsDialogView.h>


class ChartsController : public BaseController {
	Q_OBJECT

public:
	ChartsController(TransactionRepository& transactionRepoRef, CategoryRepository& categoryRepoRef, ChartsDialogView& chartsDialogViewRef, QObject* parent = nullptr);
	// Setting up plots
	void setUpPieChart();
	void setUptBarChart();
	QVector<int> getCurrentMonthAndYear();
	void getSumsforBarChartByDate(std::map<int, double>& incomeSums, std::map<int, double>& expenseSums, const int currentMonth, const int currentYear);


public slots:
	void setupChartsWindow();



private:
	TransactionRepository& transactionRepository;   ///< Reference to the transaction repository
	CategoryRepository& categoryRepository;         ///< Reference to the category repository	
	ChartsDialogView& chartsDialogView;           ///< Reference to the charts dialog view
};