#include "SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"

SenecDashboard::SenecDashboard(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this))
{
    ui.setupUi(this);;

    // App + Tray Icon icon
    auto appIcon = QIcon(":/logos/resources/strava_logo.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    QString tooltip = QString("tooltip");
    this->trayIcon->setToolTip(tooltip);
    this->trayIcon->show();

    client = new SenecClient();

    refreshViews();
}

SenecDashboard::~SenecDashboard()
{}

void SenecDashboard::refreshViews()
{
    PowerState state = client->getDashboardData();
    updateWindow(&state);
    updateTray(&state);
}

void SenecDashboard::refreshViews(PowerState* state)
{
    // Refreshes the view from test file
    updateWindow(state);
    updateTray(state);
}

void SenecDashboard::updateWindow(PowerState* state)
{
    QString self_sufficency = QString::fromStdString(state->getSelfSuffiency());
    QString timestamp = QString::fromStdString(state->getTimeStamp());
    ui.timeStampLabel->setText(timestamp);
    //ui.testLabel->setText(qdata);

    QString battery_soc = QString::fromStdString(state->getBatterySOC());

    QString generation = QString::fromStdString(state->getGeneration());
    ui.generationLabel->setText(generation);

    QString grid_state = QString::fromStdString(state->getGridState());
    ui.gridLabel->setText(grid_state);

    QString battery_state = QString::fromStdString(state->getBatteryState());
    ui.batteryLabel->setText(battery_state);

    QString usage = QString::fromStdString(state->getUsage());
    ui.usageLabel->setText(usage);
}

void SenecDashboard::updateTray(PowerState* state)
{
    // TODO implement update icons
}

void SenecDashboard::on_refreshButton_clicked()
{
    refreshViews();
}

void SenecDashboard::testRead()
{    
    // Reads in test response from "SenecClient/test" and refreshes the view
    PowerState state = client->getTestDashboardData();
    refreshViews(&state);
}
