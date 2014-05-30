#ifndef MEGANEXPORTBOX_H
#define MEGANEXPORTBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QApplication>


#include "datamodel/samplercmanager.h"
#include "displaywidgets/progressview.h"
#include "helper/rundata.h"



class MeganExportBox : public QWidget
{
    Q_OBJECT
public:
    explicit MeganExportBox(QWidget *parent = 0);
    ~MeganExportBox();
    
private:
    void createWidget();
    void exportMeganTable(const QString &meganFile, const QString &fileName, RESOURCE resourceType);
    void exportMeganFile(const QString &meganFile, const QString &fileName);
    QString lastToBlastFormat(QString line, QChar &sep);
    int countLines(QString fileName);

signals:
    
public slots:
    void exportMeganFiles();

private:
    QRadioButton *lastRadio, *blastRadio;
    QCheckBox *aminoSequences;

};

#endif // MEGANEXPORTBOX_H
