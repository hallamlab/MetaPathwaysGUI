#include "utilities.h"
#include "iostream"
#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qregexp.h"
#include "qhash.h"
#include "qsplitter.h"

Utilities::Utilities()
{
}

/*
 * For parsing the template config file. Returns a hash of String to Strings, denoting the
 * settings key, value pairs.
 */
QHash<QString,QString>* Utilities::parseConfig(QString TEMPLATE_CONFIG){
    QFile inputFile(TEMPLATE_CONFIG);
    QHash<QString, QString> *configs = new QHash<QString,QString>();

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();

            QRegExp comment("#[^\"\\n\\r]*");
            QRegExp splitRegex("\\s");
            QRegExp keepRegex("\\w+");

            if (!comment.exactMatch(line)){
                //if the line doesn't begin with a comment hash
                QStringList splitList = line.split(splitRegex);
                //then, split the line up by white space
                splitList = splitList.filter(keepRegex);
                //filter the line into a list, keeping only non-whitespace characters

                QString key;
                QString value;

                if (splitList.count() > 0){
                    //if the list has a key and value for us to use
                    if (splitList.at(0) != "") key = splitList.at(0);
                    value = splitList.at(1);

                    key.replace("'","");
                    value.replace("'","");

                    configs->insert(key,value);
                }
            }
       }
    }
    inputFile.close();
    return configs;
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
