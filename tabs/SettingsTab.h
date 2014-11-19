#ifndef SETTINGSTAB_H
#define SETTINGSTAB_H
#include "RunConfig.h"
#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QSettings>
#include <QDir>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QFileInfo>


namespace Ui {
class SettingsTab;
}

class SettingsTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit SettingsTab(QWidget *parent = 0);
    //explicit SettingsTab(QWidget *parent = 0);

    static QList<QWidget *> *allWidgets;
    void setStyling();

    QLabel *annotationDBSWarning;
    QLabel *rrnaREFDBSWarning;

    QListWidget *annotationDBS;
    QListWidget *rrnaREFDBS;

    ~SettingsTab();

private slots:
    void closeWindow();
    void annotationClicked(QModelIndex index);
    void rrnaClicked(QModelIndex index);
    void restoreDefaultValues();

public slots:
    void loadParameters();

private:
    bool writeOutToFile();
    void getAllWidgets();
    QStringList getADBNames();
    void initWidgetValues(bool defaultValue = false);
    void checkAnnotationDBS();
    void showORFDBS();
    void showRRNADBS();
    bool findFiles(QString path, QStringList &fileListing);


    QStringList *annotationFiles;
    QStringList *rrnaFiles;

    QPushButton *annotationDBSButton;
    QPushButton *rrnaREFDBSButton;

    QList<QWidget *> *qcWidgets;
    QList<QWidget *> *orfWidgets;
    QList<QWidget *> *annotationWidgets;
    QList<QWidget *> *rrnaWidgets;
    QList<QWidget *> *pathwaysWidgets;

    QStringList* RRNADBS;
    QStringList* ORFDBS;

    QPushButton *restoreDefaults;

    Ui::SettingsTab *ui;
};

#endif // SETTINGSTAB_H
