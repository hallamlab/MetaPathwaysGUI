#ifndef GENOMEVIEW_H
#define GENOMEVIEW_H

#include <QGraphicsView>
#include <QTableWidgetItem>
#include <QObject>
#include <QGraphicsObject>
#include "helper/types.h"


class GeneBrowser;

class GenomeView :  public QGraphicsView
{
    Q_OBJECT
public:
    GenomeView();
    void showData(GeneBrowserData &gbdata);

public slots:
     void itemGenomeDisplay(QTableWidgetItem * item );

private:
     virtual void wheelEvent(QWheelEvent* event);

    GeneBrowser *gBrowser;
    unsigned int XSIZE;
    unsigned int YSIZE;


};

#endif // GENOMEVIEW_H
