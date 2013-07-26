#ifndef GRIDSETUP_H
#define GRIDSETUP_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include "ec2grid.h"
#include "grid.h"
#include "nonec2.h"

namespace Ui {
class GridSetup;
}

class GridSetup : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridSetup(QWidget *parent = 0);
    ~GridSetup();
    QComboBox* gridSelection;
    QLabel* startMessage;
    QWidget* wid;

    void initGridValues();
    void initSelectChoices();

public slots:
    void initForm(QString selected);

private:
    Ui::GridSetup *ui;
};

#endif // GRIDSETUP_H
