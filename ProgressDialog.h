#ifndef ProgressDialog_H
#define ProgressDialog_H

#include <QWidget>
#include "qpushbutton.h"
#include <QProcess>

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
    
private:
    Ui::ProgressDialog *ui;

    QPushButton *cancelButton;
    QProcess *myProcess;
};

#endif // ProgressDialog_H
