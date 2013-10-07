#ifndef MAINFRAME_H
#define MAINFRAME_H
#include <QMainWindow>
#include <QMessageBox>
#include "parentwidget.h"
#include "Setup.h"
#include "resultwindow.h"
#include "utilities.h"
#include "ProgressDialog.h"
#include "welcome.h"
#include "displaywidgets/toolbarwidgets.h"
#include "displaywidgets/toolbarmanager.h"

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

    static QHash<QString,QString> *CONFIG;
    static QHash<QString,QString> *PARAMS;
    static QHash<QString,QString> *CONFIG_MAPPING;

    bool checkParams();
    bool checkConfig();
    void setupParams();
    void setupConfig();
    void createMapping();
    static bool settingsAvailable();
    void loadPathVariables();

    QMessageBox *warning;
    QString pathMetaPathways;

    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

private slots:
    void openSetup();
    void openParentWidget();
    void openResultWindow();

    void displayProgress();
    void displayWelcome();
    void displayResults();
    void showResults();

    void continueFromSetupWidget();
    void continueFromParentSettingsWidget();

private:
    Ui::MainFrame *ui;
    ParentWidget *parentWidget;
    Setup *setupWidget;
    ResultWindow *resultWindow;
    ProgressDialog *progress;
    Welcome* welcomeWindow;

    QWidget *centralWidget;
    QStackedWidget* stackedWidget;

    QAction* actionSetup;
    QAction* actionRunSettings;
    QAction* actionProgress;
    QAction* actionGridProgress;
    QAction* actionResults;
    QAction* actionSetupMenu;
    QAction* actionAbout;

    QToolBar* toolBar;
    QToolBar* leftToolBar;

    QMenu* menuSetup;
    QMenu* menuAbout;




};

#endif // MAINFRAME_H
