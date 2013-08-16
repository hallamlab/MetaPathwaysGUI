#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QWidget>
#include <QLabel>
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
    ~GraphData();
    
private:
    Ui::GraphData *ui;
};

#endif // GRAPHDATA_H
