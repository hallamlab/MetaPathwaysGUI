#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>
#include <QTimer>
#include "rundata.h"
#include "ProgressDialog.h"
#include "resultpage.h"
#include "popupviews/displayinfo.h"
#include "ui_resultwindow.h"
#include "dataviews/graphdata.h"
#include "dataviews/tabledata.h"
#include <QFileDialog>
#include <QDebug>
#include "dataviews/meganview.h"
#include "genebrowser/genomeview.h"

//#include "genebrowser/graphicsrepresentation.h"
#include "caching/fileindexmanager.h"
#include "datamodel/datamanager.h"



namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWindow(ProgressDialog *prog = 0, QWidget *parent = 0);
    RunData* getRunData();
    ~ResultWindow();

public slots:
    void sampleChanged(QString changed);
    void updateFileNames();


signals:
    void fileChanged(QString file);

private:
    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *run;
    QComboBox* sampleSelect;
    QTabWidget *resultTabs;
    QSignalMapper *signal;
    QTimer *getFileNames;
    QStringList files;
    ProgressDialog *progress;


    QHash<QString, TableData *> tables;
    QHash<QString, GraphData *> graphs;
    QHash<QString, DisplayInfo *> displayInfos;
 //   QHash<QString, GraphicsRepresentation *> graphicsRepresentation;
    QHash<QString, MeganView *> meganviews;

    DataManager *datamanager;


};

#endif // RESULTWINDOW_H
