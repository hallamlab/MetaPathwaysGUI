  #ifndef MEGANDATA_H
#define MEGANDATA_H
#include <QString>
#include <QList>
#include <QStringList>
#include <QChar>
#include <QRect>
#include <QStack>
#include <QHash>
#include <QGraphicsWidget>
#include <QEvent>
#include "genebrowser/graphicsitemscollection.h"
#include "types.h"

#include <QGraphicsView>


typedef struct _TREENODE  TREENODE;

typedef struct _TREENODE {
    QList<TREENODE *> children;
    QString name;
    unsigned int depth;
    double Yup, Ydown;
    int leafcount;
    unsigned int count;
} TREENODE;


//typedef enum { MANHATTAN, STRAIGHT } LineStyle;
class GraphicsTaxonItem;
class GraphicsItemsCollection;
class GraphicsConnectorLines;


class MeganData

{
public:
    MeganData();
    ~MeganData();


    void setData(const QString &data);
    void setOffset(double startX, double stattY);
    void setInitLineLength(double length);
    void setScale(double scale);
    void setScaleY(double yscale);
    void setDeltaY(double deltaY);
    QRect getGeometry(unsigned int depth);
    void createTreeView();
    void createTaxonItems( QHash<unsigned int, QList< GraphicsTaxonItem *> > &txons, GraphicsItemsCollection *itemsCreator, double deltaX, double deltaY, QBrush &taxonBrush, QPen &taxonPen);
    void createConnectingLines(GraphicsTaxonItem *root,  GraphicsItemsCollection* itemCreator, QPen connectorPen);
    void createTaxonNames(GraphicsTaxonItem *pitem, GraphicsItemsCollection* itemCreator, QPen namePen);

    unsigned int getDepth() ;
    unsigned int getHeight();
    double spanAtDepth(unsigned int depth);
    void adjustDepth(TREENODE *node, unsigned int maxdepth);
    void computeBounds( unsigned int maxDepth);
    void synchronizeNodeAndTaxonItems(TREENODE *node, GraphicsTaxonItem *taxons, unsigned int currDepth, unsigned int maxDepth);
    void synchronizeConnectingLines(GraphicsTaxonItem *taxon, unsigned int currDepth, unsigned int maxDepth, LineStyle style);
    void synchronizeTaxonNames(GraphicsTaxonItem *taxon, unsigned int currDepth, unsigned int maxDepth);
    TREENODE *getRoot() { return root;}
    void unscaleVertically(GraphicsTaxonItem *taxon, double unscale, unsigned int currDepth, unsigned int maxDepth);

    void setStyleVisible(GraphicsTaxonItem *taxon, LineStyle style, bool visible, unsigned int currDepth, unsigned int maxDepth);

    void unscale(GraphicsTaxonItem *taxon, QGraphicsView *view, unsigned int currDepth, unsigned int maxDepth, double scalefactor =1);

    void setLineStyle(LineStyle lineStyle);
    void setConnectorPen(QPen *qpen);

    double getUpperBoundY();
    double getLowerBoundY();
    double getLeftBoundX();
    double getRightBoundX();

private:
    void setLines(GraphicsConnectorLines *lines, LineStyle style, GraphicsTaxonItem *a, GraphicsTaxonItem *b);

    QStringList splitByDelimiter(const QString &str, const QChar &sep, bool removeEmptyStrings = true);
    QStringList splitByDelimiter(const QStringList &strlist, const QChar &sep);
    void _createConnectingLines(GraphicsTaxonItem *root);
    void _computeBounds(TREENODE *node, int &leafcounter, unsigned int currDepth, unsigned int maxDepth);
    void _adjustDepth(TREENODE *node, unsigned int depth);
    double _spanAtDepth(TREENODE *node, unsigned int depth, double *Yup, double *Ydown);

    void createTreeStructure(QStringList &tokens);
    TREENODE *createTree(QStringList &tokens);
    void tokenize(QStringList &tokens);
    void printTree(TREENODE *root, int i);
    void addAttributes(TREENODE *p, QString str) ;
    void drawMeganTree();
    void computeCoordinates(double deltaX, double deltaY);
    unsigned int treeTraversal(TREENODE *node, int &leafcounter, unsigned int currDepth, unsigned int maxDepth = 100);
    void _createTaxonItems(TREENODE *node,  QHash<unsigned int, QList< GraphicsTaxonItem *> > &taxons,  GraphicsTaxonItem *parentItem, GraphicsItemsCollection *itemCreator, double deltaX, double deltaY, QBrush &taxonBrush, QPen &taxonPen);
    void _deleteNode(TREENODE *t);



    QString data;
    TREENODE *root;
    double yscale;

    double deltaX, deltaY, YTOP, XRIGHT;
    double STARTX, STARTY;
    double INIT_LINE_LENGTH;
    double scale;
    LineStyle lineStyle;
    QPen connectorPen;



};

#endif // MeganData_H
