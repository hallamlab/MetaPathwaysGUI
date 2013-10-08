#ifndef WIDGETSTACKER_H
#define WIDGETSTACKER_H
#include <QWidget>
#include <QPoint>

class WidgetStacker
{
public:
    static WidgetStacker *getWidgetStacker();
    void setReferenceCoordinate(QPoint point);
    void stackWidget(QWidget *w);

private:
    static  WidgetStacker *widgetStacker;

    QPoint refPoint ;
    double deltaX, deltaY;
    WidgetStacker();
};

#endif // WIDGETSTACKER_H
