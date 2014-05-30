#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTimer>

#include "helper/rundata.h"

namespace Ui {
class ResultPage;
}

class ResultPage : public QWidget
{
    Q_OBJECT
    
public:
    QList<QGroupBox *> *groupBoxes;
    QTableWidget *table;
    QStringList selectedFile;
    QLabel *fileSelectedPath;
    QHash<int,QString> *TABLE_MAPPING;

    explicit ResultPage(RunData *run = 0, QWidget *parent = 0);
    ~ResultPage();

public slots:
    void updateSteps();
    void updateFile(QString changed);

private:
    void loadRunParams();
    void initMapping();
    void colorRunConfig(QString line);
    void setStyling();

    int blastCount;
    int parseBlastCount;
    int scanRRNACount;
    int statsRRNACount;

    RunData *run;
    QTimer *timer;
    QString currentFile;

    Ui::ResultPage *ui;
};

#endif // RESULTPAGE_H
