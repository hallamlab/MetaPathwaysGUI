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


