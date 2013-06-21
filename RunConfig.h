#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qradiobutton.h"
#include "qgroupbox.h"
#include "qcombobox.h"

namespace Ui {
class RunConfig;
}

class RunConfig : public QWidget
{
    Q_OBJECT
    
public:
    QList<QGroupBox *> *groupBoxes;

    static QComboBox *fileInputFormat;

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
    QPushButton *fileBrowseButton;
    QRadioButton *runAll;
    QRadioButton *redoAll;
    QRadioButton *skipAll;

    QGroupBox *runOptionsGroupBox;
};

#endif // RUNCONFIG_H
