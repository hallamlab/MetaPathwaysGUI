#ifndef UTILITIES_H
#define UTILITIES_H
#include "QHash"
#include "qstring.h"
#include <QLabel>
#include <QTableWidget>

class Utilities
{
public:
    Utilities();
    static QList<QLabel *>* createLabels(QString FILE_NAME);
    static QTableWidget* createTable(QString FILE_NAME);
    static QList<QString>* parseResults(QString FILE_NAME, QChar DELIM);
    static QHash<QString,QString>* createMapping();
    static QHash<QString,QString>* parseFile(QString TEMPLATE_FILE);
    static bool writeSettingToFile(QString TEMPLATE_FILE, QString KEY, QString VALUE);
    static int countRunSteps(QHash<QString,QString>* PARAMS);
};

#endif // UTILITIES_H
