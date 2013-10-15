#include "widgetstacker.h"

WidgetStacker *WidgetStacker::widgetStacker =0;


WidgetStacker *WidgetStacker::getWidgetStacker() {
    if( WidgetStacker::widgetStacker==0)
        WidgetStacker::widgetStacker = new WidgetStacker;
    return WidgetStacker::widgetStacker;

}

WidgetStacker::WidgetStacker()
{
    this->deltaX = 20;
    this->deltaY = 20;
    this->lastw =0;
}

void WidgetStacker::setReferenceCoordinate(QPoint point) {
    this->refPoint = point;
}

void WidgetStacker::stackWidget(QWidget *w) {
     QPoint p;

     if(this->lastw!=0) {
         this->refPoint.setX(this->lastw->x());
         this->refPoint.setY(this->lastw->y());
     }

     p.setX( this->refPoint.x() + deltaX);
     p.setY( this->refPoint.y() + deltaY);
     w->move(p.x(), p.y());
     this->refPoint = p;
     w->show();
     this->lastw = w;
}
