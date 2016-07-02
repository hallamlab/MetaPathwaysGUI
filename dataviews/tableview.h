#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);
    
private:
    void keyPressEvent(QKeyEvent * event);
signals:
    
public slots:
    
};

#endif // TABLEVIEW_H
