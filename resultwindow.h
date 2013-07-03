#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWindow(QWidget *parent = 0);
    ~ResultWindow();


private:
    Ui::ResultWindow *ui;

    QTabWidget *resultTabs;
    QWidget *aminoWidget;
    QWidget *nucWidget;

    QTableWidget *nucTable;
    QTableWidget *aminoTable;
};

#endif // RESULTWINDOW_H
