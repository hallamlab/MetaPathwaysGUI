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
    ~ProgressDialog();

private slots:
    void terminateRun();
    void updateText();
    
private:
    Ui::ProgressDialog *ui;
    RunData *run;
    ParentWidget *pw;

    QPushButton *cancelButton;
    QTextBrowser *textBrowser;
    QProgressBar *progressBar;

    QProcess *myProcess;
    QTimer *timer;
};

#endif // ProgressDialog_H
