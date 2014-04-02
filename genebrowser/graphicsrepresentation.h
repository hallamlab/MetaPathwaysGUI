#ifndef GRAPHICSREPRESENTATION_H
#define GRAPHICSREPRESENTATION_H

#include <QWidget>
#include <QGraphicsLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPointF>
#include <QBrush>
#include "genebrowser/graphicsitemscollection.h"
#include "genebrowser/genomeview.h"
#include "dataviews/meganview.h"



namespace Ui {
class GraphicsRepresentation;
}

class GraphicsRepresentation : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphicsRepresentation(QWidget *parent = 0);
    ~GraphicsRepresentation();
    void setupFromFile(const QString &file);
        
protected:
 //   bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::GraphicsRepresentation *ui;
    virtual void wheelEvent(QWheelEvent* event);

    QGraphicsView *graphicsView;
    GeneBrowser *gBrowser;
    unsigned int XSIZE;
    unsigned int YSIZE;
    MeganView *meganView;
    QString file;
    QLabel* titleLabel;

};

#endif // GRAPHICSREPRESENTATION_H
