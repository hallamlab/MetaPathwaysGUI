#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qradiobutton.h"

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

private:
    QString selectedFile;
    QLabel *fileSelectedPath;

    Ui::RunConfig *ui;
    QPushButton *runButton;
    QPushButton *cancelButton;
    QPushButton *fileBrowseButton;

    QCheckBox *runAllCheck;
    QCheckBox *redoAllCheck;
    QCheckBox *skipAllCheck;

    void getAllRadioButtons();
    void loadRunParams(QHash<QString,QString> *params);
    void setStyling();
    QList<QWidget *> *groupBoxes;
    QList<QList<QRadioButton *>*> *radioButtons;
};

#endif // RUNCONFIG_H
