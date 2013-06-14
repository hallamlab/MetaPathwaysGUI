#include "utilities.h"
#include "iostream.h"
#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qregexp.h"

Utilities::Utilities()
{
}

/*
 * For parsing the template config file. Returns a hash of String to Strings, denoting the
 * settings key, value pairs.
 */
QHash<QString,QString>* Utilities::parseConfig(QString TEMPLATE_CONFIG){
    QFile inputFile(TEMPLATE_CONFIG);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            QRegExp comment("#[^\"\\n\\r]*");
            if (!comment.exactMatch(line)){
                qDebug() << line;
            }



       }
    }

    inputFile.close();
}

/*
 * For parsing the param file. Returns a hash of string to strings, denoting the parameters'
 * key, value pairs.
 */
QHash<QString,QString>* Utilities::parseParams(QString TEMPLATE_PARAM){
    QFile inputFile(TEMPLATE_PARAM);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();

       }
    }

    inputFile.close();
}
