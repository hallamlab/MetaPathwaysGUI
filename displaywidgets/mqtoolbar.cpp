#include "mqtoolbar.h"

MQToolBar::MQToolBar(QWidget *parent) :
    QToolBar(parent)
{

    this->makeSmall();
    this->show();
}



void MQToolBar::makeSmall() {
 //   QRect rect(this->pos().x(), this->pos().y(), 2,10);
   // this->setGeometry(rect);
}

void MQToolBar::leaveEvent ( QEvent * event) {
    this->makeSmall();
    qDebug() << "leaving ";
    QWidget::leaveEvent(event);
}

void MQToolBar::enterEvent ( QEvent * event) {
   // this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    qDebug() << "entering ";
    QWidget::enterEvent(event);
}
