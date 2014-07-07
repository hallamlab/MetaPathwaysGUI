#include "taxons.h"

Taxons *Taxons::taxons = 0;

/**
 * @brief Taxons::getTaxons, implements the singleton feature
 * @return
 */
Taxons *Taxons::getTaxons() {

    if( Taxons::taxons ==0 ) {
        Taxons::taxons = new Taxons();
        Taxons::taxons->addTaxon(QString("root"));
    }
    return Taxons::taxons;
}

/**
 * @brief Taxons::addTaxons, adds the taxons from the file, if the
 * file already was added the skips
 * @param fileName, filename to read the taxons from;
 */
void Taxons::addTaxons(const QString &fileName, unsigned int col) {
    if( this->sourceFiles.contains(fileName)) return ;
    QFile inputFile(fileName);

    QRegExp comment("#[^\"\\n\\r]*");
    QChar delim = QChar('\t');
    QStringList colValues;
    unsigned int numCols = 0;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            if (comment.exactMatch(line)) continue;
            colValues = line.split(QRegExp(delim));
            numCols = colValues.length();
            if(numCols <= col ) continue;
            this->addTaxon(colValues[col]);
        }
    }
    else return;
    inputFile.close();
    this->sourceFiles[fileName] = true;
}



/**
 * @brief Taxons::addTaxon, adds a single taxon
 * @param taxon, the name of the tabon
 * @return void
 */
void Taxons::addTaxon(const QString &taxon) {
    if( this->taxonHash.contains(taxon) ) return;

    ATTRIBUTE *attribute = new ATTRIBUTE;
    attribute->name = taxon;
    this->taxonHash.insert(taxon, attribute);
}


/**
 * @brief Taxons::getTaxon, returns the pointer to the Taxon ATTRIBUTE with the
 * name taxon
 * @param taxon
 * @return pointer to the attributre for that taxons;
 */
ATTRIBUTE * Taxons::getTaxon(const QString &taxon) {
    if( !this->taxonHash.contains(taxon)) return 0;
    return this->taxonHash[taxon];

}

/**
 * @brief Taxons::hasTaxon, checks if a taxons is already in the list of taxons
 * @param taxon
 * @return
 */
bool Taxons::hasTaxon(const QString &taxon) {
    if( !this->taxonHash.contains(taxon)) return false;
    return true;
}


/**
 * @brief Taxons::clearTaxons, deletes all the taxons created so far and resets
 * the sourcefiles to empty
 */
void Taxons::clearTaxons() {
   foreach(QString key, this->taxonHash.keys() )
      delete this->taxonHash[key];

   this->taxonHash.clear();
   this->sourceFiles.clear();
}

/**
 * @brief Taxons::Taxons, the private constructor to design the singleton feature
 */
Taxons::Taxons()
{

}








