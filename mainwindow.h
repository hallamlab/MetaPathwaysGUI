#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "utilities.h"
#include "parentwidget.h"
#include "SettingsTab.h"
#include "Setup.h"
#include "welcome.h"
#include "parentsettingsmainwindow.h"
#include "grid.h"
#include <QHash>
#include <QString>

class Setup;
class ParentSettingsMainWindow;

class MainWindow : public QObject
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
    static QHash<QString, Grid*> *GRIDS;

    bool checkParams();
    bool checkConfig();
    void setupParams();
    void setupConfig();
    void createMapping();

    explicit MainWindow();

    ~MainWindow();

public slots:
    void startRun();
    void openSetup();

public:
    Setup *setupWindow;
    QMessageBox *warning;
    ParentSettingsMainWindow *parent;
};

#endif // MAINWINDOW_H
