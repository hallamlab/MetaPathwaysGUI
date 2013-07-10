#ifndef PARENTSETTINGSMAINWINDOW_H
#define PARENTSETTINGSMAINWINDOW_H

#include "parentsettingsmainwindow.h"
#include "parentwidget.h"
#include "mainwindow.h"
#include <QMainWindow>
#include <QMenuBar>

namespace Ui {
class ParentSettingsMainWindow;
}

class MainWindow;

class ParentSettingsMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ParentSettingsMainWindow(MainWindow *mw, QWidget *parent = 0);
    ~ParentSettingsMainWindow();
    
private:
    Ui::ParentSettingsMainWindow *ui;
    QMenuBar *menu;
    QAction *openSetupAction;
    QAction *openAboutAction;
    ParentWidget* parentWidget;
};

#endif // PARENTSETTINGSMAINWINDOW_H
