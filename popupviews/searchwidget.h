#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include "dataviews/tabledata.h"

namespace Ui {
class SearchWidget;
}

class TableData;

class SearchWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchWidget(TableData* td = 0, QWidget *parent = 0);
    ~SearchWidget();
    
private slots:
    void querySet();

signals:
    void lookUp(QString, int, bool);

private:
    Ui::SearchWidget *ui;

    QLineEdit* keyword;
    QComboBox* columnSelect;
    QCheckBox* caseSensitive;
    QDialogButtonBox* okAndCancel;

    TableData* td;
    QList< int> index;
};

#endif // SEARCHWIDGET_H
