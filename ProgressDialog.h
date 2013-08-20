#ifndef ProgressDialog_H
#define ProgressDialog_H

#include "qpushbutton.h"
#include "rundata.h"
#include "parentwidget.h"
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
    int stepsPassed;

    ~ProgressDialog();

private slots:
    void terminateRun();
    void updateText();
    void checkFiles();
    void changed(QProcess::ProcessState state);
    void selectedFileChanged(QString file);

private:
    Ui::ProgressDialog *ui;

    QPushButton *cancelButton;
    QTextBrowser *textBrowser;
    QProgressBar *progressBar;
    QString currentFile;

    QProcess *myProcess;
    QTimer *timer;
    QTimer *fileCheckTimer;
};

#endif // ProgressDialog_H
