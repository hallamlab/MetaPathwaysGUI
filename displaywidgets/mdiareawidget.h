#ifndef MDIAREAWIDGET_H
#define MDIAREAWIDGET_H
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QHash>
#include <QDebug>
#include "datamodel/htabledata.h"

class HTableData;

class MdiAreaWidget
{
public:
    static MdiAreaWidget *getMdiAreaWidget();
    void setMdiArea(QMdiArea *mdi);
    QMdiArea *getMdiArea();

    void addWidget(HTableData *);
    void removeWidget(HTableData *);
private:
    MdiAreaWidget();
    static MdiAreaWidget* mdiAreaWidget;
    QMdiArea *mdiArea;
    QHash<HTableData *,bool> subWindows;
};

#endif // MDIAREAWIDGET_H
