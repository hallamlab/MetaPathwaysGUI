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
    ~Setup();

public slots:
    void pythonBrowse();
    void perlBrowse();
    void metapathwaysBrowse();
    void saveSetup();
    void cancelSetup();

private:
    Ui::Setup *ui;

    QString pythonPath;
    QString perlPath;
    QString mpPath;

    QLabel *pythonLabel;
    QLabel *perlLabel;
    QLabel *metapathwaysLabel;

    QPushButton *pythonBrowseButton;
    QPushButton *perlBrowseButton;
    QPushButton *metapathwaysBrowseButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
};

#endif // Setup_H
