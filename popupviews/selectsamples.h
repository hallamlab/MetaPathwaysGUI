#ifndef SELECTSAMPLES_H
#define SELECTSAMPLES_H

#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QDialogButtonBox>
#include <QScrollArea>

#include "tabs/resultwindow.h"
#include "tabs/RunConfig.h"
#include "helper/rundata.h"


class ResultWindow;
class RunConfig;

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
    void addSamplesUnchecked(QStringList samples);
    void setReceiver(ResultWindow *resultWindow  );
    void setReceiver(RunConfig *runconfigWindow  );
private:
    void resizeWidget();
    void closeWidget();


private slots:
    void sendSelection();
    void closeOnCancel();
    void sampleClicked(int i);

public slots:
    void selectAllSlot();
    void clearAllSlot();
    
private:
    Ui::SelectSamples *ui;
    QGridLayout *sampleGrid;
    QDialogButtonBox *decisionBox;
    ResultWindow *resultWindow;
    RunConfig *runconfigWindow;
    QList<QCheckBox *> checkboxes;
    QPushButton *selectAll, *clearAll;
    QStringList samples;
    QList<unsigned int> clickedSamples;
    QScrollArea *scroll;
};

#endif // SELECTSAMPLES_H
