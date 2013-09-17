#ifndef DISPLAYINFO_H
#define DISPLAYINFO_H

#include <QDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QDebug>
#include <QTableWidgetItem>
#include "types.h"
#include "caching/fileindex.h"
#include <QHash>

namespace Ui {
class DisplayInfo;
}

class DisplayInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit DisplayInfo(QWidget *parent = 0);
    ~DisplayInfo();
    
public slots:
    void clickedOnCell(int row, int col);
    void itemDoubleClicked(QTableWidgetItem * item );
    void addFileIndex(FileIndex *fileIndex, unsigned int);
    void removeFileIndexes();


private:
    Ui::DisplayInfo *ui;
    QTextEdit * textOut;
    QDialogButtonBox * okAndCancel;
    QString sourceFile;
    SOURCETYPE sourceType;
    QHash<unsigned int, FileIndex *>fileIndexes;

};

#endif // DISPLAYINFO_H
