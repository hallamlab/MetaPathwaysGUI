#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H
#include "displaywidgets/toolbarwidgets.h"
#include "datamodel/htabledata.h"
#include <QToolBar>
#include <QAction>

class HTableData;

class ToolBarManager: public QObject
{
    Q_OBJECT
public:
    static ToolBarManager *getToolBarManager();
    void setToolBar(QToolBar *toolbar );
    void addTab(HTabWidget *htab, HTableData * htable);
    void removeTab(HTabWidget *htab);
    static ToolBarManager *toolbarmanager;
private:
    ToolBarManager();

    QToolBar *toolbar;
    QHash<HTabWidget *, QAction *> widgets;
    QHash<HTabWidget *, HTableData *> tables;


public slots:
    void tabClicked(HTabWidget *htab, SIGNALTYPE type);
};

#endif // TOOLBARMANAGER_H
