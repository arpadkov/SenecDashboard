#include "SenecDashboard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    bool startWithWindow = true;

    if (argc > 1)
    {
        std::string hidden = argv[1];
        if (hidden == "hidden")
        {
            startWithWindow = false;
        }
    }

    QApplication application(argc, argv);
    SenecDashboard dashboard;

    dashboard.show();
    dashboard.setVisible(startWithWindow);

    dashboard.refreshViews();

    return application.exec();
}
