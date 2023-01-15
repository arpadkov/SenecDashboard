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

    void refreshViews();
    void updateWindow(PowerState*);
    void updateTray(PowerState*);

    void refreshViews(PowerState*);     // For reading in test JSON response

private:
    QSystemTrayIcon* trayIcon;

    Ui::SenecDashboardClass ui;
    SenecClient* client;

private slots:
    void on_refreshButton_clicked();
    void testRead();
};
