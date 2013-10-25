#ifndef RUNDATA_H
#define RUNDATA_H

#include <QHash>
#include <QString>
#include <QProcess>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QFileInfo>
#include "types.h"
#include "utilities.h"

class RunData
{
public:

    static const QString TEMPLATE_PARAM;
    static const QString DEFAULT_TEMPLATE_PARAM;

    static const QString TEMPLATE_CONFIG;
    static const QString DEFAULT_TEMPLATE_CONFIG;

    void setPythonExecutablePath(QString pythonPath);
    void setPerlExecutablePath(QString perlPath);
    void setMetaPathwaysPath(QString mpPath);
    void setDatabasesPath(QString databasePath);

    void setParams(QHash<QString,QString> PARAMS);
    void setConfig(QHash<QString,QString> CONFIG);
    void setConfigMapping(QHash<QString,QString> CONFIG_MAPPING);
    void setProcess(QProcess *process);
    void setRRNADBS(QStringList rrnaDBS);
    void setAnnotationDBS(QStringList annotationDBS);

    QString getValueFromHash(QString key, SETTING_TYPE type);
    void setValue(QString key, QString value, SETTING_TYPE type);

    QHash<QString,QString> getParams();
    QHash<QString,QString> getConfig();
    QHash<QString,QString> getConfigMapping();
    QProcess* getProcess();

    void setNumRRNADB(unsigned int n);
    void setNumADB(unsigned int n);

    void setupDefaultParams();
    void setupDefaultConfig();
    bool validate(QString &warningMsg);
    bool checkConfig();
    bool checkParams();
    void setCurrentSample(QString currentSample);
    QString getCurrentSample();

    static RunData* getRunData();

    void addFileToList(QString file);
    void setFileList(QStringList files);
    QStringList getFileList();

    unsigned int getNumRRNADB();
    unsigned int getNumADB();

private:
    RunData();
    void initVartoNULL();

    int nRRNADB;
    int nADB;

    static RunData* runData;

    QStringList files;
    QHash<QString,QString> PARAMS;
    QHash<QString,QString> CONFIG;
    QHash<QString,QString> CONFIG_MAPPING;
    QStringList rrnaDBS;
    QStringList annotationDBS;
    QProcess *process;
    QString currentSample;
};


#endif // RUNDATA_H
