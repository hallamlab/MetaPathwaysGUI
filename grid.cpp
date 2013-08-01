#include "grid.h"

Grid::Grid()
{
    this->values = new QHash<QString,QString>();
    this->enabled = true;
}

Grid::Grid(QHash<QString,QString> *values, bool enabled){
    this->values = values;
    this->enabled = enabled;
}

