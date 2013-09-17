#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QGroupBox>
#include "dataviews/tabledata.h"

namespace Ui {
class ExportWidget;
}

class TableData;

class ExportWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExportWidget(TableData *td = 0,QWidget *parent = 0);
    ~ExportWidget();
    
private:
    Ui::ExportWidget *ui;
    TableData *td;
    QRadioButton *radioButtonTsv;
    QRadioButton *radioButtonCsv;
};

#endif // EXPORTWIDGET_H
