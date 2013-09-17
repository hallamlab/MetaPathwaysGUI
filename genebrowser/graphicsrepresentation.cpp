#include "genebrowser/graphicsrepresentation.h"
#include "ui_graphicsrepresentation.h"
#include <QDebug>
#include <QMessageBox>

GraphicsRepresentation::GraphicsRepresentation(QWidget *parent) :
    QWidget(parent),  MetaWidget(),
    ui(new Ui::GraphicsRepresentation)
{
    ui->setupUi(this);


   // graphicsView= this->findChild<QGraphicsView *>("graphicsView");

    graphicsView = new GenomeView;
    graphicsView->show();

    MeganViewer *megan = new MeganViewer;

    QFile file("/tmp/megan_tree.tre");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString  data = in.readAll();
        megan->setData(data);
        megan->createTreeView();
    }
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
