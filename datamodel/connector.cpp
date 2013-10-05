#include "connector.h"

Connector::Connector()
{
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
    qDebug() << "list size " << this->connected.size();
    if(this->connected.contains(attribute)) {
        qDebug() << "attribute not bound ";
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
    QList<ORF *> list;

    foreach(ATTRIBUTE *attribute, this->connected.keys()) {
        if(this->connected.contains(attribute))
           list.append(this->connected[attribute]);
    }
    return list;
}


