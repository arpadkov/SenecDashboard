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

    //std::string self_sufficency = state.getBatteryState();

    QString self_sufficency = QString::fromStdString(state.getSelfSuffiency());
    //ui.testLabel->setText(qdata);

    QString battery_soc = QString::fromStdString(state.getBatterySOC());

    QString generation = QString::fromStdString(state.getGeneration());
    ui.generationLabel->setText(generation);

    QString grid_state = QString::fromStdString(state.getGridState());
    ui.gridLabel->setText(grid_state);

    QString battery_state = QString::fromStdString(state.getBatteryState());
    ui.batteryLabel->setText(battery_state);

    QString usage = QString::fromStdString(state.getUsage());
    ui.usageLabel->setText(usage);
}
