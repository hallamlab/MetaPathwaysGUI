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

bool ExportSource::saveTableToFile(QString fileName, QChar delim) {
    switch(this->index) {
        case 0:
             return td0->saveTableToFile(fileName, delim);
             break;
        case 1:
             return td1->saveTableToFile(fileName, delim);
             break;
        default:
            return UNKNOWN;
    }

}
