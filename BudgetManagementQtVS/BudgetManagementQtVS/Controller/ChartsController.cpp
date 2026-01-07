#include <Controller/ChartsController.h>
#include <QDEBUG>

ChartsController::ChartsController(TransactionRepository& transactionRepoRef, CategoryRepository& categoryRepoRef, ChartsDialogView& chartsDialogViewRef, QObject* parent)
	: BaseController(parent), transactionRepository(transactionRepoRef), categoryRepository(categoryRepoRef), chartsDialogView(chartsDialogViewRef) {
	qDebug() << "ChartsController: Constructor called.";
}

////--------------------------Setting up view---------------------------------------------
//void ChartsController::setupChartsView() {
//	//plotPieChart();
//	//plotBarChart();
//	qDebug() << "ChartsController: setupChartsView called.";
//	chartsDialogView.exec();
//}