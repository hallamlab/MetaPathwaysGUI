#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    
private:
    Ui::Welcome *ui;
};

#endif // WELCOME_H
