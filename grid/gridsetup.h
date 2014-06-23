#ifndef GRIDSETUP_H
#define GRIDSETUP_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QToolButton>

#include "ec2grid.h"
#include "gridchoice.h"
#include "grid.h"
#include "nonec2.h"
#include "helper/rundata.h"

namespace Ui {
class GridSetup;
}

class GridChoice;

class GridSetup : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridSetup(QWidget *parent = 0);
    ~GridSetup();
    QComboBox* gridSelection;
    QLabel* startMessage;
    QLabel* inputLabel;
    QStackedWidget* wid;
    QToolButton* addGrid;
    QToolButton* deleteGrid;
    QPushButton* cancelButton;
    QPushButton* saveButton;
    QHash<QString, QString> defaultParamValues;
    QHash<QString, bool> allowedParams;

    QHash<QString, Grid*> *Grids;
    QList<QWidget *> *allWidgets;

    GridChoice *gc;

    void getWidgetValues(const Grid &g);
    void initGridValues();
    void initSelectChoices();
    void populateValues();

    void setDefaultParamValues() ;

    void writeGridSettingToFile(const QString &TEMPLATE_FILE);

public slots:
    void initForm(const QString &selected);
    void changeForm(QString selected);

    void addNewGrid();
    void deleteExistingGrid();
    void closeWindow();
    void saveAndClose();
    bool validateGridSettings(QString &errMsg) ;


    void newEC2Form();
    void newNonEC2Form();

private:
    Ui::GridSetup *ui;
};

#endif // GRIDSETUP_H
