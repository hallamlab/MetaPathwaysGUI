#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QPushButton>

#include "helper/qcustomplot.h"
#include "caching/tablemanager.h"

namespace Ui {
class GraphData;
}

class GraphData : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphData( QWidget *parent=0) ;
    virtual void setupFromFile(const QString &file);
    QList<QStringList>* parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT);
    bool setFile(QString fileName) ;
    void plotSomeGraph(QCustomPlot *graph, GRAPHDATA *gdata);
    bool prepareInput(const QString &xlab = "-", const QString &ylab = "-");

    QCustomPlot* graph;
    QPushButton* exportButton;
    QVector<double> graphData;
    ~GraphData();


    void computeParams(GRAPHDATA *gdata);
    void computeHistoGram(GRAPHDATA *gdata);
    
private slots:
    void saveImage();

private:
    double bucketSize;
    double min, max;
    double ymax;
    unsigned int numBuckets;
    QString file;
    QLabel* titleLabel;
    QVector<double> x, y;

    Ui::GraphData *ui;
};

#endif // GRAPHDATA_H
