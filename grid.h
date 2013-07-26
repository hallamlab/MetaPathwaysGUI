#ifndef GRID_H
#define GRID_H

#include <QString>
#include <QHash>

class Grid
{
public:
    Grid();
    QHash<QString,QString> *values;

    bool enabled;
//    QString AWS_key_id;
//    QString AWS_secret_access_key;
//    QString AWS_user_id;
//    QString keyfile;
//    QString amazon_aws_config;
//    QString cluster_name;

//    QString max_parallel_jobs;
//    QString name;
//    QString user;
//    QString server;
//    QString gridName;
//    QString os;
//    QString bit;
//    QString type;
//    QString working_dir;
};

#endif // GRID_H
