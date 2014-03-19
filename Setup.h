#ifndef Setup_H
#define Setup_H
#include "mainframe.h"
#include "ui_Setup.h"
#include <QWidget>
#include <QString>
#include <QHash>
#include <QLineEdit>
#include "rundata.h"
#include "types.h"

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
    void perlBrowse();
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
    QString perlPath;
    QString mpPath;
    QString databasePath;
    QString pathologicPath;

    QPushButton *pythonBrowseButton;
    QPushButton *perlBrowseButton;
    QPushButton *metapathwaysBrowseButton;
    QPushButton *saveButton;
    QPushButton *databaseButton;
    QPushButton *pathologicButton;

    QLineEdit *perlExecTxt;
    QLineEdit *pythonExecTxt;
    QLineEdit *dbDirectoryTxt;
    QLineEdit *pathMetaPathwaysTxt;
    QLineEdit *pathologicTxt;

};

#endif // Setup_H
