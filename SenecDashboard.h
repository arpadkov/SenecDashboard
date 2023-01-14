#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SenecDashboard.h"
#include "SenecClient.h"

class SenecDashboard : public QMainWindow
{
    Q_OBJECT

public:
    SenecDashboard(QWidget *parent = nullptr);
    ~SenecDashboard();
    void updateView();

private:
    Ui::SenecDashboardClass ui;
    int x;
    SenecClient* client;
};
