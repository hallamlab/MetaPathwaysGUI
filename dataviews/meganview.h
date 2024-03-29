#ifndef MEGANVIEW_H
#define MEGANVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QMessageBox>
#include <QFileDialog>
#include <QHash>
#include <QGraphicsLineItem>
#include <QPushButton>
#include <QEvent>
#include <QSpinBox>
#include <QCheckBox>


#include "dataviews/megandata.h"
#include "genebrowser/graphicsitemscollection.h"
#include "datamodel/samplercmanager.h"
#include "helper/rundata.h"
#include "popupviews/meganexportbox.h"
#include "displaywidgets/progressview.h"
namespace Ui {
class MeganBrowser;
}
class MeganData;



class MeganView : public QWidget
{
    Q_OBJECT
    
public:
    explicit MeganView(QWidget *parent = 0);

    ~MeganView();
    void setDataFromFile(const QString &file);
    void createTaxonItems();

    void computePositions();

    void reDraw(double scale);
    void scaleDeltaY(double scaleFactor);

    void clearExistingTree() ;
private slots:
    void zoomIn();
    void zoomOut();
    void setDepth(int depth);
    void switchStyle(bool value);
    void exportMeganFile();

private:
    void _deleteGraphicsItems(GraphicsTaxonItem *taxon);
    void _addInitLine();
    void adjustScaleY() ;
    void addItemsToScene(GraphicsTaxonItem *pitem, QGraphicsScene *_scene, unsigned int currDepth);

    bool eventFilter(QObject *object, QEvent *event);
    void setStyleVisible(bool visible);
    void unscale(double scalefactor);


    Ui::MeganBrowser *ui;
   // MeganView *mBrowser;

    MeganData *meganData;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;

    QSpinBox *spinBox;
    QCheckBox *lineType;

    unsigned int XSIZE;
    unsigned int YSIZE;
    unsigned int nameLength;
    double deltaX, deltaY, radius;
    double STARTX, STARTY;
    QHash<unsigned int, QList< GraphicsTaxonItem *> > taxons;
    double INIT_LINE_LENGTH;
    double zoomScale, yscale;
    double depth;
    LineStyle lineStyle;
    QPushButton *zoomInButton, *zoomOutButton, *exportToMegan;

    QPen *connectorPen, *taxonPen, *namePen;
    QBrush *taxonBrush;

};

#endif // MEGANVIEW_H
