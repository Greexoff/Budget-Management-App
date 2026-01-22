#pragma once
#include "Controller/BaseController.h"
#include "View/DashboardView.h"
#include <QPointer>

class DashboardController : public BaseController {
    Q_OBJECT
public:
    explicit DashboardController(QObject* parent = nullptr);
    void run() override;
    ~DashboardController() override;
signals:
    void logoutRequested();

private:
    QPointer<DashboardView> dashboardView;
};