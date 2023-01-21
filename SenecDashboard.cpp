# include <QTimer>
#include <QMessageBox>
#include "SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"
#include "Exceptions.h"


SenecDashboard::SenecDashboard(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this))
{
    ui.setupUi(this);

    client = new SenecClient();

    setupStateLabels();
    setupStateIcons();
    setupArrows();

    setupTimer();
    setupIcon();

    setupTrayMenu();
}

SenecDashboard::~SenecDashboard()
{}

void SenecDashboard::setupStateLabels()
{
    // Assigning labels to array for easier handling
    stateLabels[0] = ui.timeStampLabel;
    stateLabels[1] = ui.battery_SOCLabel;
    stateLabels[2] = ui.generationLabel;
    stateLabels[3] = ui.gridLabel;
    stateLabels[4] = ui.batteryLabel;
    stateLabels[5] = ui.usageLabel;

    for (QLabel* label : stateLabels)
    {
        ui.gridLayout->setAlignment(label, Qt::AlignCenter);
    }

    ui.gridLayout->setAlignment(ui.generationLabel, Qt::AlignBottom);
}

void SenecDashboard::setupStateIcons()
{
    ui.gridIcon->setPixmap(QPixmap(":/consumers/resources/grid.png"));
    ui.gridIcon->setScaledContents(true);
    ui.gridIcon->setFixedSize(QSize(80, 80));

    ui.batteryIcon->setPixmap(QPixmap(":/battery_icon/resources/battery_empty.png"));
    ui.batteryIcon->setScaledContents(true);
    ui.batteryIcon->setFixedSize(QSize(80, 80));

    ui.consumptionIcon->setPixmap(QPixmap(":/consumers/resources/home.png"));
    ui.consumptionIcon->setScaledContents(true);
    ui.consumptionIcon->setFixedSize(QSize(80, 80));

    ui.generationIcon->setPixmap(QPixmap(":/consumers/resources/solar_panel.png"));
    ui.generationIcon->setScaledContents(true);
    ui.generationIcon->setFixedSize(QSize(80, 80));
}

void SenecDashboard::setupArrows()
{
    // Assigning arrows to array for easier handling
    arrows[0] = ui.generationArrow;
    arrows[1] = ui.gridArrow;
    arrows[2] = ui.batteryArrow;
    arrows[3] = ui.usageArrow;

    for (QLabel* arrow : arrows)
    {
        ui.gridLayout->setAlignment(arrow, Qt::AlignCenter);
        arrow->setScaledContents(true);
    }
}

void SenecDashboard::setupTimer()
{
    // Setting up timer, connected to refreshButton click signal
    // Refreshes every 5 minutes

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SenecDashboard::on_refreshButton_clicked);
    timer->start(300000);
}

void SenecDashboard::setupIcon()
{
    // App + Tray Icon icon
    auto appIcon = QIcon(":/battery_icon/resources/battery_empty.png");
    this->setWindowIcon(appIcon);
    this->trayIcon->show();

    ui.refreshButton->setIcon(QIcon(":/logos/resources/refresh_logo.png"));
    ui.refreshButton->setIconSize(QSize(24, 24));
    ui.refreshButton->setFlat(true);
}

void SenecDashboard::setupTrayMenu()
{
    
    QMenu* trayMenu = new QMenu();

    // Show/Hide action text changed from outside on startup
    showHideAction = new QAction(this);
    connect(showHideAction, &QAction::triggered, this, &SenecDashboard::showHideDashboard);

    // Exit action
    QAction* exitAction = new QAction("Close Dashboard", this);
    connect(exitAction, &QAction::triggered, this, &SenecDashboard::on_actionExit_triggered);
    exitAction->setIcon(QIcon(":/logos/resources/close_logo.png"));

    // Refresh action
    QAction* refreshAction = new QAction("Refresh Dashboard", this);
    connect(refreshAction, &QAction::triggered, this, &SenecDashboard::on_refreshAction_triggered);
    refreshAction->setIcon(QIcon(":/logos/resources/refresh_logo.png"));

    trayMenu->addAction(refreshAction);
    trayMenu->addAction(showHideAction);
    trayMenu->addAction(exitAction);    

    trayIcon->setContextMenu(trayMenu);

    
}

bool SenecDashboard::initializeClient()
{
    try
    {
        client->setAuthToken("\\login_data.json");
        client->setBatteryId();
        return true;
    }
    catch (const std::exception& ex)
    {
        QMessageBox warningMessageBox;
        warningMessageBox.setText(ex.what());
        warningMessageBox.exec();
        return false;
    }
}

void SenecDashboard::refreshViews()
{
    setNoDataView();

    if (client->Initialized)
    {
        PowerState state = client->getDashboardData();

        updateWindow(&state);
        updateBatteryIcons(&state);
        updateTrayTooltip(&state);
        updateArrows(&state);
    }
    else if (initializeClient())
    {
        PowerState state = client->getDashboardData();

        updateWindow(&state);
        updateBatteryIcons(&state);
        updateTrayTooltip(&state);
        updateArrows(&state);
    }
    else
    {
        QMessageBox warningMessageBox;
        warningMessageBox.setText("SenecClient not initialized. Will not refresh data");
        warningMessageBox.exec();
    }
}

void SenecDashboard::setNoDataView()
{
    // Set Labels
    for (QLabel* label : stateLabels)
    {
        label->setText("-");
    }
    ui.timeStampLabel->setText("Could not refresh data");

    // Set Arrows
    for (QLabel* arrow : arrows)
    {
        arrow->hide();
    }

    // Set TrayTooltip
    QString tooltip = QString("Could not get data");
    this->trayIcon->setToolTip(tooltip);


    // Set TrayIcon
    this->trayIcon->setIcon(QIcon(":/battery_icon/resources/battery_empty.png"));

}

void SenecDashboard::refreshViews(PowerState* state)
{
    // Refreshes the view from test file
    // IMPORTANT: must call the same methods as refreshViews()

    updateWindow(state);
    updateBatteryIcons(state);
    updateTrayTooltip(state);
    updateArrows(state);
}

void SenecDashboard::updateWindow(PowerState* state)
{
    QString self_sufficency = QString::fromStdString(state->getSelfSuffiency());

    QString timestamp = QString::fromStdString(state->getTimeStamp());
    ui.timeStampLabel->setText(timestamp);

    QString battery_soc = QString::fromStdString(state->getBatterySOC());
    ui.battery_SOCLabel->setText(battery_soc);

    QString generation = QString::fromStdString(state->getGeneration());
    ui.generationLabel->setText(generation);

    QString grid_state = QString::fromStdString(state->getGridState());
    ui.gridLabel->setText(grid_state);

    QString battery_state = QString::fromStdString(state->getBatteryState());
    ui.batteryLabel->setText(battery_state);

    QString usage = QString::fromStdString(state->getUsage());
    ui.usageLabel->setText(usage);
}

void SenecDashboard::updateBatteryIcons(PowerState* state)
{
    const char* batteryIconPath = state->getBatteryIconPath();
    QIcon batteryIcon = QIcon(batteryIconPath);

    trayIcon->setIcon(batteryIcon);
    this->setWindowIcon(batteryIcon);
    ui.batteryIcon->setPixmap(QPixmap(batteryIconPath));
}

void SenecDashboard::updateTrayTooltip(PowerState* state)
{
    QString tooltip = QString("");
    tooltip.append("Battery: " + QString::fromStdString(state->getBatterySOC()) + "\n");
    tooltip.append("Grid usage: " + QString::fromStdString(state->getGridState()) + "\n");
    tooltip.append("Battery usage: " + QString::fromStdString(state->getBatteryState()) + "\n");
    tooltip.append("Consumption: " + QString::fromStdString(state->getUsage()) + "\n");
    tooltip.append("Production: " + QString::fromStdString(state->getGeneration()) + "\n");

    this->trayIcon->setToolTip(tooltip);
}

void SenecDashboard::updateArrows(PowerState* state)
{
    updateGenerationArrow(state);
    updateGridArrow(state);
    updateBatteryArrow(state);
    updateUsageArrow(state);

    for (QLabel* arrow : arrows)
    {
        arrow->show();
    }
}

void SenecDashboard::updateGenerationArrow(PowerState* state)
{
    float utilization = state->getGenerationUtilization();

    ui.generationArrow->setPixmap(QPixmap(":/arrows/resources/arrow_down.png"));
    ui.generationArrow->setFixedHeight(80);

    // Calculating width based on utilization
    int max_width = 100;
    float width = max_width * utilization;

    ui.generationArrow->setFixedWidth(width);
}

void SenecDashboard::updateGridArrow(PowerState* state)
{
    float utilization = state->getGridUtilization();

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

    ui.usageArrow->setPixmap(QPixmap(":/arrows/resources/arrow_down.png"));
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

void SenecDashboard::showHideDashboard()
{
    if (this->isVisible())
    {
        this->hide();
    }
    else
    {
        this->show();
    }

    refreshTrayShowHide();
}

void SenecDashboard::refreshTrayShowHide()
{
    if (this->isVisible())
    {
        showHideAction->setText("Hide Dashboard");        
        showHideAction->setIcon(QIcon(":/logos/resources/hide_logo.png"));
    }
    else
    {
        showHideAction->setText("Show Dashboard");
        showHideAction->setIcon(QIcon(":/logos/resources/show_logo.png"));
    }
}

void SenecDashboard::on_refreshAction_triggered()
{
    refreshViews();
}

void SenecDashboard::on_actionExit_triggered() {
    QApplication::quit();
}
