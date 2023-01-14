#include "SenecDashboard.h"
#include "SenecClient.h"

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
    std::string data = client->getDashboardData();
    QString qdata = QString::fromStdString(data);
    ui.testLabel->setText(qdata);
}
