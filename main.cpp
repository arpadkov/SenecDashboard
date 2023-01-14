#include "SenecDashboard.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SenecDashboard w;
    w.show();
    return a.exec();
}
