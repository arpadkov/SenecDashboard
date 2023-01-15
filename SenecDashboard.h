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



private:
    QSystemTrayIcon* trayIcon;

    Ui::SenecDashboardClass ui;
    SenecClient* client;

    void refreshViews();
    void updateWindow(PowerState*);
    void updateTrayIcon(PowerState*);
    void updateTrayTooltip(PowerState*);
    void updateArrows(PowerState*);

    void updateGenerationArrow(PowerState*);
    void updateGridArrow(PowerState*);
    void updateBatteryArrow(PowerState*);
    void updateUsageArrow(PowerState*);

    void refreshViews(PowerState*);     // For reading in test JSON response



private slots:
    void on_refreshButton_clicked();
    void testRead();
};
