#include "Controller/DashboardController.h"

DashboardController::DashboardController(QObject* parent) : BaseController(parent) {
    dashboardView = new DashboardView();
    if (dashboardView) {
        dashboardView->setAttribute(Qt::WA_DeleteOnClose);
    	connect(dashboardView, &DashboardView::backToProfileRequested, this, &DashboardController::logoutRequested);
    }
}
DashboardController::~DashboardController()
{
    if (dashboardView)
    {
        dashboardView->close();
    }
}
void DashboardController::run() {
    if (dashboardView) {
        dashboardView->show();
    }
}