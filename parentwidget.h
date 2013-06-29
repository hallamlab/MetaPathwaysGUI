#ifndef PARENTWIDGET_H
#define PARENTWIDGET_H

#include <QWidget>
#include "SettingsTab.h"
#include "RunConfig.h"
#include "qtabwidget.h"
#include "rundata.h"

namespace Ui {
class ParentWidget;
}

class ParentWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParentWidget(QWidget *parent = 0);

    RunConfig *runConfigTab;

    ~ParentWidget();
    
private slots:
    void continueButtonPressed();
    void cancelButtonPressed();
    void backButtonPressed();
    void tabChanged();

private:
    void executionPrep();

    RunData *run;

    Ui::ParentWidget *ui;

    QPushButton *continueButton;
    QPushButton *cancelButton;
    QPushButton *backButton;

    QTabWidget *tab;
    SettingsTab *settingsTab;
};

#endif // PARENTWIDGET_H
