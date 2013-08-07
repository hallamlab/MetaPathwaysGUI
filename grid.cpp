#include "grid.h"

Grid::Grid()
{
    this->values = new QHash<QString,QString>();
    this->enabled = true;
    this->ec2 = 0;
    this->nonec2 = 0;
    this->deleted = false;
}

Grid::Grid(QHash<QString,QString> *values, bool enabled){
    this->values = values;
    this->enabled = enabled;
}

