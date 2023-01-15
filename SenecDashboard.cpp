# include <QTimer>
#include "SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"

SenecDashboard::SenecDashboard(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this))
{
    ui.setupUi(this);;

    // App + Tray Icon icon
    auto appIcon = QIcon(":/battery_icon/resources/battery_full.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    QString tooltip = QString("tooltip");
    this->trayIcon->setToolTip(tooltip);
    this->trayIcon->show();

    client = new SenecClient();

    // Setting up timer, connected to refreshButton click signal
    // Refreshes every 5 minutes
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SenecDashboard::on_refreshButton_clicked);
    timer->start(300000);

    // Aligning labels
    ui.gridLayout->setAlignment(ui.generationLabel, Qt::AlignCenter);
    ui.gridLayout->setAlignment(ui.gridLabel, Qt::AlignCenter);
    ui.gridLayout->setAlignment(ui.batteryLabel, Qt::AlignCenter);
    ui.gridLayout->setAlignment(ui.usageLabel, Qt::AlignCenter);

    refreshViews();
}

SenecDashboard::~SenecDashboard()
{}

void SenecDashboard::refreshViews()
{
    PowerState state = client->getDashboardData();

    updateWindow(&state);
    updateTray(&state);
    updateArrows(&state);
}

void SenecDashboard::refreshViews(PowerState* state)
{
    // Refreshes the view from test file
    updateWindow(state);
    updateTray(state);
    updateArrows(state);
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

void SenecDashboard::updateArrows(PowerState* state)
{
    updateGenerationArrow(state);
    updateGridArrow(state);
    updateBatteryArrow(state);
    updateUsageArrow(state);
}

void SenecDashboard::updateGenerationArrow(PowerState* state)
{
    float utilization = state->getGenerationUtilization();

    // General setting for arrow
    ui.generationArrow->setPixmap(QPixmap(":/arrows/resources/arrow_down.png"));
    ui.generationArrow->setScaledContents(true);
    ui.gridLayout->setAlignment(ui.generationArrow, Qt::AlignCenter);
    ui.generationArrow->setFixedHeight(80);

    // Calculating width based on utilization
    int max_width = 100;
    float width = max_width * utilization;

    ui.generationArrow->setFixedWidth(width);
}

void SenecDashboard::updateGridArrow(PowerState* state)
{
    float utilization = state->getGridUtilization();

    // General setting for arrow
    ui.gridArrow->setScaledContents(true);
    ui.gridLayout->setAlignment(ui.gridArrow, Qt::AlignCenter);
    ui.gridArrow->setFixedWidth(80);

    if (state->drawingFromGrid())
    {
        ui.gridArrow->setPixmap(QPixmap(":/arrows/resources/arrow_right.png"));
    }
    else
    {
        ui.gridArrow->setPixmap(QPixmap(":/arrows/resources/arrow_left.png"));
    }

    // Calculating height based on utilization
    int max_height = 50;
    float height = max_height * utilization;

    ui.gridArrow->setFixedHeight(height);
}

void SenecDashboard::updateBatteryArrow(PowerState* state)
{
    float utilization = state->getBatteryUtilization();

    // General setting for arrow
    ui.batteryArrow->setScaledContents(true);
    ui.gridLayout->setAlignment(ui.batteryArrow, Qt::AlignCenter);
    ui.batteryArrow->setFixedWidth(80);

    if (state->drawingFromBattery())
    {
        ui.batteryArrow->setPixmap(QPixmap(":/arrows/resources/arrow_left.png"));
    }
    else
    {
        ui.batteryArrow->setPixmap(QPixmap(":/arrows/resources/arrow_right.png"));
    }

    // Calculating height based on utilization
    int max_height = 50;
    float height = max_height * utilization;

    ui.batteryArrow->setFixedHeight(height);
}

void SenecDashboard::updateUsageArrow(PowerState* state)
{
    float utilization = state->getUsageUtilization();

    // General setting for arrow
    ui.usageArrow->setPixmap(QPixmap(":/arrows/resources/arrow_down.png"));
    ui.usageArrow->setScaledContents(true);
    ui.gridLayout->setAlignment(ui.usageArrow, Qt::AlignCenter);
    ui.usageArrow->setFixedHeight(80);

    // Calculating width based on utilization
    int max_width = 100;
    float width = max_width * utilization;

    ui.usageArrow->setFixedWidth(width);
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
