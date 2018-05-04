#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QWidget>
#include "ui_DashBoard.h"


namespace Ui {
 class DashBoard;
}


class DashBoard : public QWidget
{
    Q_OBJECT
public:
       static DashBoard *getDashBoard();
       ~DashBoard();
private:
      static DashBoard *dashboard;
      explicit DashBoard(QWidget *parent = 0);
      QWidget *parent;
      Ui::DashBoard *ui;

signals:

public slots:
};

#endif // DASHBOARD_H
