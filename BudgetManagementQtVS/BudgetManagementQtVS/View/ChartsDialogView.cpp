#include "View/ChartsDialogView.h"

ChartsView::ChartsView(QWidget* parent) : QWidget(parent)
{
    setupUI();
    setupStyle();

    connect(startDateEdit, &QDateEdit::dateChanged, this, &ChartsView::dateRangeChanged);
    connect(endDateEdit, &QDateEdit::dateChanged, this, &ChartsView::dateRangeChanged);

}

void ChartsView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    // 1. Filtry Daty
    QHBoxLayout* filterLayout = new QHBoxLayout();
    startDateEdit = new QDateEdit(QDate::currentDate().addMonths(-1), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("yyyy-MM-dd");
    
    endDateEdit = new QDateEdit(QDate::currentDate(), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("yyyy-MM-dd");

    filterLayout->addStretch();
    filterLayout->addWidget(new QLabel("From:"));
    filterLayout->addWidget(startDateEdit);
    filterLayout->addWidget(new QLabel("To:"));
    filterLayout->addWidget(endDateEdit);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    // 2. Kafelki 
    QHBoxLayout* tilesLayout = new QHBoxLayout();
    auto createTile = [&](QString title, QString color, QLabel** lbl) {
        QFrame* f = new QFrame; f->setObjectName("statTile");
        QVBoxLayout* l = new QVBoxLayout(f);
        QLabel* t = new QLabel(title); t->setObjectName("tileTitle"); t->setAlignment(Qt::AlignCenter);
        *lbl = new QLabel("0.00 PLN"); (*lbl)->setObjectName("tileValue"); (*lbl)->setAlignment(Qt::AlignCenter); (*lbl)->setStyleSheet("color:"+color);
        l->addWidget(t); l->addWidget(*lbl);
        tilesLayout->addWidget(f);
    };
    createTile("Expenses (Period)", "#e74c3c", &expenseValLabel);
    createTile("Income (Period)", "#2ecc71", &incomeValLabel);
    createTile("Avg Expense (All Time)", "#f1c40f", &averageValLabel);
    mainLayout->addLayout(tilesLayout);

    // 3. Wykresy 
    QHBoxLayout* chartsLayout = new QHBoxLayout();


    pieChartView = new QChartView();
    pieChartView->chart()->setTitle("Expenses by Category");
    pieChartView->chart()->setTitleBrush(Qt::white);
    pieChartView->chart()->setBackgroundBrush(QColor("#1e1e1e"));
    pieChartView->chart()->legend()->setLabelColor(Qt::white);
    pieChartView->chart()->legend()->setAlignment(Qt::AlignRight);
    

    barChartView = new QChartView();
    barChartView->chart()->setTitle("Income vs Expenses");
    barChartView->chart()->setTitleBrush(Qt::white);
    barChartView->chart()->setBackgroundBrush(QColor("#1e1e1e"));
    barChartView->chart()->legend()->setLabelColor(Qt::white);
    barChartView->chart()->legend()->setAlignment(Qt::AlignBottom);

    chartsLayout->addWidget(pieChartView, 1);
    chartsLayout->addWidget(barChartView, 1);
    
    mainLayout->addLayout(chartsLayout, 1);
}

void ChartsView::updatePieChart(const QMap<QString, double>& data) {
    QPieSeries *series = new QPieSeries();
    
    if (data.isEmpty()) {
        series->append("No Expenses", 1);
        series->slices().first()->setBrush(Qt::gray);
    } else {
        for (auto it = data.begin(); it != data.end(); ++it) {
            series->append(it.key(), it.value());
        }
    }
    

    for(auto slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::white);
    }

    QChart *chart = pieChartView->chart();
    chart->removeAllSeries();
    chart->addSeries(series);
}

void ChartsView::updateBarChart(double totalIncome, double totalExpense) {
    QBarSet *setIncome = new QBarSet("Income");
    QBarSet *setExpense = new QBarSet("Expenses");

    *setIncome << totalIncome;
    *setExpense << totalExpense;

    setIncome->setColor(QColor("#2ecc71")); 
    setExpense->setColor(QColor("#e74c3c")); 

    QBarSeries *series = new QBarSeries();
    series->append(setIncome);
    series->append(setExpense);

    QChart *chart = barChartView->chart();
    chart->removeAllSeries();
    

    auto axes = chart->axes();
    for(auto axis : axes) chart->removeAxis(axis);

    chart->addSeries(series);


    QStringList categories; categories << "Selected Period";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(Qt::white);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelsColor(Qt::white);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
}


void ChartsView::setupStyle() {
    this->setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: #ecf0f1;
            font-family: 'Segoe UI', sans-serif;
            font-size: 14px;
        }

        QDateEdit {
            background-color: #2d2d2d;
            border: 1px solid #444444;
            border-radius: 6px;
            padding: 6px 12px;
            color: white;
            selection-background-color: #3498db;
        }
        QDateEdit:focus {
            border: 1px solid #3498db; 
        }
        QDateEdit::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 25px;
            border-left-width: 0px;
            border-top-right-radius: 6px;
            border-bottom-right-radius: 6px;
        }
        QDateEdit::down-arrow {
            image: none; 
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #bdc3c7; 
            width: 0;
            height: 0;
            margin-top: 2px;
            margin-right: 8px;
        }

 
        QCalendarWidget QWidget {
            background-color: #1e1e1e;
            color: white;
        }
        QCalendarWidget QToolButton {
            color: white;
            background-color: transparent;
            icon-size: 24px;
            font-weight: bold;
        }
        QCalendarWidget QToolButton:hover {
            background-color: #333333;
            border-radius: 4px;
        }
        /* Nagłówek kalendarza (Dni tygodnia) */
        QCalendarWidget QTableView {
            background-color: #2d2d2d;
            selection-background-color: #3498db;
            color: white;
            gridline-color: #333333;
            alternate-background-color: #2d2d2d;
        }
        QCalendarWidget QSpinBox {
            background-color: #2d2d2d;
            color: white;
            selection-background-color: #3498db; 
        }


        QFrame#statTile {
            background-color: #1e1e1e;  
            border: 1px solid #333333;
            border-radius: 12px;         
        }
        QFrame#statTile:hover {
            background-color: #252525;   
            border: 1px solid #444444;
        }

        QLabel#tileTitle {
            font-size: 13px;
            color: #95a5a6;             
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 1px;
            padding-bottom: 5px;
        }

        QLabel#tileValue {
            font-size: 24px;
            font-weight: bold;
            padding-bottom: 5px;
        }

        QLabel {
            color: #bdc3c7;
            font-weight: 500;
        }
    )");
}
void ChartsView::setIncomeValue(double v) { incomeValLabel->setText(QString::number(v, 'f', 2)); }
void ChartsView::setExpenseValue(double v) { expenseValLabel->setText(QString::number(v, 'f', 2)); }
void ChartsView::setAverageValue(double v) { averageValLabel->setText(QString::number(v, 'f', 2)); }

