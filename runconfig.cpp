#include "runconfig.h"
#include "ui_runconfig.h"

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
