#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QWidget>
#include <QLabel>
#include "metawidget.h"

namespace Ui {
class GraphData;
}

class GraphData : public QWidget, public MetaWidget
{
    Q_OBJECT
    
public:
    explicit GraphData(const QString &file, QWidget *parent = 0);
    void setupFromFile(const QString &file);
    QString file;
    QLabel* titleLabel;
    QLabel* statsLabel;
    QWidget* graph;
    ~GraphData();
    
private:
    Ui::GraphData *ui;
};

#endif // GRAPHDATA_H
