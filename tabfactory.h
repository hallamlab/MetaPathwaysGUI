#ifndef TABFACTORY_H
#define TABFACTORY_H
#include "QDockWidget"

class TabFactory
{
public:
    TabFactory();
    static QWidget* createTable(QString FILE_NAME);
    static QDockWidget* createGraph();
    static QDockWidget* createBarGraph();
};

#endif // TABFACTORY_H
