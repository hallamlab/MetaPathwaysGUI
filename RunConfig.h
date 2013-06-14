#ifndef RUNCONFIG_H
#define RUNCONFIG_H

#include <QWidget>

namespace Ui {
class RunConfig;
}

class RunConfig : public QWidget
{
    Q_OBJECT
    
public:
    explicit RunConfig(QWidget *parent = 0);
    ~RunConfig();
    
private:
    Ui::RunConfig *ui;
};

#endif // RUNCONFIG_H
