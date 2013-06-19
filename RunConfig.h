#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qradiobutton.h"
#include "qgroupbox.h"

namespace Ui {
class RunConfig;
}

class RunConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit RunConfig(QWidget *parent = 0);
    ~RunConfig();

private slots:
    void closeWindow();
    void toggleAllRun();
    void toggleAllSkip();
    void toggleAllRedo();
    void browseFile();
    void run();

private:
    void loadRunParams();
    void setStyling();

    QString selectedFile;
    QLabel *fileSelectedPath;

    Ui::RunConfig *ui;
    QPushButton *runButton;
    QPushButton *cancelButton;
    QPushButton *fileBrowseButton;

    QRadioButton *runAll;
    QRadioButton *redoAll;
    QRadioButton *skipAll;

    QList<QGroupBox *> *groupBoxes;
    QGroupBox *runOptionsGroupBox;
};

#endif // RUNCONFIG_H
