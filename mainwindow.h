#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qstring.h"
#include "RemoteForm.h"
#include "ui_mainwindow.h"
#include "qhash.h"
#include "utilities.h"
#include "parentwidget.h"
#include "SettingsTab.h"
#include "Setup.h"

class Setup;

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

    bool checkParams();
    bool checkConfig();
    void setupParams();
    void setupConfig();
    void createMapping();
    void openSetup();

    explicit MainWindow();

    ~MainWindow();

public slots:
    void startRun();

public:
    Setup *setupWindow;
    ParentWidget *parentWidget;
};

#endif // MAINWINDOW_H
