#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H

#include <QWidget>
#include "qlist.h"
#include "RunConfig.h"
#include "qpushbutton.h"

namespace Ui {
class SettingsTab;
}

class SettingsTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsTab(QWidget *parent = 0);
    ~SettingsTab();

private slots:
    void closeWindow();
    void openParameterSetup();

private:
    bool writeOutToFile();
    void getAllWidgets();
    void initWidgetValues();

    RunConfig *RunConfigWindow;
    QList<QWidget *> *qcWidgets;
    QList<QWidget *> *orfWidgets;
    QList<QWidget *> *annotationWidgets;
    QList<QWidget *> *rrnaWidgets;
    QList<QWidget *> *allWidgets;

    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
