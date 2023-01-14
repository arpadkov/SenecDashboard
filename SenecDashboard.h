#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SenecDashboard.h"

class SenecDashboard : public QMainWindow
{
    Q_OBJECT

public:
    SenecDashboard(QWidget *parent = nullptr);
    ~SenecDashboard();

private:
    Ui::SenecDashboardClass ui;
};
