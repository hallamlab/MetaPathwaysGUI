#include "nonec2.h"
#include "ui_nonec2.h"

NonEC2::NonEC2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NonEC2)
{
    ui->setupUi(this);
}

NonEC2::~NonEC2()
{
    delete ui;
}
