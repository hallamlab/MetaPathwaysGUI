#ifndef ProgressDialog_H
#define ProgressDialog_H

#include "qpushbutton.h"
#include "rundata.h"
#include "parentwidget.h"
#include "resultpage.h"
#include <QTimer>
#include <QWidget>
#include <QProcess>
#include <QTextBrowser>
#include <QProgressBar>


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
    ParentWidget *pw;
    QString METAPATH;
    QStringList filesDetected;
    QHash<int,QString> *TABLE_MAPPING;
    QHash<QString, QString> previousStatus;


    QHash<QString,int> stepsPassed;

    void checkStepsWithDBS(QHash<QString,QString> *statusHash, QString stepName, QString realStepName);
    void colorRunConfig(QHash<QString,QString> *statusHash);
    void initMapping();

    ~ProgressDialog();
private:
    bool checkInputOutPutLocations() ;

private slots:
    void terminateRun();
    void checkFiles();
    void selectedFileChanged(QString file);
    void readStepsLog();
    void startRun();

private:
    Ui::ProgressDialog *ui;

    unsigned int _blastCount;
    unsigned int _parseBlastCount;
    unsigned int _scanCount;
    unsigned int _statsCount;

    unsigned int _totalStepsPerSample;
    unsigned int _stepsCompletedSample;

    QPushButton *cancelButton;
    QPushButton *hideButton;
    QPushButton *runButton;

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
};

#endif // ProgressDialog_H
