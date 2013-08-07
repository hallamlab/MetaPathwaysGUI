#ifndef GRID_H
#define GRID_H

#include <QString>
#include <QHash>
#include "ec2grid.h"
#include "nonec2.h"

class EC2Grid;
class NonEC2;

class Grid
{
public:
    Grid();
    Grid(QHash<QString,QString>* values, bool enabled);
    QHash<QString,QString> *values;

    EC2Grid* ec2;
    NonEC2* nonec2;

    bool enabled;
    bool newGrid;
    bool deleted;
    QString name;
};

#endif // GRID_H
