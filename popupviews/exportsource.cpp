#include "exportsource.h"


ExportSource::ExportSource(HTableData *td) {
    this->index = 1;
    this->td1 = td;
}

ExportSource::ExportSource(TableData *td) {
    this->index=0;
    this->td0 = td;
}


QStringList ExportSource::getHeaders() {
    switch(this->index) {
        case 0:
             return td0->getHeaders();
             break;
        case 1:
             return td1->getHeaders();
             break;
        default:
            return QStringList();
    }

}


QStringList ExportSource::getSampleNames() {
    switch(this->index) {
        case 0:
             return td0->getSampleNames();
             break;
        case 1:
             return td1->getSampleNames();
             break;
        default:
            return QStringList();
    }

}


QString ExportSource::getSampleName(unsigned int i) {
    switch(this->index) {
        case 0:
             return td0->getSampleName(i);
             break;
        case 1:
             return td1->getSampleName(i);
             break;
        default:
            return QString();
    }

}


QString ExportSource::getHeader(unsigned int i) {
   switch(this->index) {
       case 0:
            return td0->getHeader(i);
            break;
       case 1:
            return td1->getHeader(i);
            break;
       default:
           return QString();
   }

}


enum TYPE ExportSource::getFieldType(unsigned int i) {
   switch(this->index) {
       case 0:
            return td0->getFieldType(i);
            break;
       case 1:
            return td1->getFieldType(i);
            break;
       default:
           return UNKNOWN;
   }
}

bool ExportSource::saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders) {
    switch(this->index) {
        case 0:
             return td0->saveTableToFile(fileName, delim, selectedHeaders);
             break;
        case 1:
             return td1->saveTableToFile(fileName, delim, selectedHeaders);
             break;
        default:
            return UNKNOWN;
    }

}

/**
 * @brief ExportSource::saveSequencesToFile, exports sequences to file
 * @param sampleName, the name of the sample to export for
 * @param fileName, the name of the file
 * @param type, type of sequences, Contig, Untranslated ORF or amino acid sequences.
 * @return
 */
bool ExportSource::saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type) {
    switch(this->index) {
        case 0: // functional-and-tax tables
             return td0->saveSequencesToFile(fileName, type);
             break;
        case 1: //KEGG, COG, TABLE
             return td1->saveSequencesToFile(sampleName, fileName, type);
             break;
        default:
            return UNKNOWN;
    }
}

/**
 * @brief ExportSource::saveMeganExportToFile, saves the LAST/BLAST lines for exporting into MEGAN
 * @param sampleName
 * @param fileName
 * @param type, either MEGANBLASTFILE or MEGANLASTFILE
 * @return
 */
bool ExportSource::saveMeganExportToFile(QString sampleName, QString fileName,  RESOURCE type) {
    switch(this->index) {
        case 0: // functional-and-tax tables
             return td0->saveMeganExportToFile(fileName, type);
             break;
        case 1: //KEGG, COG, TABLE

             return td1->saveMeganExportToFile(sampleName, fileName, type);
             break;
        default:
            return UNKNOWN;
    }
}


bool ExportSource::isMultiSampleMode() {
    switch(this->index) {
        case 0:
             return td0->isMultiSampleMode();
             break;
        case 1:
             return td1->isMultiSampleMode();
             break;
        default:
            return false;
    }
}


unsigned int ExportSource::getIndex() {
    return this->index;

}


HTableData *ExportSource::getHTableDataPointer() {
    return this->td1;
}

TableData *ExportSource::getTableDataPointer() {
    return this->td0;
}
