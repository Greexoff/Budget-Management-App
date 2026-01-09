#include <Controller/ChartsController.h>
#include <QVector>
#include <map>
#include <chrono>

#include <QtCharts/QPieSeries>

ChartsController::ChartsController(TransactionRepository& transactionRepoRef, CategoryRepository& categoryRepoRef, ChartsDialogView& chartsDialogViewRef, QObject* parent)
	: BaseController(parent), transactionRepository(transactionRepoRef), categoryRepository(categoryRepoRef), chartsDialogView(chartsDialogViewRef) {
	qDebug() << "ChartsController: Constructor called.";
}

//--------------------------Setting up view---------------------------------------------
void ChartsController::setupChartsWindow() {
	qDebug() << "ChartsController: setupChartsView called.";
	setUpPieChart();
	setUptBarChart();
}

void ChartsController::setUpPieChart() {

	QVector<int> date = getCurrentMonthAndYear();
	
	QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId());
	QVector<Transaction> transactions = transactionRepository.getAllProfileTransaction(getProfileId());
	std::map<int, double> categorySums;
	std::map<int, QString> categoryNames;

	qDebug() << "Categories retrieved:" << categories.size();
	qDebug() << "Transactions retrieved:" << transactions.size();

	// sum of transaction values per category
	for (auto category : categories) {
		categoryNames.insert({ category.getCategoryId(), category.getCategoryName() });
		double sum = 0.0;
		for (auto transaction : transactions) {
			if (transaction.getCategoryId() == category.getCategoryId() 
				&& transaction.getTransactionType() == "Expense"
				&& transaction.getTransactionDate().year() == date[1]
				&& transaction.getTransactionDate().month() == date[0]) {
					sum += transaction.getTransactionAmount();
			}
		}
		categorySums.insert({ category.getCategoryId(), sum });
	}
	
	for (const auto& pair : categorySums) {
		qDebug() << "Category ID:" << pair.first << "Sum of Expenses:" << pair.second;
	}

	chartsDialogView.displayPieChart(categorySums, categoryNames);
}

void ChartsController::setUptBarChart() {
	// dane z ostatnich miesiecy - 3

	int currentMonth = getCurrentMonthAndYear()[0];
	int currentYear = getCurrentMonthAndYear()[1];

	std::map<int, double> incomeSums = {};
	std::map<int, double> expenseSums = {};

	// getting sums from last 3 months
	getSumsforBarChartByDate(incomeSums, expenseSums, currentMonth, currentYear);

	currentMonth -= 1;
	if (currentMonth == 0) {
		currentMonth = 12;
		currentYear -= 1;
	}
	getSumsforBarChartByDate(incomeSums, expenseSums, currentMonth, currentYear);
	currentMonth -= 1;
	if (currentMonth == 0) {
		currentMonth = 12;
		currentYear -= 1;
	}
	getSumsforBarChartByDate(incomeSums, expenseSums, currentMonth, currentYear);




	chartsDialogView.displayBarChart(incomeSums, expenseSums);

}

// helper function returning a vector of ints which contains [0] - month, [1] - year
QVector<int> ChartsController::getCurrentMonthAndYear() {
	// Get date
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);

	std::tm timeData{}; // Local struct to hold the time data

	localtime_s(&timeData, &tt);

	int currentYear = timeData.tm_year + 1900;
	int currentMonth = timeData.tm_mon + 1;

	QVector<int> monthAndYear = {};
	monthAndYear.push_back(currentMonth);
	monthAndYear.push_back(currentYear);

	return monthAndYear;
}

void ChartsController::getSumsforBarChartByDate(std::map<int, double>& incomeSums, std::map<int, double>& expenseSums, const int currentMonth, const int currentYear) {
	QVector<Transaction> transactions = transactionRepository.getAllProfileTransaction(getProfileId()); // transactions

	for (auto transaction : transactions)
	{
		if (transaction.getTransactionDate().month() == currentMonth
			&& transaction.getTransactionDate().year() == currentYear) 
		{
			if (transaction.getTransactionType() == "Expense")
			{
				if (expenseSums.find(currentMonth) != expenseSums.end())
				{
					expenseSums[currentMonth] += transaction.getTransactionAmount();
				}
				else
				{
					expenseSums.insert({ {currentMonth},{transaction.getTransactionAmount()} });
				}
			}
			else if (transaction.getTransactionType() == "Income")
			{
				if (incomeSums.find(currentMonth) != incomeSums.end())
				{
					incomeSums[currentMonth] += transaction.getTransactionAmount();
				}
				else
				{
					incomeSums.insert({ {currentMonth},{transaction.getTransactionAmount()} });
				}
			}
		}
	}
}

//for (int i = 0; i < 3; i++) {
//	if (i == 0) {
//		//current month
//		for (auto transaction : transactions) {
//			if (transaction.getTransactionDate().year() == date[1]
//				&& transaction.getTransactionDate().month() == date[0])
//			{
//				if (transaction.getTransactionType() == "Expense") {
//					if (expenseSums.find(date[0]) != expenseSums.end()) {
//						expenseSums[date[0]] += transaction.getTransactionAmount();
//					}
//					else {
//						expenseSums.insert({ {date[0]}, {transaction.getTransactionAmount()} });
//					}
//				}
//				// incomes
//				else if (transaction.getTransactionType() == "Income") {
//					if (incomeSums.find(date[0]) != incomeSums.end()) {
//						incomeSums[date[0]] += transaction.getTransactionAmount();
//					}
//					else {
//						incomeSums.insert({ {date[0]}, {transaction.getTransactionAmount()} });
//
//					}
//				}
//			}
//			else {
//				date[0] -= i;
//				if (date[0] == 0) {
//					date[0] = 12;
//				}
//			}
//		}
//	}
//}