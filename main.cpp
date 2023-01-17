#include "SenecDashboard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    SenecDashboard dashboard;
    dashboard.show();
    dashboard.initializeClient();
    dashboard.refreshViews();
    return application.exec();
}
