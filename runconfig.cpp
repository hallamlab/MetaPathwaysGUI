#include "RunConfig.h"
#include "ui_RunConfig.h"

RunConfig::RunConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunConfig)
{
    ui->setupUi(this);
}

RunConfig::~RunConfig()
{
    delete ui;
}
