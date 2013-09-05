#include "graphdata.h"
#include "ui_graphdata.h"

GraphData::GraphData(const QString &file, QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::GraphData)
{
    ui->setupUi(this);

    this->file = file;
    titleLabel = this->findChild<QLabel *>("titleLabel");
    graph = this->findChild<QCustomPlot *>("graph");
    titleLabel->setText(file);

    this->setNumBuckets(500);

    this->setupFromFile(file);

    this->computeParams();

    qDebug() << graphData->size();
    this->computeHistoGram();
    this->plotSomeGraph(this->graph);
}

unsigned int GraphData::getNumBuckets() {
    return this->numBuckets;
}

void GraphData::computeParams() {
    double _min= 999999, _max = -999999;
    foreach(double v, *graphData) {
        if( v > _max ) _max = v;
        if( v < _min ) _min= v;
    }
    setMaxValue(_max);
    setMinValue(_min);
    this->bucketSize = (getMaxValue() - getMinValue())/(double)getNumBuckets();
}



void GraphData::setNumBuckets(unsigned int n) {
    this->numBuckets =n;

}



void GraphData::setMaxValue(double m) {
    this->max = m;
}

void GraphData::setMinValue(double s) {
    this->min = s;
}

double GraphData::getMaxValue() {
    return this->max;
}

double GraphData::getMinValue() {
    return this->min;
}

void GraphData::setupFromFile(const QString &file){
    graphData = new QVector<double>();
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
                if (ok) graphData->append(number.toDouble());
            }
        }
        inputFile.close();
    }


}
void GraphData::computeHistoGram() {
    for(unsigned int i =0; i < getNumBuckets(); i++) {
        y.push_back(0);
        x.push_back(this->bucketSize*(i + 0.5));
    }

    unsigned int j;
    foreach(double v, *graphData) {
        j = (unsigned int)(v - this->min)/this->bucketSize;
        y[j]++;
    }

    double _ymax = 0;
    foreach(double v, y){
        if (v > _ymax) _ymax = v;
    }
    this->setYMax(_ymax);
    qDebug() << y;
    qDebug() << x;

}

void GraphData::setYMax(double y){
    this->ymax = y;
}

void GraphData::plotSomeGraph(QCustomPlot *customPlot){
    QCPBars *barGraph = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    // create graph and assign data to it:
    customPlot->addPlottable(barGraph);

    barGraph->setBrush(QBrush(QColor(255,255,255)));
    barGraph->setData(this->x, this->y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->yAxis->setRange(0,this->ymax);
    customPlot->xAxis->setRange(this->min -1 , this->max + 1);
    barGraph->setBrush(Qt::SolidPattern);
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
    delete ui;
}
