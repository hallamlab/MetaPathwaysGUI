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
#include "gridsetup.h"

class ParentWidget;
class GridSetup;

namespace Ui {
class RunConfig;
}

class RunConfig : public QWidget
{
    Q_OBJECT
    
public:
    QList<QGroupBox *> *groupBoxes;
    QGroupBox *runOptionsGroupBox;
    QPushButton *fileBrowseButton;
    QComboBox *fileInputFormat;
    QTableWidget *table;
    QStringList *selectedFiles;
    QLabel *filesSelected;
    QLabel* sampleWarning;
    QCheckBox* gridBlastChoice;
    QPushButton* setupGrids;
    GridSetup* gridSetup;

    explicit RunConfig(QWidget *parent = 0);
    ~RunConfig();

private slots:
    void closeWindow();
    void toggleAllRun();
    void toggleAllSkip();
    void toggleAllRedo();
    void browseFile();
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

};

#endif // RUNCONFIG_H
