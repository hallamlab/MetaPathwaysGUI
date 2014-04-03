#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <QMainWindow>
#include <QMessageBox>
#include <QRect>
#include <QStackedWidget>
#include <fstream>
#include <iostream>
#include <QSettings>
#include <QStylePainter>
#include "Setup.h"
#include "resultwindow.h"
#include "RunConfig.h"
#include "SettingsTab.h"
#include "utilities.h"
#include "ProgressDialog.h"
#include "welcome.h"
#include "grid/gridprogress.h"
#include "displaywidgets/toolbarwidgets.h"
#include "displaywidgets/toolbarmanager.h"
#include "displaywidgets/mqtoolbar.h"
#ifdef Q_OS_WIN
    #include <windows.h> // for Sleep
#endif

namespace Ui {
class MainFrame;
}

typedef struct _ENVVARIABLES {
    QString pathMetaPathways;
} ENV;

class Setup;
class ResultWindow;
class ProgressDialog;

class MainFrame : public QMainWindow
{
    Q_OBJECT
    
public:
    static const QString TEMPLATE_PARAM;
    static const QString DEFAULT_TEMPLATE_PARAM;

    static const QString TEMPLATE_CONFIG;
    static const QString DEFAULT_TEMPLATE_CONFIG;

    static QHash<QString,QString> CONFIG;
    static QHash<QString,QString> PARAMS;
    static QHash<QString,QString> CONFIG_MAPPING;

    void createMapping();
    void loadPathVariables();

    QMessageBox *warning;
    QString pathMetaPathways;

    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

private slots:
    void openSetup();

    void validateSetup();
    void displayProgress();
    void displayResults();
    void displayStages();
    void displayParams();
    void displayGridProgress();

private:
    void showSetupError(QString warningStr);
    void addRemainingTabs();
    void updateWidgets();
    void executionPrep();
    void greyTabs(bool enabled);
    void setupLeftToolBar();
    void qSleep(int ms);

    Ui::MainFrame *ui;
    Setup *setupWidget;
    ResultWindow *resultWindow;
    ProgressDialog *progress;
    Welcome* welcomeWindow;
    RunConfig* stages;
    SettingsTab* settings;
    GridProgress* gridProgress;

    QScrollArea* stageScroll;
    QScrollArea* settingsScroll;

    QWidget *centralWidget;
    QStackedWidget* stackedWidget;

    QAction* actionSetup;
    QAction* actionRunSettings;
    QAction* actionProgress;
    QAction* actionGridProgress;
    QAction* actionResults;
    QAction* actionSetupMenu;
    QAction* actionAbout;
    QAction* actionRunParams;
    QAction* actionRunStages;

    QToolBar* toolBar;
    QToolBar* leftToolBar;

//    QMenu* menuSetup;
//    QMenu* menuAbout;

    RunData* rundata;


};

#endif // MAINFRAME_H
