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
 * Creates a new file with filename TEMPLATE_FILE. Copies over old lines and the new specified KEY, VALUE
 * pairing for settings.
 */

bool Utilities::writeSettingToFile(QString TEMPLATE_FILE, QString KEY, QString VALUE){
    QFile inputFile(TEMPLATE_FILE);
    QFile newFile( TEMPLATE_FILE + "_new" );

    if (inputFile.open(QIODevice::ReadWrite) && newFile.open(QIODevice::ReadWrite))
    {
       QTextStream in(&inputFile);
       QTextStream out(&newFile);

       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            QRegExp comment("#[^\"\\n\\r]*");
            QRegExp splitRegex("\\s");
            QRegExp keepRegex("\\w+");
            bool found = false;

            if (!comment.exactMatch(line)){
                //if the line doesn't begin with a comment hash
                QStringList splitList = line.split(splitRegex);
                //then, split the line up by white space
                splitList = splitList.filter(keepRegex);
                //filter the line into a list, keeping only non-whitespace characters

                if (splitList.count() > 0){
                    //if the list has a key and value for us to use
                    if (splitList.at(0) == KEY ){
                        splitList.replace(1,VALUE);
                        out << splitList.at(0) << " " << splitList.at(1) << "\n";
                        found = true;
                    }
                }
            }
            if (!found) out << line << "\n";
       }
       inputFile.close();
       inputFile.remove(TEMPLATE_FILE);
       //delete old file

       newFile.rename(TEMPLATE_FILE + "_new", TEMPLATE_FILE);
       newFile.close();
       //set new file with the proper name and close
       return true;
    }
    return false;
}

/*
 * For parsing the template config or parameter file.
 * Returns a hash of String to Strings, denoting the settings key, value pairs.
 */
QHash<QString,QString>* Utilities::parseFile(QString TEMPLATE_FILE){
    QFile inputFile(TEMPLATE_FILE);
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

