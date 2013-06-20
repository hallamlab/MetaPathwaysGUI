#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>

namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
    
private:
    Ui::ResultWidget *ui;
};

#endif // RESULTWIDGET_H
