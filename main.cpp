#include "SenecDashboard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    SenecDashboard window;
    window.show();
    return application.exec();
}
