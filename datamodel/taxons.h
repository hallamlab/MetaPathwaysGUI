#ifndef TAXONS_H
#define TAXONS_H

#include <QHash>
#include <QString>
#include <QFile>
#include <QRegExp>

#include "helper/types.h"
#include "datamodel/datamodeltypes.h"

class Taxons
{
public:
    static Taxons * getTaxons();
    void addTaxons(const QString &fileName, unsigned int col);
    void addTaxon(const QString &taxon);
    ATTRIBUTE *getTaxon(const QString &taxon);
    bool hasTaxon(const QString &taxon);
    void clearTaxons();

private:
    Taxons();
    static Taxons *taxons;

    QHash < QString, ATTRIBUTE * > taxonHash;

    QHash<QString , bool> sourceFiles;

};

#endif // TAXONS_H
