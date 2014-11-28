#include "statsqgroupbox.h"

StatsQGroupBox::StatsQGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
}


void StatsQGroupBox::setModel(RunDataStats *model) {
    this->model = model;
    this->statsTableView->setModel(this->model);
}
