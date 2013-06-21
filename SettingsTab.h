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
    static QList<QWidget *> *allWidgets;

    ~SettingsTab();

private slots:
    void closeWindow();
    void openParameterSetup();
    void annotationDBSPressed();
    void rrnaREFDBSPressed();

private:
    bool writeOutToFile();
    void getAllWidgets();
    void initWidgetValues();

    QString annotationDBSPath;
    QString rrnaREFDBSPath;

    QPushButton *annotationDBSButton;
    QPushButton *rrnaREFDBSButton;
    QComboBox *annotationDBS;
    QComboBox *rrnaREFDBS;

    RunConfig *RunConfigWindow;
    QList<QWidget *> *qcWidgets;
    QList<QWidget *> *orfWidgets;
    QList<QWidget *> *annotationWidgets;
    QList<QWidget *> *rrnaWidgets;
    QList<QWidget *> *pathwaysWidgets;


    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
