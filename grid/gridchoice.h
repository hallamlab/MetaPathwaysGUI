#ifndef GRIDCHOICE_H
#define GRIDCHOICE_H

#include <QWidget>
#include <QPushButton>
#include "gridsetup.h"

namespace Ui {
class GridChoice;
}

class GridSetup;

class GridChoice : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridChoice(QWidget *parent = 0, GridSetup const *g = 0);
    QPushButton* EC2Instance;
    QPushButton* NonEC2Instance;
    ~GridChoice();
    
private:
    Ui::GridChoice *ui;
};

#endif // GRIDCHOICE_H
