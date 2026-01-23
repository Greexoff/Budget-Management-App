/**
 * @file DashboardView.h
 * @brief Header file for the Main Application Window (Dashboard).
 */
#pragma once


#include <QMainWindow>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QStackedWidget> 

 /**
  * @class DashboardView
  * @brief The main container window that manages the sidebar and stacked pages.
  */
class DashboardView : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief Constructs the DashboardView. */
    explicit DashboardView(QWidget* parent = nullptr);

    /** @brief Sets the central widget of the main window. */
    QWidget* setMainWidget();
    /** @brief creates the main horizontal layout. */
    static QHBoxLayout* setMainLayout(QWidget* centralWidget);
    /** @brief Creates the sidebar frame. */
    static QFrame* setSidebarFrame();
    /** @brief Creates the sidebar vertical layout. */
    static QVBoxLayout* setSidebarLayout(QFrame* sidebarFrame);
    /** @brief Adds the application title to the sidebar. */
    static void setAppTitle(QVBoxLayout*& sidebarLayout);

    /** @brief Initializes the map of section buttons (Transaction, Category, etc.). */
    void fillSectionsTabs();

    enum SelectedPage { TransactionPage, CategoryPage, FinancialAccountPage, ChartsPage };

    /**
     * @brief Switches the stacked widget to the requested page.
     * @param selectedPage The enum value of the page to display.
     */
    void navigateToPage(SelectedPage selectedPage);

    /**
     * @brief Replaces a view widget in the stacked widget with a new instance.
     * @param page The page index to replace.
     * @param widget The new widget instance.
     */
    void replaceCurrentWidget(SelectedPage page, QWidget* widget) const;
signals:
    /** @brief Emitted when the user wants to logout/switch profiles. */
    void backToProfileRequested();
    /** @brief Emitted when navigation between pages occurs. */
    void pageChangeRequested(SelectedPage selectedPage);

private:
    QMap<SelectedPage, QPushButton*> sectionsTabs;
    QPushButton* logoutButton;
    QStackedWidget* stackedWidget;
    void addTabsWidgetsToSidebar(QVBoxLayout*& sidebarLayout);
    void setDashboardUi();
    void setupConnections();
    void updateSidebarStyle(SelectedPage selectedPage);
    void setupStyle();
private slots:
    void onButtonChangeProfileClicked();
};