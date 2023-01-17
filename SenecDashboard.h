#pragma once

#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui_SenecDashboard.h"
#include "SenecClient.h"
#include "PowerState.h"

class SenecDashboard : public QMainWindow
{
    Q_OBJECT

public:
    SenecDashboard(QWidget *parent = nullptr);
    ~SenecDashboard();

    void initializeClient();
    void refreshViews();

private:
    QSystemTrayIcon* trayIcon;

    Ui::SenecDashboardClass ui;
    SenecClient* client;

    void refreshViews(PowerState*);     // For reading in test JSON response

    void updateWindow(PowerState*);
    void updateTrayIcon(PowerState*);
    void updateTrayTooltip(PowerState*);
    void updateArrows(PowerState*);

    void updateGenerationArrow(PowerState*);
    void updateGridArrow(PowerState*);
    void updateBatteryArrow(PowerState*);
    void updateUsageArrow(PowerState*);

private slots:
    void on_refreshButton_clicked();
    void testRead();
};
