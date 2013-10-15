#ifndef METAWIDGET_H
#define METAWIDGET_H

#include <QString>
#include <QLabel>

class MetaWidget
{

public:
    MetaWidget();
    virtual void setupFromFile(const QString &file) = 0;

    QLabel* titleLabel;
    QLabel* statsLabel;
    QString file;

};

#endif // METAWIDGET_H
