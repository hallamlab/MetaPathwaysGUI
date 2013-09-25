#include "meganview.h"
#include "ui_meganbrowser.h"

MeganView::MeganView(QWidget *parent):
    ui(new Ui::MeganBrowser)
{
    ui->setupUi(this);
//    this->setDragMode(QGraphicsView::ScrollHandDrag);
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsView = this->findChild<QGraphicsView *>("graphicsView");
    zoomInButton = this->findChild<QPushButton *>("zoomIn");
    zoomOutButton = this->findChild<QPushButton *>("zoomOut");
    spinBox = this->findChild<QSpinBox *>("spinBox");
    lineType = this->findChild<QCheckBox *>("LineType");


    graphicsView->viewport()->installEventFilter(this);
    connectorPen = new QPen( Qt::gray, 2, Qt::SolidLine );
    taxonPen = new QPen( Qt::gray, 1, Qt::SolidLine );
    namePen = new QPen( Qt::blue, 1, Qt::SolidLine );
    taxonBrush = new QBrush(QColor(Qt::green));

    XSIZE = 200;
    YSIZE = 200;
    nameLength = 100;

    this->STARTX = 100;
    this->STARTY = 50;

    this->deltaX = 40;
    this->deltaY = 30;
    this->INIT_LINE_LENGTH = 20;
    this->zoomScale = 1;
    this->depth = 3;
    this->yscale=1;
    meganData = 0;

    if(zoomInButton) {
        connect(zoomInButton, SIGNAL(clicked()), this,  SLOT(zoomIn()));
        connect(zoomOutButton, SIGNAL(clicked()), this,  SLOT(zoomOut()));
        qDebug() << " Connecting buttons ";
    }
    if( lineType ) {
        connect(lineType, SIGNAL(toggled(bool)), this, SLOT(switchStyle(bool)) );
    }

    graphicsView->setGeometry(0,0,1000,1000);

    scene = new QGraphicsScene(0, 0, 1000, 100);
    graphicsView->setScene(scene);

    qDebug() << graphicsView->x() << "  " << graphicsView->y();

    connect(spinBox,SIGNAL(valueChanged(int)),this,SLOT(setDepth(int)));

}


void MeganView::_deleteGraphicsItems(GraphicsTaxonItem *pitem) {
    QList<GraphicsConnectorLines *>::Iterator itline;

    LineStyle styles[2] = { STRAIGHT, MANHATTAN} ;

    for(unsigned int i=0; i < 2; i++)
        for(itline = pitem->Lines[styles[i]].begin(); itline!=pitem->Lines[styles[i]].end(); itline++) {
             delete *itline;
        }

    QList<GraphicsTaxonItem *>::Iterator it;

    for(it = pitem->Children.begin(); it!=pitem->Children.end(); it++) {
        _deleteGraphicsItems(*it);
    }
    delete pitem->Name;
    delete pitem;

}

void MeganView::clearExistingTree() {

    if(meganData ==0) {
        return;
     }
    delete meganData;
    meganData =0;

    this->_deleteGraphicsItems(taxons[0][0]) ;
    scene->clear();
    taxons.clear();
}

void MeganView::setDataFromFile(const QString &fileName) {
      QFile file(fileName);
      meganData = new MeganData;
      if(file.open(QIODevice::ReadOnly)) {
          QTextStream in(&file);
          QString  data = in.readAll();
          meganData->setData(data);
          qDebug() << "b4 create treeview";
          meganData->createTreeView();
          meganData->setInitLineLength(INIT_LINE_LENGTH);

          this->lineStyle = MANHATTAN;
          GraphicsItemsCollection *itemCreator = GraphicsItemsCollection::getGraphicsItemsCollection();
          meganData->createTaxonItems(taxons, itemCreator, this->deltaX, this->deltaY, *taxonBrush);
          meganData->createConnectingLines(taxons[0][0], itemCreator, *connectorPen);
          meganData->createTaxonNames(taxons[0][0], itemCreator, *namePen);
          this->XSIZE = this->deltaX*meganData->getDepth();
          this->YSIZE = this->deltaY*meganData->getHeight();
          meganData->setOffset(STARTX, STARTY);

          this->computePositions();
          scene->setSceneRect(scene->itemsBoundingRect());
          qDebug() << "bounding rect" << scene->itemsBoundingRect();
          scene->update();
          file.close();
      }
}
void MeganView::setDepth(int depth) {
    this->depth = depth;
    this->reDraw(this->zoomScale);
}

void MeganView::addItemsToScene(GraphicsTaxonItem *pitem, QGraphicsScene *_scene, unsigned int currDepth) {

    if( (currDepth < this->depth && pitem->Children.size()==0) || currDepth==this->depth) {
        if(!pitem->Name->inscene) { _scene->addItem(pitem->Name); pitem->Name->inscene = true ;}
    }
    else {
         if(pitem->Name->inscene) { _scene->removeItem(pitem->Name); pitem->Name->inscene = false;}
    }


    if( currDepth <= this->depth ) {
        if(! pitem->inscene ) {_scene->addItem(pitem); pitem->inscene = true;}
    }
    else {
        if(pitem->inscene) { _scene->removeItem(pitem); pitem->inscene = false; }
    }
    QList<GraphicsConnectorLines *>::Iterator itline;
    LineStyle styles[2] = { STRAIGHT, MANHATTAN} ;
    for(unsigned int i=0; i < 2; i++)
        for(itline = pitem->Lines[styles[i]].begin(); itline!=pitem->Lines[styles[i]].end(); itline++) {
            if( currDepth < this->depth && styles[i]==this->lineStyle) {
               if(! (*itline)->inscene ) {_scene->addItem(*itline); (*itline)->inscene = true;}
           }
           else {
               if( (*itline)->inscene) { _scene->removeItem(*itline);  (*itline)->inscene = false; }
           }
        }

    QList<GraphicsTaxonItem *>::Iterator it;

    for(it = pitem->Children.begin(); it!=pitem->Children.end(); it++) {
       this->addItemsToScene(*it, _scene, currDepth+1);
    }


}



void MeganView::_addInitLine() {
    QGraphicsLineItem *line = new QGraphicsLineItem(taxons[0][0]->rect().left() + taxons[0][0]->radius/2 - this->INIT_LINE_LENGTH, taxons[0][0]->rect().top() + taxons[0][0]->radius/2, taxons[0][0]->rect().left()+ taxons[0][0]->radius/2, taxons[0][0]->rect().top() + taxons[0][0]->radius/2);
    line->setToolTip( "Line " + QString::number(taxons[0][0]->rect().left()) + " " +  QString::number(taxons[0][0]->rect().top()) +  " "  +  QString::number(taxons[0][0]->rect().left()) + " " +  QString::number(taxons[0][0]->rect().top()));
    line->setPen( *connectorPen);

    scene->addItem(line);
    //taxons[0][0]->Lines.append(line);
}

void MeganView::computePositions(){
    meganData->computeBounds(this->depth );

    meganData->synchronizeNodeAndTaxonItems(meganData->getRoot(),taxons[0][0], 0 ,this->depth);
    meganData->synchronizeConnectingLines(taxons[0][0], 0,  this->depth, this->lineStyle);
    meganData->synchronizeTaxonNames(taxons[0][0], 0, this->depth);

    this->addItemsToScene(taxons[0][0], scene, 0);
}


void MeganView::scaleDeltaY(double scaleFactor) {
    this->deltaY = this->deltaY*scaleFactor;
}


void MeganView::unscaleVertically(double unscale) {
    meganData->unscaleVertically(taxons[0][0], unscale, 0, this->depth);
    scene->update();
}

void MeganView::reDraw(double scale) {
      meganData->setScale(scale);
      meganData->setDeltaY(this->deltaY);
      this->computePositions();
      scene->update();
      scene->setSceneRect(scene->itemsBoundingRect());
      scene->update();
}


void MeganView::zoomIn() {
    zoomScale *= 1.25;
    this->reDraw(this->zoomScale);
   // scene->update();

}

void MeganView::zoomOut() {
    zoomScale /= 1.25;
    this->reDraw(this->zoomScale);

}


void MeganView::switchStyle(bool value) {
    if(value==true) {
        this->lineStyle = MANHATTAN;
    }
    else {
        this->lineStyle = STRAIGHT;

    }
    this->reDraw(this->zoomScale);
}


void MeganView::setStyleVisible( bool visible) {
    meganData->setStyleVisible(taxons[0][0], this->lineStyle, visible, 0, this->depth);
}

bool MeganView::eventFilter(QObject *object, QEvent *event){

    if (object==graphicsView->viewport() && event->type() == QEvent::Wheel){;
        double scaleFactor = 1.05;

        QWheelEvent *qwe = dynamic_cast<QWheelEvent *>(event);
        if(qwe->delta() > 0) {
            //this->scaleDeltaY(scaleFactor);
            graphicsView->scale(1, scaleFactor);
            this->yscale *= scaleFactor;
            //this->unscaleVertically(1/this->yscale);
        } else {
           // this->scaleDeltaY(1 /scaleFactor);
            graphicsView->scale(1, 1/scaleFactor);
            this->yscale /= scaleFactor;
            //this->unscaleVertically(this->yscale);
        }
        event->accept();

        return true;
    }
    return false;
}

MeganView::~MeganView()
{
    delete ui;
}

