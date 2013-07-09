#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include "rundata.h"

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

private slots:
    void updateSteps();

private:
    void loadRunParams();
    void initMapping();
    void colorRunConfig(QString line);
    void setStyling();
    RunData *run;
    QTimer *timer;

    Ui::ResultPage *ui;
};

#endif // RESULTPAGE_H
