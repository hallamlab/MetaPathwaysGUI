#ifndef POPUPLISTENER_H
#define POPUPLISTENER_H

#include <QWidget>
#include <QDebug>

class PopupListener : public QWidget
{
    Q_OBJECT
public:
    explicit PopupListener(QWidget *parent = 0);
    
signals:
    
public slots:
    void clickedOnCell(int row, int col);
    
};

#endif // POPUPLISTENER_H
