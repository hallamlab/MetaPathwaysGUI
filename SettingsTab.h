#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H
#include "RunConfig.h"
#include <QWidget>
#include <qlist>
#include <QPushButton>
#include <QTextEdit>

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

    QStringList *annotationFiles;
    QStringList *rrnaFiles;

    QString *annotationFileList;
    QString *rrnaFileList;

    QPushButton *annotationDBSButton;
    QPushButton *rrnaREFDBSButton;
    QTextEdit *annotationDBS;
    QTextEdit *rrnaREFDBS;

    RunConfig *RunConfigWindow;
    QList<QWidget *> *qcWidgets;
    QList<QWidget *> *orfWidgets;
    QList<QWidget *> *annotationWidgets;
    QList<QWidget *> *rrnaWidgets;
    QList<QWidget *> *pathwaysWidgets;


    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
