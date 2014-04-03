#include "genebrowser/graphicsrepresentation.h"
#include "ui_graphicsrepresentation.h"
#include <QDebug>
#include <QMessageBox>

GraphicsRepresentation::GraphicsRepresentation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphicsRepresentation)
{
    ui->setupUi(this);


   // graphicsView= this->findChild<QGraphicsView *>("graphicsView");

    graphicsView = new GenomeView;
    graphicsView->show();

    meganView = new MeganView();
    meganView->setDataFromFile("/tmp/megan_tree.tre");
    meganView->show();

    QString s;

}

/*
bool GraphicsRepresentation::eventFilter(QObject *object, QEvent *event){
    gBrowser->eventFilter(object,event);

}
*/
void GraphicsRepresentation::wheelEvent(QWheelEvent* event) {
    gBrowser->wheelEvent(event);
}

GraphicsRepresentation::~GraphicsRepresentation()
{
    delete ui;
}


void GraphicsRepresentation::setupFromFile(const QString &file) {



}
