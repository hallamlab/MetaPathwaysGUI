#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <QMainWindow>
#include <QMessageBox>
#include <QRect>
#include "parentwidget.h"
#include "Setup.h"
#include "resultwindow.h"
#include "utilities.h"
#include "ProgressDialog.h"
#include "welcome.h"
#include "displaywidgets/toolbarwidgets.h"
#include "displaywidgets/toolbarmanager.h"
#include "displaywidgets/mqtoolbar.h"

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
   // static bool settingsAvailable();
    void loadPathVariables();

    QMessageBox *warning;
    QString pathMetaPathways;

    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

private slots:
    void openSetup();

    void validateSetup();
    void displayProgress();
    void displayWelcome();
    void displayResults();
    void displayStages();
    void displayParams();

private:
    void showSetupError(QString warningStr);
    void addRemainingTabs();
    void updateWidgets();
    void executionPrep();
    void greyTabs(bool enabled);

    Ui::MainFrame *ui;
    ParentWidget *parentWidget;
    Setup *setupWidget;
    ResultWindow *resultWindow;
    ProgressDialog *progress;
    Welcome* welcomeWindow;
    RunConfig* stages;
    SettingsTab* settings;

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

    QMenu* menuSetup;
    QMenu* menuAbout;

    RunData* rundata;


};

#endif // MAINFRAME_H
