#ifndef NONEC2_H
#define NONEC2_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include "grid.h"

namespace Ui {
class NonEC2;
}

class Grid;

class NonEC2 : public QWidget
{
    Q_OBJECT

public:
    explicit NonEC2(QWidget *parent = 0);
    void populateValues(const Grid *g, const QString &selected);

    QComboBox* bit;
    QCheckBox* enabled;
    QSpinBox* max_parallel_jobs;
    QComboBox* os;
    QLineEdit* server;
    QLineEdit* user;
    QLineEdit* working_dir;

    ~NonEC2();
    
private:
    Ui::NonEC2 *ui;
};

#endif // NONEC2_H
