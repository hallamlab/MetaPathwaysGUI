#ifndef SELECTSAMPLES_H
#define SELECTSAMPLES_H

#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QDialogButtonBox>
#include "resultwindow.h"


class ResultWindow;

namespace Ui {
class SelectSamples;
}

class SelectSamples : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectSamples(QWidget *parent = 0);
    ~SelectSamples();
    void addSamples(QStringList samples);
    void setReceiver(ResultWindow *resultWindow  );


private slots:
    void sendSelection();

public slots:
    void selectAllSlot();
    void clearAllSlot();
    
private:
    Ui::SelectSamples *ui;
    QGridLayout *sampleGrid;
    QDialogButtonBox *decisionBox;
    ResultWindow *resultWindow;
    QList<QCheckBox *> checkboxes;
    QPushButton *selectAll, *clearAll;
};

#endif // SELECTSAMPLES_H
