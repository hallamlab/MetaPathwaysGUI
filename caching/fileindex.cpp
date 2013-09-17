#include "fileindex.h"


FileIndex::FileIndex()
{
}


FileIndex::FileIndex(QString inputFilePath, enum SOURCETYPE type)
{
    QFile inputFile(inputFilePath);
    if (!QFile::exists(inputFilePath))
    {
        // warn that the file doesn't exist
        return;
    }
    else if (!inputFile.open(QIODevice::ReadOnly))
    {
        // warn that the file couldn't be read
        return;
    }
    QTextStream stream(&inputFile);
    sourceFile = inputFilePath;

    this->indexFastaFile(stream);
    inputFile.close();

}


QString  FileIndex::extractFastaSeqName(QString line, QRegExp &sep) {
    line = line.remove(0,1);
    QStringList names=  line.split(QRegExp("[\\t\\s]"));
    return names[0];
 }

void FileIndex::indexFastaFile(QTextStream &inputFile) {

    QRegExp sep("[\\t\\s]");
    QString line;
    unsigned int pos=0, prevPos=0;
    QString name;

    FILELOCSPAN s;
    bool readyToInsert = false;
    while( !inputFile.atEnd()) {
        s.end = pos;
        line  = inputFile.readLine();
        prevPos = pos;
        pos =  pos + line.length() + 1;

        if( line.indexOf(">") ==0 ) {
            if( readyToInsert) {
                this->locations.insert(name, s);
            }
            s.beg = prevPos;
            name = FileIndex::extractFastaSeqName(line, sep);
            readyToInsert = true;
        }
    }
    this->locations.insert(name, s);
}


QString FileIndex::getDataToDisplay(QString &key) {
    FILELOCSPAN s;
    if( !locations.contains(key)) QString("Data not found!");
    s = locations[key];


    QFile inputFile(sourceFile);
    if (! QFile::exists(sourceFile) )
    {
        // warn that the file doesn't exist
        return QString("Data file not found!");;
    }
    else if (!inputFile.open(QIODevice::ReadOnly))
    {
        // warn that the file couldn't be read
        return  QString("Data not could not be opened!");
    }
    QTextStream stream(&inputFile);
    stream.seek(s.beg);
    QString line;
    QString result("");
    unsigned int pos = s.beg;
    while( !inputFile.atEnd() && pos < s.end) {
        line  = inputFile.readLine();
        result.append(line);
        pos = pos + line.size() + 1;
    }
    return result;
}

