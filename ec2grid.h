#ifndef EC2GRID_H
#define EC2GRID_H

#include <QWidget>

namespace Ui {
class EC2Grid;
}

class EC2Grid : public QWidget
{
    Q_OBJECT
    
public:
    explicit EC2Grid(QWidget *parent = 0);
    ~EC2Grid();
    
private:
    Ui::EC2Grid *ui;
};

#endif // EC2GRID_H
