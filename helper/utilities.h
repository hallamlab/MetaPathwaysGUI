#ifndef UTILITIES_H
#define UTILITIES_H
#include <QHash>
#include <QString>
#include <QLabel>
#include <QTableWidget>
#include <QList>
#include <QThread>
#include <QTextFormat>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QHash>
#include <QSplitter>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QMessageBox>
#include <QDir>
#include <QVector>

#include "datamodel/datamodeltypes.h"
#include "helper/types.h"


class TableNumberItem : public QTableWidgetItem{

public:
    TableNumberItem(const QString txt = QString("0")) :QTableWidgetItem(txt) {}
    bool operator <(const QTableWidgetItem &other) const;
};


class Utilities
{
public:
    Utilities();
    static QList<QLabel *>* createLabels(const QString &FILE_NAME,const QChar &DELIM);
    static QTableWidget* createTable(const QString &FILE_NAME, const QChar &DELIM);
    static QList<QString>* parseResults(const QString &FILE_NAME, const QChar &DELIM);
    static QHash<QString,QString> createMapping();
    static QHash<QString,QString> parseFile(const QString &TEMPLATE_FILE, const QString TYPE);
    static QHash<QString,QString> parseFileConfig(const QString &CONFIG_FILE);
    static QString extractAttribute(QString line);

    static int countRunSteps(QHash<QString,QString>* PARAMS);
    static QStringList getUniqueDBS(QStringList dbs);
    static QStringList getSplitNames(QString nameString);
    static QString createToolTipTable(  QList< QList<QString> > tableData );
    static QLabel *ShowWaitScreen(QString pngFile, QString msg ) ;
    static QLabel *ShowWaitScreen(QString msg) ;
    static bool checkEmptyFile(QString filepath);

    static void read_metadata_file(QString metadatafile, QHash<QString, QStringList> &samples);


    static QString insertCharacterAtIntervals(QString string, QChar c, int n);
    static QString getShortORFId(const QString &orfname);
    static QString getShortContigId(const QString &orfname);
    static bool writeSettingToFile(const QString &TEMPLATE_FIL, const QString TYPE, const QString &KEY, const QString &VALUE, const bool &CREATE, const bool &DELET);


   static QString prepareToPrint(QString str, QChar delim);
    static QStringList getFilesWithPattern(const QString &folderName, const QRegExp &pattern);
    static unsigned int numNonZeros(const QVector<double> &v);
    static void removeZeros(QVector<double> &x, QVector<double> &y, unsigned int index = 0 );
    static unsigned int uniqueORFsCount(const QList<ORF *> &orfList);
    static QString getSubSequence(QString, QString, unsigned int start, unsigned int end);
    static bool compareFreq(const  TaxonFreqQPair &a, const TaxonFreqQPair &b);
    static void showInfo(QWidget *parent, QString info);
    static bool exportMeganCompatibleFile(QString inputFileName, QString outputFileName, QHash<QString, bool> orfNames);
    static void readOrfContigList(QString fileName, QString sampleName, QHash<QString, bool> &list, SEQNAME type);
};




#endif // UTILITIES_H
