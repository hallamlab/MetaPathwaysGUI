#include "graphdata.h"
#include "ui_graphdata.h"
#include <QFileDialog>
#include <QDebug>


GraphData::GraphData( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphData)
{
    ui->setupUi(this);

    titleLabel = this->findChild<QLabel *>("titleLabel");
    graph = this->findChild<QCustomPlot *>("graph");
    exportButton = this->findChild<QPushButton *>("exportButton");

    connect(exportButton,SIGNAL(clicked()),this,SLOT(saveImage()));
}

void GraphData::saveImage(){
    QDateTime now;
    bool saved;
    QString path = QFileDialog::getSaveFileName(0,"Select where to save the image.","~","*.png");

    if (!path.isEmpty()){
        saved = graph->savePng(path);
        QMessageBox box;

        if (saved){
            box.warning(0,"Saved!", "Image saved successfully at " + path, QMessageBox::Ok);
        }else{
            box.warning(0,"Failed to Save!", "Image could not be saved to " + path + ", do you have write permissions at that directory?", QMessageBox::Ok);
        }
    }
}


bool GraphData::setFile(QString fileName) {
    this->file = fileName;
}

bool GraphData::prepareInput() {
    TableManager *tableManager = TableManager::getTableManager();

    GRAPHDATA *gdata = tableManager->getGraphData(file);
    if(gdata ==0 )  {
       this->setupFromFile(file);
       gdata = new GRAPHDATA();
       gdata->numBuckets = 500;
       this->computeParams(gdata);
       this->computeHistoGram(gdata);
       tableManager->setGraphData(file, gdata);
    }
    this->plotSomeGraph(this->graph, gdata);
}




void GraphData::computeParams(GRAPHDATA *gdata) {
    double _min= 999999, _max = -999999;
    foreach(double v, graphData) {
        if( v > _max ) _max = v;
        if( v < _min ) _min= v;
    }
    gdata->x.clear();
    gdata->y.clear();
    gdata->xmin= _min;
    gdata->xmax = _max;
    gdata->bucketSize = (gdata->xmax - gdata->xmin)/(double)gdata->numBuckets;
}



void GraphData::setupFromFile(const QString &file){
    graphData.clear();
    QFile inputFile(file);

    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists()){
        QTextStream in(&inputFile);

        while (!in.atEnd()){
            QString line = in.readLine().trimmed();
            QRegExp splitRegex("\\t");

            QStringList splitList = line.split(splitRegex);

            foreach (QString number, splitList){
                bool ok;
                number.toDouble(&ok);
                if (ok) graphData.append(number.toDouble());
            }
        }
        inputFile.close();
    }


}
void GraphData::computeHistoGram(GRAPHDATA *gdata) {
    QVector<double> z;
    for(unsigned int i =0; i < gdata->numBuckets; i++) {
        gdata->y.push_back(0);
        z.push_back(0);
        gdata->x.push_back(gdata->bucketSize*(i));
    }

    unsigned int j;
    foreach(double v, graphData) {
        j = (unsigned int)((v - gdata->xmin)/gdata->bucketSize);
        z[j]++;
    }

    gdata->y[0] = (y[0] + y[1])/2;
    for( j = 1; j < gdata->numBuckets -1; j++) {
        gdata->y[j] = (z[j-1] + z[j] + z[j+1]);
    }
    gdata->y[j] = (z[j-1] + z[j])/2;

    double _ymax = 0;
    foreach(double v, gdata->y){
        if (v > _ymax) _ymax = v;
    }
    gdata->ymax = _ymax;

}



void GraphData::plotSomeGraph(QCustomPlot *customPlot, GRAPHDATA *gdata){
  //  QCPBars *barGraph = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    // QCPCurve *barGraph = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    // create graph and assign data to it:
    customPlot->clearGraphs();
    customPlot->addGraph();
    //customPlot->addPlottable(barGraph);

   // customPlot->graph(0)->setPen(pen);


    customPlot->graph()->setBrush(QBrush(QColor(255,255,255)));
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setData(gdata->x, gdata->y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
 //   qDebug() << "yaxis range " << customPlot->yAxis->range().maxRange << " xaxis range " << customPlot->xAxis->range().minRange << " - " << customPlot->xAxis->range().maxRange;
    customPlot->yAxis->setRange(0,gdata->ymax);
    customPlot->xAxis->setRange(gdata->xmin -1 , gdata->xmax + 1);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    //customPlot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);

}


/*
 * Parses the data points for data.
 */
QList<QStringList>* GraphData::parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT){
    QFile inputFile(file);
    QList<QStringList>* data = new QList<QStringList>();
    bool firstLine = true;

    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while ( !in.atEnd() ){
            QString line = in.readLine().trimmed();
            QStringList lineSplit = line.split(QRegExp("\\t"));
            lineSplit.removeAll("");

            if (!lineSplit.isEmpty()){
                //take lines as they come
                if (firstLine){
                    QStringList *headerList = new QStringList(lineSplit);
//                    headers = headerList;
                    firstLine = false;
                }else {
                    data->append(lineSplit);
                }
            }
        }
    }
    return data;
}

GraphData::~GraphData()
{
    delete this->graph;
    delete ui;
}
