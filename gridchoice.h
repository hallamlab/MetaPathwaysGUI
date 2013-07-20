#ifndef GRIDCHOICE_H
#define GRIDCHOICE_H

#include <QWidget>

namespace Ui {
class GridChoice;
}

class GridChoice : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridChoice(QWidget *parent = 0);
    ~GridChoice();
    
private:
    Ui::GridChoice *ui;
};

#endif // GRIDCHOICE_H
