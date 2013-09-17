#ifndef FILEINDEX_H
#define FILEINDEX_H
#include "types.h"
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>

#include <QDebug>

typedef struct FILELOCATIONSPAN {
    unsigned int beg;
    unsigned int end;
} FILELOCSPAN;

class FileIndex
{
public:
    FileIndex();
    FileIndex(QString intputFilePath, enum SOURCETYPE type);
    QString getDataToDisplay(QString &key);

private:
    void indexFastaFile(QTextStream &inputFile);
    QString extractFastaSeqName(QString line, QRegExp &sep) ;
    QHash<QString, FILELOCSPAN> locations;
    QString sourceFile;
};

#endif // FILEINDEX_H
