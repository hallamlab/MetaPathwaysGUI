#ifndef GRIDPROGRESS_H
#define GRIDPROGRESS_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QApplication>
#include <QHash>
#include <QString>
#include <QList>
#include <QTimer>
#include <QSet>
#include <QDebug>
#include <QLabel>
#include <QSpacerItem>


#include "datamodel/samplercmanager.h"
#include "displaywidgets/progressview.h"
#include "helper/rundata.h"



class GridProgress : public QWidget
{
    Q_OBJECT
public:
    explicit GridProgress(QWidget *parent = 0);
    ~GridProgress();
    
private:
    void createEmptyWidget();
    bool hasServersChanged();
    void addServerStats();
    void addLabel(QGridLayout *layout, QString text, unsigned int row, unsigned int col);
    QList< QHash<QString, QList<unsigned int> > > serverData;
    QList< QHash<QString, QList<unsigned int> > > transitionData;



signals:
    
public slots:
    void updatServerStats();


private:
    QTimer *timer;
    QVBoxLayout *Vbox ;
    QGridLayout *gridStates;
    QGridLayout *transitionMatrix;
    QList<QWidget *> serverStatsLabels;

    unsigned int counter;


    QStringList serverNames;

};

#endif // GridProgress_H
