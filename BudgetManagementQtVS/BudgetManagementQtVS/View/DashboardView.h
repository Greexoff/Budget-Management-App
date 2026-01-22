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

class DashboardView : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardView(QWidget* parent = nullptr);
    QWidget* setMainWidget();
    static QHBoxLayout* setMainLayout(QWidget* centralWidget);
    static QFrame* setSidebarFrame();
    static QVBoxLayout* setSidebarLayout(QFrame* sidebarFrame);
    static void setAppTitle(QVBoxLayout*& sidebarLayout);
    void fillSectionsTabs();

	signals:
        void backToProfileRequested();

private:
    enum SelectedPage
    {
        DashboardPage,
        TransactionPage,
        CategoryPage,
        FinancialAccountPage,
        ChartsPage
    };
    QMap<SelectedPage, QPushButton*> sectionsTabs;
    QPushButton* logoutButton;
    QStackedWidget* stackedWidget;

    void addTabsWidgetsToSidebar(QVBoxLayout*& sidebarLayout);
    void setDashboardUi();
    void setupConnections();
    void navigateToPage(SelectedPage selectedPage);
    void updateSidebarStyle(SelectedPage selectedPage);
    void setupStyle();
private slots:
    void onButtonChangeProfileClicked();
};