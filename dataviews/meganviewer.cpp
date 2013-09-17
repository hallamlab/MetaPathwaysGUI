#include "meganviewer.h"
#include <QDebug>
MeganViewer::MeganViewer()
{
}



void MeganViewer::setData(const QString &data) {

   this->data  = data;

}

void MeganViewer::printTree(TREENODE *root, int i) {

    QString space;
    for(unsigned int j =0; j < i; j++) space.append(' ');
    qDebug() << i <<" " << space  << root->name << " " << root->count ;

    foreach(TREENODE *child, root->children) {
        printTree(child, i + 1);
    }

}

void MeganViewer::createTreeView() {
    QStringList tokens;
    this->tokenize(tokens);
    qDebug() << " last token " << tokens[tokens.size()-1];
    this->createTreeStructure(tokens);
    qDebug() << " first node " << root->name;
   // this->printTree(root, 0);
}

void MeganViewer::tokenize(QStringList &tokens) {
    QStringList tempTokens;
    QStringList tempTempTokens;
    tempTokens << this->splitByDelimiter(this->data, QChar('(')) ;
    foreach(QString tempToken, tempTokens) {
       tempTempTokens << this->splitByDelimiter(tempToken, QChar(')'));
    }
    foreach(QString tempToken, tempTempTokens) {
        tokens << this->splitByDelimiter(tempToken, QChar('{'));
    }

    for(unsigned int i =0; i < tokens.size(); i++) {
        if( tokens[i] == QString("{"))
            tokens[i] = QString(",");
    }
}

void MeganViewer::addAttributes(TREENODE *p, QString str) {
    QStringList tempTokens;
    tempTokens << str.split(QChar('}')) ;
    if( tempTokens.size() < 2 )
        p->count = 0;
    else {
        bool k;
        qDebug() << str << "  " << tempTokens[1];
        p->count = tempTokens[1].toInt(&k);
    }
    if( tempTokens.size() < 1)
        p->name = "Error";
    else
        p->name = tempTokens[0];

}

TREENODE *MeganViewer::createTree(QStringList &tokens) {
    QStack<TREENODE *> S;
    QString prev;
    TREENODE *node, *singleNode, *pnode;
    for(QStringList::const_iterator t = tokens.begin(); t != tokens.end(); t++ ) {
        if( *t == QString(",")) continue;

        if( *t == QString("(")) {
            node = new TREENODE;
            S.push(node);
        }

        if( (*t !=QString(")") && *t!=QString("(")) && prev!=QString(")")) {
            singleNode = new TREENODE;
            this->addAttributes(singleNode, *t);
            node->children.append(singleNode);
        }

        if( (*t !=QString(")") && *t!=QString("(")) && prev==QString(")")) {
            this->addAttributes(node, *t);

            node = S.pop();
            if(!S.empty()) {
                pnode = S.top();
                pnode->children.append(node);
                node = pnode;
            }
        }
        prev = *t;

    }
    qDebug() << "node " << node->name;
    return node;
}

void MeganViewer::createTreeStructure(QStringList &tokens) {
    root = createTree(tokens);
}

QStringList MeganViewer::splitByDelimiter(const QString &str, const QChar &sep, bool removeEmptyStrings) {
    QStringList results;
    QStringList tempStrings = str.split(sep);

    for(unsigned int i=0; i < tempStrings.size(); i++) {
        if( i > 0 ) results << QString(sep);
        if(!(tempStrings[i].isEmpty() && removeEmptyStrings) )
           results << tempStrings[i] ;
    }
    return results;
}

QStringList MeganViewer::splitByDelimiter(const QStringList &strlist, const QChar &sep) {
     QStringList results;
     foreach( QString s, strlist) {
         results << this->splitByDelimiter(s, sep);
     }
     return results;
}

