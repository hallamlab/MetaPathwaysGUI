#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResultWindow(QWidget *parent = 0);
    ~ResultWindow();

private slots:
    void exportData();

private:
    Ui::ResultWindow *ui;

    QTabWidget *resultTabs;
    QWidget *aminoWidget;
    QWidget *nucWidget;

    QTableWidget *nucTable;
    QTableWidget *aminoTable;

    QPushButton *exportAmino;
    QPushButton *exportNuc;
};

#endif // RESULTWINDOW_H
