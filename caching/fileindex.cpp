#include "fileindex.h"


FileIndex::FileIndex()
{
}


FileIndex::FileIndex(QString inputFilePath, QString sampleName, RESOURCE type)
{
    QFile inputFile(inputFilePath);
    if (!QFile::exists(inputFilePath))
    {
       // QMessageBox::warning(this, "File to index is missing for sample " + sampleName, inputFilePath, QMessageBox::Ok);
        return;
    }
    else if (!inputFile.open(QIODevice::ReadOnly))
    {
        //QMessageBox::warning(this, QString("File to index could not be read for sample ") + sampleName, inputFilePath, QMessageBox::Ok);
        return;
    }
    QTextStream stream(&inputFile);
    this->sourceFile = inputFilePath;
    this->sampleName = sampleName;
    this->type = type;

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
        line  = inputFile.readLine().trimmed();
        prevPos = pos;
        pos =  pos + line.length() + 1;

        if( line.indexOf(">") ==0 ) {
            if( readyToInsert) {
                this->locations.insert(name, s);
            }
            s.beg = prevPos;
            if(this->type==NUCFASTA)
                name = Utilities::getShortContigId(FileIndex::extractFastaSeqName(line, sep));
            if(this->type==NUCFNA)
                name = Utilities::getShortORFId(FileIndex::extractFastaSeqName(line, sep));
            if(this->type==AMINOFAA)
                name = Utilities::getShortORFId(FileIndex::extractFastaSeqName(line, sep));
            readyToInsert = true;
        }
    }
    s.end = pos;

  //  qDebug() << "Begin " << s.beg << "  End " << s.end;

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

   // qDebug() << "A " << s.beg << "  " << s.end;

    while( !inputFile.atEnd() && pos < s.end) {
        line  = inputFile.readLine();
        result.append(line);
        pos = pos + line.size() + 1;
    }
    inputFile.close();

    return result;
}


bool FileIndex::writeFileIndex(QString filePath, SOURCETYPE type) {
    QFile outputFile(filePath +".tmp");

    QHash<QString, FILELOCSPAN>::const_iterator it;
    QString line;
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream outstream(&outputFile);
        for(it = this->locations.begin(); it!= this->locations.end(); it++) {
            line = it.key() + "\t" + QString::number(it.value().beg) + "\t" + QString::number(it.value().end);
            outstream << line << endl;
        }
        QFile  oldfile(filePath);
        if( QFile::exists(filePath) ) {
            oldfile.remove();
            oldfile.remove();
        }
        outputFile.rename(filePath);
        outputFile.close();
       return true;
    }

    return false;

}


QString FileIndex::getSourceFile() {
    return this->sourceFile;
}

void FileIndex::setSourceFile(QString sourceFile) {
    this->sourceFile = sourceFile;
}

bool FileIndex::loadFileIndex(QString filePath, SOURCETYPE type) {
    QFile inputFile(filePath);
    if (! QFile::exists(filePath) )
    {
        // warn that the file doesn't exist
        return false;
    }
    else if (!inputFile.open(QIODevice::ReadOnly))
    {
        // warn that the file couldn't be read
        return  false;
    }


    if(type==FASTA) {
       FILELOCSPAN s;
       while( !inputFile.atEnd()) {
           QString line  = inputFile.readLine().trimmed();
           QStringList fields = line.split(QChar('\t'));
           if( fields.size() !=3 ) continue;
           s.beg =  fields[1].trimmed().toInt();
           s.end =  fields[2].trimmed().toInt();
           this->locations.insert(fields[0].trimmed(), s);
       }
       inputFile.close();
    }
    return true;
}


FileIndex::~FileIndex()
{
    this->locations.clear();
}
