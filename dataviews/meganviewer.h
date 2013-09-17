#ifndef MEGANVIEWER_H
#define MEGANVIEWER_H
#include <QString>
#include <QList>
#include <QStringList>
#include <QChar>
#include <QStack>

#include <QEvent>


typedef struct _TREENODE  TREENODE;

typedef struct _TREENODE {
    QList<TREENODE *> children;
    QString name;
    unsigned int count;
} TREENODE;



class MeganViewer
{
public:
    MeganViewer();
    void setData(const QString &data);
    void createTreeView();

private:
    QStringList splitByDelimiter(const QString &str, const QChar &sep, bool removeEmptyStrings = true);
    QStringList splitByDelimiter(const QStringList &strlist, const QChar &sep);

    void createTreeStructure(QStringList &tokens);
    TREENODE *createTree(QStringList &tokens);
    void tokenize(QStringList &tokens);
    void printTree(TREENODE *root, int i);
    void addAttributes(TREENODE *p, QString str) ;

    QString data;
    TREENODE *root;

};

#endif // MEGANVIEWER_H
