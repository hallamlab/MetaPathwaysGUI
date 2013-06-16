#ifndef UTILITIES_H
#define UTILITIES_H
#include "QHash"
#include "qstring.h"

class Utilities
{
public:
    Utilities();
    static QHash<QString,QString>* parseFile(QString TEMPLATE_FILE);
    static bool writeSettingToFile(QString TEMPLATE_FILE, QString KEY, QString VALUE);
};

#endif // UTILITIES_H
