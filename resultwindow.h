#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>
#include <QTimer>
#include "rundata.h"
#include "ProgressDialog.h"
#include "resultpage.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWindow(ResultPage *rp, ProgressDialog *prog = 0, RunData *run = 0, QWidget *parent = 0);
    RunData* getRunData();
    ~ResultWindow();

public slots:
    void sampleChanged(QString changed);
    void updateFileNames();

signals:
    void fileChanged(QString file);

private:
    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *run;
    QComboBox* sampleSelect;
    QTabWidget *resultTabs;
    QSignalMapper *signal;
    QTimer *getFileNames;
    QStringList files;
    ProgressDialog *progress;
};

#endif // RESULTWINDOW_H