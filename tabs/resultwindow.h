#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>
#include <QTimer>
#include <QMdiArea>
#include <QTableView>
#include <QProgressDialog>
#include <QFileDialog>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QToolButton>


#include "ProgressDialog.h"
#include "resultpage.h"
#include "ui_resultwindow.h"

//#include "genebrowser/graphicsrepresentation.h"
#include "caching/fileindexmanager.h"
#include "datamodel/datamanager.h"
#include "datamodel/rundatastats.h"
#include "datamodel/samplercmanager.h"

#include "dataviews/graphdata.h"
#include "dataviews/tabledata.h"
#include "dataviews/meganview.h"

#include "displaywidgets/createwidgets.h"
#include "displaywidgets/mdiareawidget.h"
#include "displaywidgets/progressview.h"

#include "genebrowser/genomeview.h"

#include "helper/rundata.h"
#include "helper/types.h"

#include "popupviews/displayinfo.h"
#include "popupviews/selectsamples.h"

#include "tabs/simpletabgroups.h"
#include "helper/globaldatatransit.h"
#include "helper/labels.h"
#include "tabs/simpletabgroups.cpp"


extern const QString SELECT_SAMPLE_TAG;
class SelectSamples;
class TableData;
class GraphData;


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

    void copyROWData(QList<ROW *> &src, QList<ROW *> &tar, QList<ORF *> &selectORFs);
public slots:
    void sampleChanged(QString changed);
    void updateFileNames();
    void setVisible(int);
    void receiveSelection(QList<unsigned int> &selection);


    void ProvideContexMenu(QPoint position);
    void showTable(QString sampleName,  ATTRTYPE attrType);

private slots:
    void clickedSelectSample();
    void _loadResults();
    void browseFolder();
    void removeFolder();
    bool needsReloading();
    bool needsReloading(QString sampleName);


signals:
    void fileChanged(QString file);

private:
    explicit ResultWindow(QWidget *parent = 0);
    void switchToComparativeMode();
    void indexSamples(bool useResourceFolder, bool forcereindex =false);
    void indexSample(QString sampleName, bool userResourceFolder) ;
  //  QString getFilePath(QString sampleName, QString OUTPUTPATH, INPUTFILETYPE type) ;
    TableData* getFunctionalAndTaxTable(QString sampleName, bool useCache = true, bool useLCAStar = false);
    TableData *_createFunctionalAndTaxTable(const QString &sampleName);
    QStringList getResultFoldersList();


public:
    QHash<HTableData *, bool> htablesAddSignals;
private:
    static ResultWindow *resultWindow;

    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *rundata;
    QComboBox* sampleSelect;
    QPushButton *selectSamplesButton;
    QPushButton *loadResults;
    QComboBox *resultViewMode;
    QLabel *currentSampleLabel;

    QTabWidget *resultTabs;
    QToolButton *addResultFolder, *deleteResultFolder;
    QComboBox *resultFolders;


    SimpleTabGroups<TableData> simpleTabGroups;
    SimpleTabGroups<GraphData> simpleGraphGroups;

    QSignalMapper *signal;
    QTimer *getFileNames;
    QStringList files;

    SelectSamples *selectWindow;

    QHash<QString, TableData *> tables;
    QHash<QString, GraphData *> graphs;
    QHash<QString, DisplayInfo *> displayInfos;
 //   QHash<QString, GraphicsRepresentation *> graphicsRepresentation;
    QHash<QString, MeganView *> meganviews;

    HTABLE_PARAMS tableParams;
    QList<unsigned int> selectedSamples;

    DataManager *datamanager;
    bool disableSampleChanged;
    QHash<QString, QString> runids;


};

#endif // RESULTWINDOW_H
