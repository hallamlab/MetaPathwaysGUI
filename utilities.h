#ifndef UTILITIES_H
#define UTILITIES_H
#include <QHash>
#include <QString>
#include <QLabel>
#include <QTableWidget>

class TableNumberItem : public QTableWidgetItem{

public:
    TableNumberItem(const QString txt = QString("0")) :QTableWidgetItem(txt) {}
    bool operator <(const QTableWidgetItem &other) const;
};

class Utilities
{
public:
    Utilities();
    static bool validateConfig(const QHash<QString,QString>* PARAMS);
    static QList<QLabel *>* createLabels(const QString &FILE_NAME,const QChar &DELIM);
    static QTableWidget* createTable(const QString &FILE_NAME, const QChar &DELIM);
    static QList<QString>* parseResults(const QString &FILE_NAME, const QChar &DELIM);
    static QHash<QString,QString>* createMapping();
    static QHash<QString,QString>* parseFile(const QString &TEMPLATE_FILE);
    static bool writeSettingToFile(const QString &TEMPLATE_FILE, const QString &KEY, const QString &VALUE, const bool &CREATE, const bool &DELETE);
    static int countRunSteps(QHash<QString,QString>* PARAMS);
    static void getUniqueDBS(QStringList dbs, QStringList* &uniqueDBS);
};

#endif // UTILITIES_H
