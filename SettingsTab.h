#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H
#include "RunConfig.h"
#include <QWidget>
#include <qlist>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QSettings>
#include <QDir>

namespace Ui {
class SettingsTab;
}

class SettingsTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsTab(QWidget *parent = 0);
    static QList<QWidget *> *allWidgets;
    void setStyling();

    QLabel *annotationDBSWarning;
    QLabel *rrnaREFDBSWarning;

    QListWidget *annotationDBS;
    QListWidget *rrnaREFDBS;

    ~SettingsTab();

private slots:
    void closeWindow();
    void openParameterSetup();
    void annotationClicked(QModelIndex index);
    void rrnaClicked(QModelIndex index);

signals:
    void setContinueButton();
    void hideContinueButton();

private:
    bool writeOutToFile();
    void getAllWidgets();
    void initWidgetValues();
    void showORFDBS();
    void showRRNADBS();
    bool findFiles(QString path, QStringList *fileListing);
    void isBothDBSSet();


    QStringList *annotationFiles;
    QStringList *rrnaFiles;

    QPushButton *annotationDBSButton;
    QPushButton *rrnaREFDBSButton;

    RunConfig *RunConfigWindow;
    QList<QWidget *> *qcWidgets;
    QList<QWidget *> *orfWidgets;
    QList<QWidget *> *annotationWidgets;
    QList<QWidget *> *rrnaWidgets;
    QList<QWidget *> *pathwaysWidgets;

    QStringList* RRNADBS;
    QStringList* ORFDBS;

    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
