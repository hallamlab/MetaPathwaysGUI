#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qstring.h"
#include "Setup.h"
#include "RemoteForm.h"
#include "ui_mainwindow.h"
#include "qhash.h"
#include "utilities.h"
#include "parentwidget.h"
#include "SettingsTab.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

    void checkParams();
    bool checkConfig();

    void loadParams(QString TEMPLATE_PARAM);
    void loadConfig(QString TEMPLATE_CONFIG);
    void createMapping();

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    void openSetup();
    void openSettings();
    void openRemoteJob();

private:
    QPushButton *setupButton;
    QPushButton *startButton;
    QPushButton *remoteButton;
    QLabel *warningLabel;
    QGraphicsView *logo;

    Ui::MainWindow *ui;
    Setup *setupWindow;
    SettingsTab *settingsWindow;
    RemoteForm *remoteWindow;
    ParentWidget *parentWidget;
};

#endif // MAINWINDOW_H
