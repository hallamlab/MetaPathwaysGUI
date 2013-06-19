#ifndef PARENTWIDGET_H
#define PARENTWIDGET_H

#include <QWidget>

namespace Ui {
class ParentWidget;
}

class ParentWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParentWidget(QWidget *parent = 0);
    ~ParentWidget();
    
private:
    Ui::ParentWidget *ui;
};

#endif // PARENTWIDGET_H
