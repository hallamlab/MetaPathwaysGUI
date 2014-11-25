#ifndef RUNDATA_H
#define RUNDATA_H

#include <QHash>
#include <QString>
#include <QProcess>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QSysInfo>
#include <QtGlobal>

#include "types.h"
#include "utilities.h"
#include "constants.h"


class RunData: public QObject
{
    Q_OBJECT
public:

    static const QString TEMPLATE_PARAM;
    static const QString DEFAULT_TEMPLATE_PARAM;

    static const QString TEMPLATE_CONFIG;
    static const QString DEFAULT_TEMPLATE_CONFIG;

    void setPythonExecutablePath(QString pythonPath);
    void setPgdbFolderPath(QString pgdbFolderPath);
    void setMetaPathwaysPath(QString mpPath);
    void setDatabasesPath(QString databasePath);

    void setParams(QHash<QString,QString> PARAMS);
    void setConfig(QHash<QString,QString> CONFIG);
    void setTempConfig(QHash<QString,QString> TEMP_CONFIG);

    bool checkPythonBinary(QString &message, QString path = QString());
    bool checkMetaPathwaysPath(QString &message, QString metapathwaysDir);
    bool checkBinaries(QString &message, QString executablesDir);
    bool checkRefDatabasesPath(QString &message,  QString refdbsDir);
    bool checkPGDBFolderPath(QString &message, QString pgdbFolderPath);
    bool checkPathwayToolsExecutable(QString &message, QString executable);



    QProcessEnvironment getProcessEnvironment(QString METAPATH);

    void setConfigMapping(QHash<QString,QString> CONFIG_MAPPING);
    void setProcess(QProcess *process);
    QProcess * getProcess();
    void setRRNADBS(QStringList rrnaDBS);
    void setAnnotationDBS(QStringList annotationDBS);
    void setPathologicPath(QString pathologicPath);

    void setSamplesSubsetToRun(QList<QString> &selection);
    QList<QString> getSamplesSubsetToRun();

    QString getValueFromHash(QString key, SETTING_TYPE type);
    void setValue(QString key, QString value, SETTING_TYPE type);

    QHash<QString,QString> getParams();
    QHash<QString,QString> getConfig();
    QHash<QString,QString> getTempConfig();
    QHash<QString,QString> getConfigMapping();


    void setNumRRNADB(unsigned int n);
    void setNumADB(unsigned int n);

    void setupDefaultParams();
    void loadDefaultParams();
    void setupDefaultConfig();
    bool validate(QString &warningMsg);
    bool checkConfig();
    bool checkParams();
    void setCurrentSample(QString currentSample);
    int  getRunningStepNumber();

    QString getCurrentInputFormat();

    QString getCurrentSample();
    QString getSampleFolder(QString sampleName);

    QHash<QString, QString> getSampleFolderMap();

    void addToProcessedSamples(QString sampleName);
    bool isAlreadyProcessedSample(QString sampleName);
    void clearProcessedSamples();
    int numberOfProcessedSamples() ;

    static RunData* getRunData();

    void addFileToList(QString file);
    void setFileList(QStringList files);
    QStringList getFileList(const QString &fileType);
   //   QStringList getFileList();

    void updateCurrentFileList();
    QStringList getCurrentFileList();

    void loadInputFiles(const QString &folder);
     //   void loadInputFiles();
    void loadOutputFolders();
    QStringList getOutputFolders();
    bool isOutputFolderValid(const QString &folder);
    QStringList getSubFolders(const QString & folder);
    QString getSystem();
    void setResultFolders(QStringList folders);


    unsigned int getNumRRNADB();
    unsigned int getNumADB();
    QStringList getrNADBNames();
    QStringList getADBNames();

    QHash<QString, QString> getPARAMS();
    void emitloadSampleList();

    bool saveContext(const QString &key, QVariant value);
    bool hasContext(const QString &key);
    QVariant getContext(const QString &key);

    void setDirtyBit(QString key);
    void clearsDirtyBit(QString key);
    bool isBitDirty(QString key);

    void triggerParameterFileRead();

signals:
    void loadSampleList();
    void loadParameters();

public slots:

    void updateInputFileFormat(QString currentInputFileFormat) ;


private:
    RunData();
    void initVartoNULL();

    int nRRNADB;
    int nADB;

    static RunData* runData;

    QStringList files, currentfiles;
    QHash<QString,QString> PARAMS;
    QHash<QString, QString> PARAMS_DEFAULT;

public:
    QHash<QString,QString> CONFIG, TEMP_CONFIG;
private:

    QHash<QString,QString> CONFIG_MAPPING;
    QStringList rrnaDBS;
    QStringList annotationDBS;
    QProcess *process;

    QString currentSample;
    QHash<QString, bool> processedSamples;

    QString system;
    QList<QString> selectSamplesToRun;
    QHash<QString, QString> outputFolders;
    QString currentInputFormat;
    QHash<QString, bool> dirtybits;

    QStringList resultFolders;
};


#endif // RUNDATA_H
