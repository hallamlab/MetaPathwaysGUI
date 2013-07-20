#ifndef RUNDATA_H
#define RUNDATA_H

#include <QHash>
#include <QString>
#include <QProcess>

class RunData
{
public:
    RunData();
    RunData(QHash<QString,QString> *PARAMS,
            QHash<QString,QString> *CONFIG,
            QHash<QString,QString> *CONFIG_MAPPING,
            QProcess *run);

    QHash<QString,QString>* getParams();
    QHash<QString,QString>* getConfig();
    QHash<QString,QString>* getConfigMapping();
    QHash<QString,QString>* getRunResults();
    QStringList* getRRNADBS();
    QStringList* getAnnotationDBS();
    void setRRNADBS(QStringList* rrnaDBS);
    void setAnnotationDBS(QStringList* annotationDBS);
    QProcess* getProcess();
    void setProcess(QProcess* run);


private:
    QHash<QString,QString> *PARAMS;
    QHash<QString,QString> *CONFIG;
    QHash<QString,QString> *CONFIG_MAPPING;
    QHash<QString,QString> *RUN_RESULTS;
    QStringList *rrnaDBS;
    QStringList *annotationDBS;
    QProcess* run;

};


#endif // RUNDATA_H
