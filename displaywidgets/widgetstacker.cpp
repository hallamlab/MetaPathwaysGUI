#include "widgetstacker.h"

WidgetStacker *WidgetStacker::widgetStacker =0;


WidgetStacker *WidgetStacker::getWidgetStacker() {
    if( WidgetStacker::widgetStacker==0)
        WidgetStacker::widgetStacker = new WidgetStacker;
    return WidgetStacker::widgetStacker;

}

WidgetStacker::WidgetStacker()
{
    this->deltaX = 10;
    this->deltaY = 10;
}

void WidgetStacker::setReferenceCoordinate(QPoint point) {
    this->refPoint = point;
}

void WidgetStacker::stackWidget(QWidget *w) {
     QPoint p;

     p.setX( this->refPoint.x() + deltaX);
     p.setY( this->refPoint.y() + deltaY);
     this->refPoint = p;
     w->show();


}
