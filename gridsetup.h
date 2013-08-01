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
    QStackedWidget* wid;
    QToolButton* addGrid;
    QToolButton* deleteGrid;
    QPushButton* cancelButton;
    QPushButton* saveButton;

    QHash<QString, EC2Grid*> *EC2Grids;
    QHash<QString, NonEC2*> *NonEC2Grids;

    GridChoice *gc;

    void initGridValues();
    void initSelectChoices();
    void populateValues();

public slots:
    void initForm(QString selected);

    void addNewGrid();
    void deleteExistingGrid();
    void closeWindow();
    void saveAndClose();

    void newEC2Form();
    void newNonEC2Form();

private:
    Ui::GridSetup *ui;
};

#endif // GRIDSETUP_H
