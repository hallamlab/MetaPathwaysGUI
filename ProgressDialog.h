#ifndef ProgressDialog_H
#define ProgressDialog_H

#include <QWidget>
#include "qpushbutton.h"
#include <QProcess>
#include <QTextBrowser>
#include <QTimer>
#include "rundata.h"
#include <QProgressBar>
#include "rundata.h"
#include "parentwidget.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(ParentWidget *pw = NULL, RunData *run = NULL, QWidget *parent = 0);
    void initProgressBar();
    void colorRunConfig(QString line);
    ~ProgressDialog();

private slots:
    void terminateRun();
    void updateText();
    
private:
    void closeEvent (QCloseEvent* event );

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
