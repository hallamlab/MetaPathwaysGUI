#ifndef ProgressDialog_H
#define ProgressDialog_H

#include "qpushbutton.h"
#include "helper/rundata.h"
#include "resultpage.h"
#include "helper/progressdisplaydata.h"
#include "dataviews/statusview.h"

#include <QTimer>
#include <QWidget>
#include <QProcess>
#include <QTextBrowser>
#include <QProgressBar>
#include <QCheckBox>
#include <QMessageBox>
#include <QTreeView>

#include "dataviews/statusview.h"
#include "datamodel/treeitem.h"
#include "datamodel/treemodel.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    void updateProgressBar();
    void initProcess();

    RunData *rundata;
    QString METAPATH;
    QStringList filesDetected;
    QHash<int,QString> *TABLE_MAPPING;
    QHash<QString, QString> previousStatus;


    QHash<QString,int> stepsPassed;

    void checkStepsWithDBS(QHash<QString,QString> *statusHash, QString stepName, QString realStepName);
    void colorRunConfig(QHash<QString,QString> &statusHash);
    void initMapping();

    ~ProgressDialog();
private:
    bool checkInputOutPutLocations() ;

private slots:

    void terminateRun();
    void checkFiles();
    void selectedFileChanged(QString file);
    void loadSampleList();
    void showErrors();
    void readStepsLog();
    void startRun();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    unsigned int countTotalNumberOfSteps();

    void resetRunTab() ;


private:
    Ui::ProgressDialog *ui;

    unsigned int _blastCount;
    unsigned int _parseBlastCount;
    unsigned int _scanCount;
    unsigned int _statsCount;

    unsigned int _totalSteps;
    unsigned int _stepsCompleted;

    QPushButton *cancelButton;
    QPushButton *hideButton;
    QPushButton *runButton, *showErrorsButton;

    QTextBrowser *logBrowser;
    QLabel *logLabel;
    QTableWidget *summaryTable;
    QLabel *summaryLabel;
    QProgressBar *progressBar;
    QProgressBar *globalProgressBar;
    QLabel* progressLabel;
    QProcess *myProcess;
    QTimer *timer;
    QTimer *fileCheckTimer;
    QTextEdit *standardOut;
    QComboBox* sampleSelect;
    QCheckBox* runVerbose;
    QCheckBox *computeStats;

};

#endif // ProgressDialog_H