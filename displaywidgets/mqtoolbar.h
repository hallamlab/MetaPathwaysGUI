#ifndef MQTOOLBAR_H
#define MQTOOLBAR_H

#include <QToolBar>
#include <QDebug>
#include <QObject>
#include <QEvent>
#include <QWidget>

class MQToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MQToolBar(QWidget *parent = 0);

private:
    void makeSmall();
protected:
   // bool eventFilter(QObject *object, QEvent *event);
   virtual void enterEvent ( QEvent * event);
   virtual void  leaveEvent ( QEvent * event);

    
};

#endif // MQTOOLBAR_H
