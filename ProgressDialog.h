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
    explicit ProgressDialog(ParentWidget *pw = NULL, RunData *run = NULL, QWidget *parent = 0);
    void initProgressBar();
    void initProcess();

    RunData *run;
    ParentWidget *pw;
    QString METAPATH;
    QStringList *filesDetected;
    QHash<int,QString> *TABLE_MAPPING;
    QHash<QString, QString> previousStatus;

    int blastCount;
    int parseBlastCount;
    int scanRRNACount;
    int statsCount;

    bool blastFailed;
    bool parseBlastFailed;
    bool scanRRNAFailed;
    bool statsFailed;

    QHash<QString,int> *stepsPassed;

    void colorRunConfig(QString stepName,  QString status);
    void multiStepCheck(QString *stepName, QString *status);
    void initMapping();

    ~ProgressDialog();

private slots:
    void terminateRun();
    void updateText();
    void checkFiles();
    void toggleDetails();
    void updateTable();
    void selectedFileChanged(QString file);

private:
    Ui::ProgressDialog *ui;

    QPushButton *cancelButton;
    QPushButton *hideButton;
    QTextBrowser *logBrowser;
    QLabel *logLabel;
    QTableWidget *summaryTable;
    QLabel *summaryLabel;
    QProgressBar *progressBar;
    QProgressBar *globalProgressBar;
    QString currentFile;
    QLabel* progressLabel;
    QProcess *myProcess;
    QTimer *timer;
    QTimer *fileCheckTimer;
    QTextEdit *standardOut;
};

#endif // ProgressDialog_H
