#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qradiobutton.h"
#include "qgroupbox.h"
#include "qcombobox.h"
#include "QTableWidget"

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

    explicit RunConfig(QWidget *parent = 0);
    ~RunConfig();

private slots:
    void closeWindow();
    void toggleAllRun();
    void toggleAllSkip();
    void toggleAllRedo();
    void browseFile();
    //void run();

private:
    void loadRunParams();
    void setStyling();

    QLabel *fileSelectedPath;
    QString selectedFile;

    Ui::RunConfig *ui;
    QRadioButton *runAll;
    QRadioButton *redoAll;
    QRadioButton *skipAll;

};

#endif // RUNCONFIG_H
