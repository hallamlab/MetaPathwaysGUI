#ifndef GRIDSETUP_H
#define GRIDSETUP_H

#include <QWidget>

namespace Ui {
class GridSetup;
}

class GridSetup : public QWidget
{
    Q_OBJECT
    
public:
    explicit GridSetup(QWidget *parent = 0);
    ~GridSetup();
    
private:
    Ui::GridSetup *ui;
};

#endif // GRIDSETUP_H
