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

    void _loadPathVariables();
    void savePathVariables();
    void updateValues();
    bool hasValidBinaries(QString metapathways_dir, QString folder);
    void showSetupError(QString warningStr);

private:
    void validatePartialSetup(int i) ;
public slots:
    void pythonBrowse();
    void pgdbFolderBrowse();
    void metapathwaysBrowse();
    void saveSetup();
    void saveParamFile();
    void saveConfigFile();

    void cancelSetup();
    void databaseBrowse();
    void pathologicBrowse();
    void canSave(int i);
    void validateSetup(bool interactive, bool triggerLoad = false);
private slots:
    void saveconfig();
    void updateExecutablesFolderList();
    QStringList getExecutablesPath(QString metapathways_dir);

signals:
    //void continueFromSetup();
    void editSlotChanged(int);


private:
    Ui::Setup *ui;

    QString pythonPath;
    QString executablesPath;
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
    QComboBox *executablesCombo;

    QHash<QString, bool> partialvalidation;

};




#endif // Setup_H
