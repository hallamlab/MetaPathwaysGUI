#include "gridsetup.h"
#include "ui_gridsetup.h"

GridSetup::GridSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridSetup)
{
    ui->setupUi(this);
}

GridSetup::~GridSetup()
{
    delete ui;
}
