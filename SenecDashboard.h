#pragma once

#include "ui_SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"
#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

class SenecDashboard : public QMainWindow
{
    Q_OBJECT

public:
    SenecDashboard(QWidget *parent = nullptr);
    ~SenecDashboard();

    bool initializeClient();
    void refreshViews();
    void refreshTrayShowHide();

    void setPowerState();

private:
    PowerState state;

    QSystemTrayIcon* trayIcon;
    QAction* showHideAction;

    Ui::SenecDashboardClass ui;
    SenecClient* client;

    QLabel* stateLabels[6];
    QLabel* arrows[4];

    void setupStateLabels();
    void setupStateIcons();
    void setupArrows();
    void setupTimer();
    void setupIcon();
    void setupTrayMenu();

    void setNoDataView();
    void updateWindow();
    void updateBatteryIcons();
    void updateTrayTooltip();
    void updateArrows();

    void updateGenerationArrow();
    void updateGridArrow();
    void updateBatteryArrow();
    void updateUsageArrow();

private slots:
    void on_refreshButton_clicked();
    void testRead();
    void showHideDashboard();
    void on_actionExit_triggered();
    void on_refreshAction_triggered();
};
