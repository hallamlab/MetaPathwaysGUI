#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QWidget>

namespace Ui {
class ResultPage;
}

class ResultPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultPage(QWidget *parent = 0);
    ~ResultPage();
    
private:
    Ui::ResultPage *ui;
};

#endif // RESULTPAGE_H
