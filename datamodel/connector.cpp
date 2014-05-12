#include "connector.h"

Connector::Connector()
{

}

Connector::~Connector() {


}

QList<ATTRIBUTE *> Connector::getSinks(ATTRIBUTE *) {
    QList<ATTRIBUTE *> results;

    return results;
}

void Connector::addToList(ATTRIBUTE *attribute, ORF* orf) {
    this->connected[attribute].append(orf);
}


void Connector::setAttrType(ATTRTYPE attrType) {
    this->attrType = attrType;
}


ATTRTYPE Connector::getAttrType() {
    return this->attrType;
}


QList<ORF *> Connector::getORFList(ATTRIBUTE *attribute) {

    if(this->connected.contains(attribute)) {

        return this->connected[attribute];
    }
    else {
        return QList<ORF*>();
    }

}


QList<ORF *> Connector::getORFList(QList<ATTRIBUTE *> attributes) {
    QList<ORF *> list;
    foreach(ATTRIBUTE *attribute, attributes) {
        if(this->connected.contains(attribute))
           list.append(this->connected[attribute]);
    }
    return list;
}



QList<ORF *> Connector::getORFList() {
    QHash<ORF *, bool> listHash;

    foreach(ATTRIBUTE *attribute, this->connected.keys()) {
        foreach(ORF *orf, this->connected[attribute])
            if(!listHash.contains(orf))
                listHash[orf] = true;
    }
    return listHash.keys();
}


unsigned int Connector::getNumOfORFs(ATTRIBUTE *attribute) {
    unsigned int size = 0;
    if(this->connected.contains(attribute))
       size += this->connected[attribute].size();
    return size;
}

unsigned int Connector::getNumOfORFs() {
    unsigned int size =0;
    foreach(ATTRIBUTE *attribute, this->connected.keys()) {
        if(this->connected.contains(attribute))
           size += this->connected[attribute].size();
    }
    return size;
}

unsigned int Connector::getNumOfORFs(QList<ATTRIBUTE *> attributes) {
    unsigned int size =0;
    foreach(ATTRIBUTE *attribute, attributes) {
        if(this->connected.contains(attribute))
           size += this->connected[attribute].size();
    }
    return size;
}

unsigned int Connector::getNumAttributes() {
    return connected.size();
}

