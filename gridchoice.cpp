#include "gridchoice.h"
#include "ui_gridchoice.h"

GridChoice::GridChoice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridChoice)
{
    ui->setupUi(this);
}

GridChoice::~GridChoice()
{
    delete ui;
}
