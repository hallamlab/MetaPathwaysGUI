#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
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

private slots:
    void exportData();

private:
    Ui::ResultWindow *ui;
    QWidget* parent;
    RunData *run;

    QTabWidget *resultTabs;
};

#endif // RESULTWINDOW_H
