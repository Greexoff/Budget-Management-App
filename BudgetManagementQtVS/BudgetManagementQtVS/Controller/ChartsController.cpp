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
}

void ChartsController::setUpPieChart() {
	// Get date
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);

	std::tm result{}; // Local struct to hold the time data

	localtime_s(&result, &tt);

	int current_year = result.tm_year + 1900;
	int current_month = result.tm_mon + 1;

	// Implementation for plotting pie chart
	QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId()); // Example profileId
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
				&& transaction.getTransactionDate().year() == current_year
				&& transaction.getTransactionDate().month() == current_month) {
					sum += transaction.getTransactionAmount();
			}
		}
		categorySums.insert({ category.getCategoryId(), sum });
	}
	
	for (const auto& pair : categorySums) {
		qDebug() << "Category ID:" << pair.first << "Sum of Expenses:" << pair.second;
	}

	chartsDialogView.displayCharts(categorySums, categoryNames);
}