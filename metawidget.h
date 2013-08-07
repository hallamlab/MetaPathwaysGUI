#ifndef METAWIDGET_H
#define METAWIDGET_H

#include <QString>
#include <QLabel>

class MetaWidget
{
public:
    MetaWidget();

    void setupFromFile(const QString &file);
    QLabel* titleLabel;
    QLabel* statsLabel;
    QString file;
};

#endif // METAWIDGET_H
