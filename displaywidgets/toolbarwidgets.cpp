#include "toolbarwidgets.h"
#include <QPixmap>
#include <QImage>
#include <QDebug>

ToolBarQLabel::ToolBarQLabel(const QString & text, SIGNALTYPE type) : QLabel(text)
{
    this->type = type;
    connect( this, SIGNAL( clicked(SIGNALTYPE) ), this, SLOT( slotClicked(SIGNALTYPE) ) );
}
void ToolBarQLabel::mousePressEvent ( QMouseEvent * event )
{
    emit clicked(this->type);
}
void ToolBarQLabel::slotClicked( SIGNALTYPE type)
{
    qDebug()<<"Clicked " << this->type;
}


HTabWidget::HTabWidget(QString text, QString imageFile) {

    QImage image(imageFile);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    ToolBarQLabel* lab1 = new ToolBarQLabel(" X", CROSS);
    lab1->setPixmap(QPixmap::fromImage(image));
   // lab1->resize(10,10);
    lab1->setAlignment(Qt::AlignCenter);

    layout->addWidget(lab1);
    ToolBarQLabel* lab2 = new ToolBarQLabel(text, ACTIVE);
    //lab2->resize(60,10);
    lab2->setAlignment(Qt::AlignLeft);
    layout->addWidget(lab2);

    this->setStyleSheet("padding:0px; margin:1px; border:1px solid rgb(0, 255, 0); ");
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    this->show();
}

