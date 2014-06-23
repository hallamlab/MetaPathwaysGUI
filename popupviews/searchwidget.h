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
    void toggleANDOR(bool );

signals:
    void lookUp(QString,int, QString, int, QString, int, QString, int, OPTYPE, bool);

private:
    void createWidget();

private:
    Ui::SearchWidget *ui;

    QLineEdit* keyword_1, *keyword_2, *keyword_3, *keyword_4;
    QComboBox* columnSelect_1, *columnSelect_2, *columnSelect_3, *columnSelect_4;
    QLabel *andORlabel_1, *andORlabel_2, *andORlabel_3, *andORlabel_4;

    QCheckBox *andOR;
    QCheckBox* caseSensitive;
    QDialogButtonBox* okAndCancel;

    ExportSource* td;


    QList< int> index;
};

#endif // SEARCHWIDGET_H
