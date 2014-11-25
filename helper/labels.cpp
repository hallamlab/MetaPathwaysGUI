#include "labels.h"

Labels* Labels::labels  = 0;
HTABLE_PARAMS  *Labels::tableParams = 0;
QHash<ATTRTYPE, QString> *Labels::tabNames = 0;

Labels::Labels()
{

  QHash<ATTRTYPE, QString> function1, function2;

  function1[KEGG] = "KEGG Function";
  function2[KEGG] = "KEGG Function (alias)";

  function1[COG] = "COG Category";
  function2[COG] = "COG Category (Alias)";

  function1[METACYC] = "Pathway/Reaction";
  function2[METACYC] = "Common name";

  function1[SEED] = "SEED Subsystem Category";
  function2[SEED] = "SEED Subsystem (Alias)";

  function1[CAZY] = "CAZY Family";
  function2[CAZY] = "CAZY Family (Alias)";


  QHash<VALUETYPE, QString> valuetypes;
  valuetypes[ORFCOUNT] = "ORF Count";
  valuetypes[RPKMCOUNT] = "RPKM Count";
  valuetypes[BASEPWY_ORF] = "ORF Count";
   valuetypes[BASEPWY_RPKM] = "RPKM Count";
  valuetypes[BASEPWY_RXNCOV] = "# Reactions Covered";
  valuetypes[BASEPWY_RXNTOT] = "# Total Reactions";

  tableParams = new HTABLE_PARAMS;

  foreach(ATTRTYPE attr, function1.keys() ) {
      foreach(VALUETYPE vtype,  valuetypes.keys() ) {
          Labels::tableParams->headers[attr][vtype] << function1[attr] << function2[attr] << valuetypes[vtype];
      }
  }


  tabNames = new QHash<ATTRTYPE, QString>;
  tabNames->insert(KEGG, "KEGG");
  tabNames->insert(COG, "COG");
  tabNames->insert(METACYC, "MetaCyc");
  tabNames->insert(SEED, "SEED");
  tabNames->insert(CAZY, "CAZY");

}


/**
 * @brief Labels::getLabels, singleton constructor
 * @return
 */
Labels *Labels::getLabels() {

  if( Labels::labels == 0 ) {
     Labels::labels = new Labels;
  }

  return Labels::labels;
}

/**
 * @brief getHtableHeader, returns the header for the function and quantity type for the HTables
 * @param attrType
 * @param valueType
 * @return
 */
QStringList Labels::getHtableHeader(ATTRTYPE attrType, VALUETYPE  valueType) {
  if( Labels::tableParams->headers.contains(attrType) &&  Labels::tableParams->headers[attrType].contains(valueType) )
    return Labels::tableParams->headers[attrType][valueType];
  else
    return QStringList();
}


/**
 * @brief Labels::getTabName, get the name of the tab
 * @param attrType
 * @return
 */
QString Labels::getTabName(ATTRTYPE attrType) {
  if( Labels::tabNames->contains(attrType) )
     return Labels::tabNames->value(attrType);
  else
    return QString("UNKNOWN");
}


