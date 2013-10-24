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
#include <QMdiArea>

//#include "genebrowser/graphicsrepresentation.h"
#include "caching/fileindexmanager.h"
#include "datamodel/datamanager.h"
#include "popupviews/selectsamples.h"

#include "displaywidgets/mdiareawidget.h"
#include "displaywidgets/progressview.h"
#include "datamodel/samplercmanager.h"



class SelectSamples;

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    RunData* getRunData();
    HTableData *getHTableData(QString sampleName, ATTRTYPE attr,  QList<enum TYPE> types, QStringList headers,  DataManager *datamanager);
    ~ResultWindow();

    static ResultWindow *getResultWindow();

public slots:
    void sampleChanged(QString changed);
    void updateFileNames();
    void setVisible(int);
    void receiveSelection(QList<bool> &selection);

private slots:
    void clickedSelectSample();
    void _loadResults();

signals:
    void fileChanged(QString file);

private:
    explicit ResultWindow(QWidget *parent = 0);
    void switchToComparativeMode();
    void indexSamples(bool useResourceFolder);
  //  QString getFilePath(QString sampleName, QString OUTPUTPATH, INPUTFILETYPE type) ;


private:
    static ResultWindow *resultWindow;

    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *rundata;
    QComboBox* sampleSelect;
    QPushButton *selectSamplesButton;
    QPushButton *loadResults;
    QCheckBox *checkComparativeMode;
    QLabel *currentSampleLabel;

    QTabWidget *resultTabs;
    QSignalMapper *signal;
    QTimer *getFileNames;
    QStringList files;

    SelectSamples *selectWindow;

    QHash<QString, TableData *> tables;
    QHash<QString, GraphData *> graphs;
    QHash<QString, DisplayInfo *> displayInfos;
 //   QHash<QString, GraphicsRepresentation *> graphicsRepresentation;
    QHash<QString, MeganView *> meganviews;

    QList<bool> selectedSamples;

    DataManager *datamanager;
    bool disableSampleChanged;



};

#endif // RESULTWINDOW_H
