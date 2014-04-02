#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "dataviews/tabledata.h"
#include "datamodel/htabledata.h"
#include "popupviews/exportsource.h"

namespace Ui {
class SearchWidget;
}

class TableData;

class ExportSource;
class SearchWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit SearchWidget(TableData* td = 0, QWidget *parent = 0);
    explicit SearchWidget(HTableData* td = 0, QWidget *parent = 0);
    ~SearchWidget();
    
private slots:
    void querySet();

signals:
    void lookUp(QString, int, bool);

private:
    void createWidget();
private:
    Ui::SearchWidget *ui;

    QLineEdit* keyword;
    QComboBox* columnSelect;
    QCheckBox* caseSensitive;
    QDialogButtonBox* okAndCancel;

    ExportSource* td;


    QList< int> index;
};

#endif // SEARCHWIDGET_H
