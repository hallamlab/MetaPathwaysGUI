#ifndef Setup_H
#define Setup_H
#include "mainframe.h"
#include "ui_Setup.h"
#include <QWidget>
#include <QString>
#include <QHash>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QSettings>
#include "helper/rundata.h"
#include "helper/types.h"

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT
    
public:
    explicit Setup( QWidget *parent = 0);
    ~Setup();

public:
    void loadPathVariables();
    void savePathVariables();
    void updateValues();

public slots:
    void pythonBrowse();
    void pgdbFolderBrowse();
    void metapathwaysBrowse();
    void saveSetup();
    void cancelSetup();
    void databaseBrowse();
    void pathologicBrowse();
    void canSave();

signals:
    void continueFromSetup();


private:
    Ui::Setup *ui;

    QString pythonPath;
    QString pgdbFolderPath;
    QString mpPath;
    QString databasePath;
    QString pathologicPath;

    QPushButton *pythonBrowseButton;
    QPushButton *pgdbFolderBrowseButton;
    QPushButton *metapathwaysBrowseButton;
    QPushButton *saveButton;
    QPushButton *databaseButton;
    QPushButton *pathologicButton;

    QLineEdit *pgdbFolderTxt;
    QLineEdit *pythonExecTxt;
    QLineEdit *dbDirectoryTxt;
    QLineEdit *pathMetaPathwaysTxt;
    QLineEdit *pathologicTxt;

};

#endif // Setup_H
