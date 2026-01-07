#include <Controller/ChartsController.h>
#include <QVector>
#include <map>
#include <chrono>

ChartsController::ChartsController(TransactionRepository& transactionRepoRef, CategoryRepository& categoryRepoRef, ChartsDialogView& chartsDialogViewRef, QObject* parent)
	: BaseController(parent), transactionRepository(transactionRepoRef), categoryRepository(categoryRepoRef), chartsDialogView(chartsDialogViewRef) {
	qDebug() << "ChartsController: Constructor called.";
}

//--------------------------Setting up view---------------------------------------------
void ChartsController::setupChartsWindow() {
	//plotPieChart();
	//plotBarChart();
	qDebug() << "ChartsController: setupChartsView called.";
	plotPieChart();
	chartsDialogView.exec();
}

void ChartsController::plotPieChart() {
	// Get date
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	qDebug() << "Current time point obtained: ";

	// Implementation for plotting pie chart
	QVector<Category> categories = categoryRepository.getAllProfileCategories(getProfileId()); // Example profileId
	QVector<Transaction> transactions = transactionRepository.getAllProfileTransaction(getProfileId());
	std::map<int, double> categorySums;

	qDebug() << "Categories retrieved:" << categories.size();
	qDebug() << "Transactions retrieved:" << transactions.size();

	// sum of transaction values per category
	for (auto category : categories) {
		double sum = 0.0;
		for (auto transaction : transactions) {
			if (transaction.getCategoryId() == category.getCategoryId() && transaction.getTransactionType() == "Expense") {
					sum += transaction.getTransactionAmount();
			}
		}
		categorySums.insert({ category.getCategoryId(), sum });
	}
	
	for (const auto& pair : categorySums) {
		qDebug() << "Category ID:" << pair.first << "Sum of Expenses:" << pair.second;
	}
	
	qDebug() << "ChartsController: plotPieChart called.";
}