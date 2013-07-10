#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QSignalMapper>
#include "rundata.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWindow(RunData *run = 0, QWidget *parent = 0);
    RunData* getRunData();
    ~ResultWindow();

private:
    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *run;
    QComboBox* sampleSelect;
    QTabWidget *resultTabs;
    QSignalMapper *signal;
};

#endif // RESULTWINDOW_H
