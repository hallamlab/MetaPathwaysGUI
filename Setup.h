#ifndef Setup_H
#define Setup_H
#include "ui_Setup.h"
#include "mainwindow.h"
#include <QWidget>
#include <QString>
#include <QHash>

namespace Ui {
class Setup;
}

class MainWindow;

class Setup : public QWidget
{
    Q_OBJECT
    
public:
    explicit Setup( QWidget *parent,MainWindow *mw);
    void canSave();

    ~Setup();

public slots:
    void pythonBrowse();
    void perlBrowse();
    void metapathwaysBrowse();
    void saveSetup();
    void cancelSetup();
    void databaseBrowse();

signals:

private:
    Ui::Setup *ui;
    MainWindow *mw;

    QString pythonPath;
    QString perlPath;
    QString mpPath;
    QString databasePath;

    QLabel *pythonLabel;
    QLabel *perlLabel;
    QLabel *metapathwaysLabel;
    QLabel *databaseLabel;

    QPushButton *pythonBrowseButton;
    QPushButton *perlBrowseButton;
    QPushButton *metapathwaysBrowseButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *databaseButton;

};

#endif // Setup_H
