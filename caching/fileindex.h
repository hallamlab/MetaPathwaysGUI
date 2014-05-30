#ifndef FILEINDEX_H
#define FILEINDEX_H
#include "helper/types.h"
#include "helper/utilities.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>
#include <QMessageBox>


#include <QDebug>

typedef struct FILELOCATIONSPAN {
    unsigned int beg;
    unsigned int end;
} FILELOCSPAN;

class FileIndex
{

public:
    FileIndex();
    FileIndex(QString intputFilePath, QString sampleName,  RESOURCE type);
    ~FileIndex();
    QString getDataToDisplay(QString &key);
    void setSourceFile(QString sourceFile);
    QString getSourceFile();
    bool loadFileIndex(QString filePath, enum SOURCETYPE type);
    bool writeFileIndex(QString filePath, enum SOURCETYPE type);
    QString sourceFile;
    QString sampleName;
    RESOURCE type;
private:
    void indexFastaFile(QTextStream &inputFile);
    QString extractFastaSeqName(QString line, QRegExp &sep) ;
    QHash<QString, FILELOCSPAN> locations;
};

#endif // FILEINDEX_H
