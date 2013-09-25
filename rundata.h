#ifndef RUNDATA_H
#define RUNDATA_H

#include <QHash>
#include <QString>
#include <QProcess>

class RunData
{
public:
    void setParams(QHash<QString,QString>* PARAMS);
    void setConfig(QHash<QString,QString>* CONFIG);
    void setConfigMapping(QHash<QString,QString>* CONFIG_MAPPING);
    void setProcess(QProcess* run);
    void setRRNADBS(QStringList* rrnaDBS);
    void setAnnotationDBS(QStringList* annotationDBS);

    QHash<QString,QString>* getParams();
    QHash<QString,QString>* getConfig();
    QHash<QString,QString>* getConfigMapping();
    QProcess* getProcess();

    static RunData* getRunData();

    QStringList *files;
    int nRRNADB;
    int nADB;

private:
    RunData();
    static RunData* runData;

    QHash<QString,QString> *PARAMS;
    QHash<QString,QString> *CONFIG;
    QHash<QString,QString> *CONFIG_MAPPING;
    QStringList *rrnaDBS;
    QStringList *annotationDBS;
    QProcess* run;
};


#endif // RUNDATA_H
