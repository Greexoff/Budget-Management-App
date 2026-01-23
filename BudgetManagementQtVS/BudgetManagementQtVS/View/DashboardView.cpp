#include "View/DashboardView.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QFrame>


DashboardView::DashboardView(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Budget Manager Application");
    resize(1200, 800);
    setDashboardUi();
    setupStyle();
    setupConnections();
}

QWidget* DashboardView::setMainWidget()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    return centralWidget;
}

QHBoxLayout* DashboardView::setMainLayout(QWidget* centralWidget)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    return mainLayout;
}

QFrame* DashboardView::setSidebarFrame()
{
    QFrame* sidebarFrame = new QFrame();
    sidebarFrame->setObjectName("sidebarFrame");
    sidebarFrame->setFixedWidth(250);
    return sidebarFrame;
}

QVBoxLayout* DashboardView::setSidebarLayout(QFrame* sidebarFrame)
{
	QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 20, 0, 20);
    sidebarLayout->setSpacing(10);
    return sidebarLayout;
}

void DashboardView::setAppTitle(QVBoxLayout* &sidebarLayout)
{
    QLabel* appTitle = new QLabel("Budget Management");
    appTitle->setObjectName("appTitle");
    appTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addSpacing(30);
}

void DashboardView::fillSectionsTabs()
{
    sectionsTabs = {
      {TransactionPage, new QPushButton("Transaction")},
      {CategoryPage, new QPushButton("Categories")},
      {FinancialAccountPage, new QPushButton("Financial Accounts")},
      {ChartsPage, new QPushButton("Charts")},
    };
}

void DashboardView::addTabsWidgetsToSidebar(QVBoxLayout* &sidebarLayout)
{
    for (auto const& [page, navigationButton] : sectionsTabs.asKeyValueRange())
    {
        navigationButton->setObjectName("navButton");
        sidebarLayout->addWidget(navigationButton);
        if (page == TransactionPage)
        {
            navigationButton->setProperty("active", true);
        }
    }
    sidebarLayout->addStretch();
    logoutButton = new QPushButton("Switch profile");
    logoutButton->setObjectName("navButton");
    sidebarLayout->addWidget(logoutButton);
}

void DashboardView::setDashboardUi()
{
    QWidget* centralWidget = setMainWidget();
    QHBoxLayout* mainLayout = setMainLayout(centralWidget);
    QFrame* sidebarFrame = setSidebarFrame();
    QVBoxLayout* sidebarLayout = setSidebarLayout(sidebarFrame);
    setAppTitle(sidebarLayout);

    fillSectionsTabs();
    addTabsWidgetsToSidebar(sidebarLayout);

    stackedWidget = new QStackedWidget();
    for (auto const& page : sectionsTabs.keys())
    {
        stackedWidget->addWidget(new QWidget());
    }
    mainLayout->addWidget(sidebarFrame);
    mainLayout->addWidget(stackedWidget);

}

void DashboardView::replaceCurrentWidget(SelectedPage page, QWidget* widget) const
{
    if (!widget)
    {
        return;
    }
    QWidget* oldWidget = stackedWidget->widget(page);
    if (oldWidget == widget) return;
    stackedWidget->removeWidget(oldWidget);
    if (oldWidget)
    {
        oldWidget->deleteLater();
    }
    stackedWidget->insertWidget(page, widget);
    stackedWidget->setCurrentIndex(page);
    widget->show();
    stackedWidget->update();
}

void DashboardView::setupConnections()
{
    for (auto const& [page, navigationButton] : sectionsTabs.asKeyValueRange())
    {
        connect(navigationButton, &QPushButton::clicked, [this, page]() { navigateToPage(page); });
    }
    connect(logoutButton, &QPushButton::clicked, this, &DashboardView::onButtonChangeProfileClicked);
}

void DashboardView::navigateToPage(SelectedPage selectedPage)
{
    stackedWidget->setCurrentIndex(selectedPage);
    updateSidebarStyle(selectedPage);
    emit pageChangeRequested(selectedPage);
}

void DashboardView::updateSidebarStyle(SelectedPage selectedPage)
{
    for (auto const& [page, navigationButton] : sectionsTabs.asKeyValueRange())
    {
        const bool isActive = (page == selectedPage);
        navigationButton->setProperty("active", isActive);
        navigationButton->style()->unpolish(navigationButton);
		navigationButton->style()->polish(navigationButton);
    }
}
void DashboardView::setupStyle()
{
    this->setStyleSheet(
        "QMainWindow { background-color: #121212; color: white; }"
        "QWidget { color: white; }"
        "QFrame#sidebarFrame { background-color: #1e1e1e; border-right: 1px solid #333333; }"
        "QLabel#appTitle { color: white; font-size: 20px; font-weight: bold; margin-bottom: 20px; }"
        "QPushButton#navButton { text-align: left; padding: 15px 30px; border: none; color: #b0bec5; font-size: 14px; background: transparent; }"
        "QPushButton#navButton:hover { background-color: #333333; color: white; }"
        "QPushButton#navButton[active='true'] { text-align: left; padding: 15px 30px; border: none; color: white; font-size: 14px; background-color: #2c3e50; border-left: 5px solid #3498db; }"
    );
}
void DashboardView::onButtonChangeProfileClicked() { emit backToProfileRequested(); }
