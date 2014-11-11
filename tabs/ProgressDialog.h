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

typedef struct _STATUS {
    QString step;
    QHash<QString, bool> done, expected;
} STATUS;

class ProgressDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    void updateProgressBar();
    void updateCurrentRunningProcessStatus();
    void initProcess();
    void checkBinaries();
    QStringList getActiveSteps() ;
    void shadeActiveSteps();

    bool isProcessingSample(QString sampleName);
    void updateHighestBlock(QString sampleName);

    RunData *rundata;
    QString METAPATH;
    QStringList filesDetected;
    QHash<int,QString> TABLE_MAPPING;
    QHash<QString, QString> previousStatus;


    QHash<QString,int> stepsPassed;
    QHash<QString, bool> expectedSteps;

    QHash<QString, STATUS> status;
    QHash<QString, QString> running;

    short int getState(const QString &stepName);

    bool updateItem(const QString stepName, STATUS_SYMBOL status, ProgressDisplayData *progressdisplaydata) ;
    void colorRunConfig();
    void initMapping();

    ~ProgressDialog();
private:
    bool checkInputOutPutLocations() ;
    void setExpectedSteps();
    void updateStatus(const QString &line);
    bool isProcessRunning();
    bool isRunningAStep();
    bool isStepActive(QString stepName);
private slots:

    void terminateRun();
    void checkFiles();
    void selectedFileChanged(const QString &sampleName);
    void loadSampleListToRun();
    void showErrors();
    void readStepsLog();
    void timerTickResponse();
    void delayReadingStepsLog();

    void startRun();
    void setProcessToZero();

    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    unsigned int countTotalNumberOfSteps();
    unsigned int getNumStepsCompleted();

    void resetRunTab(bool status, bool clearlog = true) ;
    void updateOverwriteChoice();

public:
public:
    bool shouldOverwrite();

private:
    Ui::ProgressDialog *ui;

    unsigned int _blastCount;
    unsigned int _parseBlastCount;
    unsigned int _scanCount;
    unsigned int _statsCount;

    unsigned int _totalSteps;
    unsigned int _stepsCompleted;
    QHash<QString, STATUS_SYMBOL> _stepstatus;

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
    QLineEdit *sampleNameLineEdit;
    QProcess *myProcess;
    QTimer *timer;
    QTimer *fileCheckTimer;
    QTextEdit *standardOut;
    QComboBox* sampleSelect;
    QCheckBox* runVerbose;
    QCheckBox *overwrite;
    QString runid;
    int currentBLOCK;

    static unsigned int waitCounter;

};

#endif // ProgressDialog_H
