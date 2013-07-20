#ifndef PARENTWIDGET_H
#define PARENTWIDGET_H

#include "SettingsTab.h"
#include "RunConfig.h"
#include "qtabwidget.h"
#include "rundata.h"
#include <QWidget>

namespace Ui {
class ParentWidget;
}

class ParentWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParentWidget(QWidget *parent = 0);
    QPushButton *continueButton;
    QPushButton *cancelButton;
    QPushButton *backButton;
    RunConfig *runConfigTab;
    SettingsTab *settingsTab;

    ~ParentWidget();
    
private slots:
    void continueButtonPressed();
    void cancelButtonPressed();
    void backButtonPressed();
    void tabChanged();
    void enableContinueButton();

private:
    void executionPrep();
    Ui::ParentWidget *ui;

    RunData *run;
    QTabWidget *tab;
};

#endif // PARENTWIDGET_H
