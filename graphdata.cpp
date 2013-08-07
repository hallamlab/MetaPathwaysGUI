#include "graphdata.h"
#include "ui_graphdata.h"
#include "qcustomplot.h"

GraphData::GraphData(const QString &file, QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::GraphData)
{
    ui->setupUi(this);

    this->file = file;
    titleLabel = this->findChild<QLabel *>("titleLabel");
    statsLabel = this->findChild<QLabel *>("statsLabel");
    this->graph = this->findChild<QCustomPlot *>("graph");

    this->setupFromFile(file);
}

void GraphData::setupFromFile(const QString &file){
    QCustomPlot *customPlot = new QCustomPlot();

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();
    this->graph = customPlot;
}

GraphData::~GraphData()
{
    delete ui;
}
