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


public slots:
	void setupChartsWindow();



private:
	TransactionRepository& transactionRepository;   ///< Reference to the transaction repository
	CategoryRepository& categoryRepository;         ///< Reference to the category repository	
	ChartsDialogView& chartsDialogView;           ///< Reference to the charts dialog view
};