#ifndef UTILITIES_H
#define UTILITIES_H
#include "QHash"
#include "qstring.h"

class Utilities
{
public:
    Utilities();
    static QHash<QString,QString>* createMapping();
    static QHash<QString,QString>* parseFile(QString TEMPLATE_FILE);
    static bool writeSettingToFile(QString TEMPLATE_FILE, QString KEY, QString VALUE);
    static int countRunSteps(QHash<QString,QString>* PARAMS);
};

#endif // UTILITIES_H
