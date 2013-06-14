#ifndef Setup_H
#define Setup_H

#include <QWidget>

namespace Ui {
class Setup;
}

class Setup : public QWidget
{
    Q_OBJECT
    
public:
    explicit Setup(QWidget *parent = 0);
    ~Setup();
    
private:
    Ui::Setup *ui;
};

#endif // Setup_H
