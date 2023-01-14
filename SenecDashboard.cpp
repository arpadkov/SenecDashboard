#include "SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"

SenecDashboard::SenecDashboard(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    client = new SenecClient();
    updateView();
}

SenecDashboard::~SenecDashboard()
{}

void SenecDashboard::updateView()
{
    PowerState state = client->getDashboardData();

    std::string self_sufficency = state.getBatteryState();

    QString qdata = QString::fromStdString(self_sufficency);
    ui.testLabel->setText(qdata);
}
