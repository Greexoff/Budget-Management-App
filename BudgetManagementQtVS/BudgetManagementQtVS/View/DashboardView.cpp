/**
 * @file DashboardView.cpp
 * @brief Implementation of the Dashboard Main Window.
 */
#include "View/DashboardView.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QFrame>

 /**
  * @brief Constructor. Sets window properties and builds the dashboard UI.
  */
DashboardView::DashboardView(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Budget Manager Application");
    resize(1200, 800);
    setDashboardUi();
    setupStyle();
    setupConnections();
}
/** @brief Creates and sets the central widget. */
QWidget* DashboardView::setMainWidget()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    return centralWidget;
}
/** @brief Initializes the main horizontal layout. */
QHBoxLayout* DashboardView::setMainLayout(QWidget* centralWidget)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    return mainLayout;
}
/** @brief Configures the sidebar frame geometry. */
QFrame* DashboardView::setSidebarFrame()
{
    QFrame* sidebarFrame = new QFrame();
    sidebarFrame->setObjectName("sidebarFrame");
    sidebarFrame->setFixedWidth(250);
    return sidebarFrame;
}
/** @brief Configures the sidebar layout margins. */
QVBoxLayout* DashboardView::setSidebarLayout(QFrame* sidebarFrame)
{
	QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebarFrame);
    sidebarLayout->setContentsMargins(0, 20, 0, 20);
    sidebarLayout->setSpacing(10);
    return sidebarLayout;
}
/** @brief Adds the 'Budget Management' label to the sidebar. */
void DashboardView::setAppTitle(QVBoxLayout* &sidebarLayout)
{
    QLabel* appTitle = new QLabel("Budget Management");
    appTitle->setObjectName("appTitle");
    appTitle->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addSpacing(30);
}
/** @brief Maps page enums to their respective navigation buttons. */
void DashboardView::fillSectionsTabs()
{
    sectionsTabs = {
      {TransactionPage, new QPushButton("Transaction")},
      {CategoryPage, new QPushButton("Categories")},
      {FinancialAccountPage, new QPushButton("Financial Accounts")},
      {ChartsPage, new QPushButton("Charts")},
    };
}
/** @brief Adds navigation buttons and the logout button to the sidebar layout. */
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
/** @brief Orchestrates the construction of the entire Dashboard UI. */
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
/**
 * @brief Removes the old widget at the specified index and inserts the new one.
 */
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
/** @brief Connects navigation buttons to the navigateToPage slot. */
void DashboardView::setupConnections()
{
    for (auto const& [page, navigationButton] : sectionsTabs.asKeyValueRange())
    {
        connect(navigationButton, &QPushButton::clicked, [this, page]() { navigateToPage(page); });
    }
    connect(logoutButton, &QPushButton::clicked, this, &DashboardView::onButtonChangeProfileClicked);
}
/**
 * @brief Changes the current index of the QStackedWidget and updates sidebar styling.
 */
void DashboardView::navigateToPage(SelectedPage selectedPage)
{
    stackedWidget->setCurrentIndex(selectedPage);
    updateSidebarStyle(selectedPage);
    emit pageChangeRequested(selectedPage);
}
/**
 * @brief Updates the active property of sidebar buttons to highlight the current page.
 */
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
/** @brief Sets global styles for the dashboard and sidebar. */
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
/** @brief Emits the signal to return to the profile view. */
void DashboardView::onButtonChangeProfileClicked() { emit backToProfileRequested(); }
