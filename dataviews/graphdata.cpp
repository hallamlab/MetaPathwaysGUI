#include "dataviews/graphdata.h"
#include <QFileDialog>
#include "ui_graphdata.h"
#include <QDebug>


GraphData::GraphData( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphData)
{
    ui->setupUi(this);

    titleLabel = this->findChild<QLabel *>("titleLabel");
    graph = this->findChild<QCustomPlot *>("graph");
    exportButton = this->findChild<QPushButton *>("exportButton");
    this->setMinimumSize(200,300);
    this->setMaximumSize(700,900);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

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
    return true;
}

bool GraphData::prepareInput(const QString &xlab, const QString &ylab) {
    TableManager *tableManager = TableManager::getTableManager();

    GRAPHDATA *gdata = tableManager->getGraphData(file);
    if(gdata ==0 )  {
       this->setupFromFile(file);
       gdata = new GRAPHDATA();
       gdata->xlab = xlab;
       gdata->ylab = ylab;
       gdata->xmin = 0;
       gdata->ymin = 0;
       gdata->numBuckets = 200;


       /*double buckets =   static_cast<double>(gdata->x.size())/10.0 > 1.0 ? static_cast<double>(gdata->x.size())/10.0  : 2 ;
       if( gdata->numBuckets > static_cast<int>(buckets) ) gdata->numBuckets = static_cast<int>(buckets);
       qDebug() << gdata->numBuckets;
*/

       this->computeParams(gdata);
       this->computeHistoGram(gdata);
       tableManager->setGraphData(file, gdata);
    }


    this->plotSomeGraph(this->graph, gdata);
    return true;
}




void GraphData::computeParams(GRAPHDATA *gdata) {
    double _min= 1e+20 , _max = -999999;
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

/**
 * @brief GraphData::computeHistoGram this function computes the histogram
 * @param gdata
 */

void GraphData::computeHistoGram(GRAPHDATA *gdata) {

    QVector<double> z;

    double deltaw = (gdata->xmax -gdata->xmin) / gdata->numBuckets;

    for(int i = 0; i < gdata->numBuckets; i++) {
        gdata->x.push_back( gdata->xmin  + (i + 0.5)*deltaw);
        gdata->y.push_back(0);
    }




    unsigned int j;
    foreach(double v, graphData) {
        j = (unsigned int)((v - gdata->xmin)/gdata->bucketSize);
        if(j > gdata->numBuckets - 1) j = gdata->numBuckets -1;
        try{

            gdata->y[j]++;
        }
        catch(...) {

        }
    }


    double _ymax = 0;
    foreach(double v, gdata->y){
        if (v > _ymax) _ymax = v;
    }
    gdata->ymax = _ymax;
}




void GraphData::plotBarGraph(QCustomPlot *customPlot, GRAPHDATA *gdata) {


    QVector<double> x3, y3;

    double deltaw = 0.8*(gdata->xmax -gdata->xmin) / gdata->numBuckets;

    double miny = 1e+20, maxy = -1e+20;
    double wtmean =0, wtsum=0, sumy=0, wtsumsq;

    for(int i =0; i < gdata->x.size(); i++ )  {
        wtsum += gdata->x.at(i)*gdata->y.at(i);
        wtsumsq += gdata->x.at(i)*gdata->x.at(i)*gdata->y.at(i);
        sumy += gdata->y.at(i);
    }
    wtsumsq = wtsumsq/sumy;
    wtmean = wtsum/sumy;

    double leftlim = wtmean - sqrt(wtsumsq - wtmean*wtmean);
    double rightlim = wtmean +  sqrt(wtsumsq - wtmean*wtmean);

    int countleft =0, countright=0;
    for(int i = 0; i <gdata->x.size(); i++) {
        if( gdata->x.at(i) < leftlim  ) {
            countleft++;
        }
        else if( gdata->x.at(i) > rightlim ) {
            countright++;
        }
        else {
            if( gdata->y.at(i) < miny ) miny = gdata->y.at(i);
            if( gdata->y.at(i) > maxy ) maxy = gdata->y.at(i);
            x3.append(gdata->x.at(i));
            y3.append(gdata->y.at(i));
        }
    }
    y3[0] += countleft;
    y3[y3.size()-1] += countright;

    customPlot->yAxis->setRange(miny, maxy);
    customPlot->xAxis->setRange(x3.first() - deltaw , x3.last() + deltaw);


    QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis );
    customPlot->addPlottable(bars1);
    bars1->setWidth(deltaw);
    bars1->setData(x3, y3);

    bars1->setPen(QPen(Qt::black));
    bars1->setAntialiased(true);
    bars1->setAntialiasedFill(false);
   // bars1->setBrush(QColor("#705BE8"));
   // bars1->keyAxis()->setAutoTicks(false);
  //  bars1->keyAxis()->setTickVector(x3);
   // bars1->valueAxis()->setSubTickCount(deltaw);





}

void GraphData::plotLineGraph(QCustomPlot *customPlot, GRAPHDATA *gdata) {


    customPlot->graph()->setData(gdata->x, gdata->y);
    // give the axes some labels:
    customPlot->xAxis->setLabel(gdata->xlab);
    customPlot->yAxis->setLabel(gdata->ylab);

   // qDebug() << gdata->x;
   // qDebug() << gdata->y;

    // set axes ranges, so we see all data:
 //   qDebug() << "yaxis range " << customPlot->yAxis->range().maxRange << " xaxis range " << customPlot->xAxis->range().minRange << " - " << customPlot->xAxis->range().maxRange;
    customPlot->yAxis->setRange(0, gdata->ymax);
    customPlot->xAxis->setRange(gdata->xmin -1 , gdata->xmax + 1);

}

void GraphData::updateMinMax(GRAPHDATA *gdata) {
    double _min= 1e+20 , _max = -999999;
    foreach(double v, gdata->x) {
        if( v > _max ) _max = v;
        if( v < _min ) _min= v;
    }
    gdata->xmin = _min;
    gdata->xmax = _max;

    _min= 1e+20 ; _max = -999999;
    foreach(double v, gdata->y) {
        if( v > _max ) _max = v;
        if( v < _min ) _min= v;
    }
    gdata->ymin = _min;
    gdata->ymax = _max;

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
    //qDebug() << gdata->x;

    unsigned int nonzeroSize = Utilities::numNonZeros(gdata->y);

    Utilities::removeZeros(gdata->x, gdata->y, 1);

    this->updateMinMax(gdata);

    if( nonzeroSize < 100) {
         this->plotBarGraph(customPlot, gdata);
    }
    else {
         this->plotLineGraph(customPlot, gdata);
    }
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
