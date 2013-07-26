#include "grid.h"

Grid::Grid()
{
    this->values = new QHash<QString,QString>();
    this->enabled = true;
}

