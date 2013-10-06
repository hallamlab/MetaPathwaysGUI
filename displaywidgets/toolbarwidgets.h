#ifndef TOOLBARWIDGETS_H
#define TOOLBARWIDGETS_H
#include <QString>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QHBoxLayout>

typedef enum _SIGNALYPE { CROSS, ACTIVE } SIGNALTYPE;
class ToolBarQLabel: public QLabel {
       Q_OBJECT
public:
    ToolBarQLabel( const QString & text, SIGNALTYPE type );
private:
    SIGNALTYPE type;
protected:
    void mousePressEvent ( QMouseEvent * event );
signals:
    void clicked(SIGNALTYPE type);
public slots:
    void slotClicked(SIGNALTYPE type);

};


class HTabWidget: public QWidget {
public:
    HTabWidget(QString text, QString imageFile);

};



#endif // TOOLBARWIDGETS_H
