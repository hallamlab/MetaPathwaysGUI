#include "tabfactory.h"
#include "QDockWidget"
#include "QGridLayout"
#include "QTableWidget"
#include "qcustomplot.h"
#include "QTextEdit"
#include <QLabel>
#include "utilities.h"


TabFactory::TabFactory()
{
}

QWidget* TabFactory::createTable(QString FILE_NAME){
    QGridLayout *layout = new QGridLayout();
    QWidget *mainWidget = new QWidget();
    QString summaryText;
    QList<QString>* labelText;
    QTableWidget *table;

    QLabel *summaryLabel = new QLabel();
    summaryLabel->setWordWrap(true);

    QChar labelDELIM = '$';
    QChar tableDELIM = '#';

    labelText = Utilities::parseResults(FILE_NAME,labelDELIM);
    table = Utilities::createTable(FILE_NAME, tableDELIM);

    layout->addWidget(table, 0, 0, -1, 1);


    foreach(QString s, *labelText){
        summaryText.append(s.trimmed());
        summaryText.append("\n");
    }

    summaryLabel->setText(summaryText);
    layout->addWidget(summaryLabel, 0, 1 ,1, 1);

    mainWidget->setLayout(layout);

    return mainWidget;
}

QDockWidget* TabFactory::createGraph(){
    QDockWidget *dock = new QDockWidget();
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
    QWidget *widget = customPlot;
    dock->setWidget(widget);
    return dock;
}

QDockWidget* TabFactory::createBarGraph(){
    QDockWidget *dock = new QDockWidget();
    QCustomPlot *customPlot = new QCustomPlot();
    QCPBars *myBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);

    customPlot->addPlottable(myBars);
    // now we can modify properties of myBars:
    myBars->setName("Pretty bars");
    QVector<double> keyData;
    QVector<double> valueData;
    keyData << 1 << 2 << 3 << 4 << 5;
    valueData << 2 << 4 << 8 << 12 << 15;
    myBars->setData(keyData, valueData);
    customPlot->rescaleAxes();
    customPlot->replot();

    QWidget *widget = customPlot;
    dock->setWidget(widget);
    return dock;
}
