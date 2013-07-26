#include "ec2grid.h"
#include "ui_ec2grid.h"

EC2Grid::EC2Grid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EC2Grid)
{
    ui->setupUi(this);
}

EC2Grid::~EC2Grid()
{
    delete ui;
}
