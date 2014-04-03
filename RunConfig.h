#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QComboBox>
#include <QTableWidget>
#include "grid/gridsetup.h"
#include "types.h"
#include "rundata.h"

class ParentWidget;
class GridSetup;

namespace Ui {
class RunConfig;
}

class RunConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit RunConfig(QWidget *parent = 0);
    ~RunConfig();

    QString selectedFolder;
    QString selectedFiles;

    QList<QGroupBox *> *groupBoxes;
    QCheckBox* gridBlastChoice;
    QComboBox *fileInputFormat;
    QCheckBox* overwrite;

private slots:
    void closeWindow();
    void toggleAllRun();
    void toggleAllSkip();
    void toggleAllRedo();
    void browseFile();
    void browseFolder();
    void specifyGrid();
    //void run();

signals:
    void fileSet();

private:
    void loadRunParams();
    void setStyling();

    Ui::RunConfig *ui;
    QRadioButton *runAll;
    QRadioButton *redoAll;
    QRadioButton *skipAll;

    RunData *rundata;

    QGroupBox *runOptionsGroupBox;
    QPushButton *fileBrowseButton;
    QTableWidget *table;
    QPushButton* setupGrids;
    GridSetup* gridSetup;
    QPushButton *folderBrowseButton;

    QLineEdit* inputLine;
    QLineEdit* outputLine;
};

#endif // RUNCONFIG_H
