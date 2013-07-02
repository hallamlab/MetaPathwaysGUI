#ifndef Setup_H
#define Setup_H

#include <QWidget>
#include "ui_Setup.h"
#include "qstring.h"
#include "qhash.h"

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT
    
public:
    explicit Setup(QWidget *parent = 0);
    void canSave();
    ~Setup();

public slots:
    void pythonBrowse();
    void perlBrowse();
    void metapathwaysBrowse();
    void saveSetup();
    void cancelSetup();
    void databaseBrowse();


private:
    Ui::Setup *ui;

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
