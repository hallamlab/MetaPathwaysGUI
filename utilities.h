#ifndef UTILITIES_H
#define UTILITIES_H
#include "QHash"
#include "qstring.h"

class Utilities
{
public:
    Utilities();
    static QHash<QString,QString>* parseConfig(QString TEMPLATE_CONFIG);
    static QHash<QString,QString>* parseParams(QString TEMPLATE_PARAM);
};

#endif // UTILITIES_H
