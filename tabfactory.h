#ifndef TABFACTORY_H
#define TABFACTORY_H
#include "QDockWidget"

class TabFactory
{
public:
    TabFactory();
    static QDockWidget* createTable(int numRows,int numCols);
    static QDockWidget* createGraph();
    static QDockWidget* createBarGraph();
};

#endif // TABFACTORY_H
