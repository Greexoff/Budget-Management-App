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
    enum SelectedPage
    {
        TransactionPage,
        CategoryPage,
        FinancialAccountPage,
        ChartsPage
    };
    void navigateToPage(SelectedPage selectedPage);
    void replaceCurrentWidget(SelectedPage page, QWidget* widget) const;
	signals:
        void backToProfileRequested();
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