#ifndef ProgressDialog_H
#define ProgressDialog_H

#include <QWidget>
#include "qpushbutton.h"
#include <QProcess>
#include <QTextBrowser>
#include <QTimer>


namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

private slots:
    void terminateRun();
    void updateText();
    
private:
    Ui::ProgressDialog *ui;

    QPushButton *cancelButton;
    QTextBrowser *textBrowser;

    QProcess *myProcess;
    QTimer *timer;
};

#endif // ProgressDialog_H
