#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QPushButton>
#include "metawidget.h"
#include "qcustomplot.h"

namespace Ui {
class GraphData;
}

class GraphData : public QWidget, public MetaWidget
{
    Q_OBJECT
    
public:
    explicit GraphData(const QString &file, QWidget *parent = 0);
    virtual void setupFromFile(const QString &file);
    QList<QStringList>* parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT);
    void plotSomeGraph(QCustomPlot *graph);

    QCustomPlot* graph;
    QPushButton* exportButton;
    QVector<double>* graphData;
    ~GraphData();

    void setNumBuckets(unsigned int num);
    unsigned int getNumBuckets();

    void setMaxValue(double m);
    void setMinValue(double s);
    void setYMax(double y);
    double getMaxValue();
    double getMinValue();
    double getYMaxValue();
    void computeParams();
    void computeHistoGram();
    
private:
    double bucketSize;
    double min, max;
    double ymax;
    unsigned int numBuckets;
    QVector<double> x, y;

    Ui::GraphData *ui;
};

#endif // GRAPHDATA_H
