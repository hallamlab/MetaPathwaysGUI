#ifndef NONEC2_H
#define NONEC2_H

#include <QWidget>

namespace Ui {
class NonEC2;
}

class NonEC2 : public QWidget
{
    Q_OBJECT
    
public:
    explicit NonEC2(QWidget *parent = 0);
    ~NonEC2();
    
private:
    Ui::NonEC2 *ui;
};

#endif // NONEC2_H
