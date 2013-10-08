#include "toolbarwidgets.h"
#include <QPixmap>
#include <QImage>
#include <QDebug>

ToolBarQLabel::ToolBarQLabel(const QString & text, SIGNALTYPE type) : QLabel(text)
{
    this->type = type;
    connect( this, SIGNAL( clicked(SIGNALTYPE) ), this, SLOT( slotClicked(SIGNALTYPE) ) );
  //  this->setStyleSheet("QLabel {padding-left:0px; margin-left:0px}");
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
    lab1->setAlignment(Qt::AlignLeft);
    lab1->setAlignment(Qt::AlignVCenter);
    lab1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    connect( lab1, SIGNAL( clicked(SIGNALTYPE) ), this, SLOT( slotClicked(SIGNALTYPE) ) );

    layout->addWidget(lab1);

    ToolBarQLabel* lab2 = new ToolBarQLabel(text, ACTIVE);
    //lab2->resize(60,10);
    lab2->setAlignment(Qt::AlignLeft);
    //lab1->setAlignment(Qt::AlignVCenter);
    layout->addWidget(lab2);
    layout->addStretch();


    this->setStyleSheet("QWidget {padding:0px; margin-left:0px;  border:1px grey; border-style:ridge}");
    this->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//    this->show();
}


void HTabWidget::slotClicked( SIGNALTYPE type)
{
    emit tabClicked(this, type);
}

