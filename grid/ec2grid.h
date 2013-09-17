#ifndef EC2GRID_H
#define EC2GRID_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include "grid/grid.h"

namespace Ui {
class EC2Grid;
}

class Grid;

class EC2Grid : public QWidget
{
    Q_OBJECT
    
public:

    QLineEdit* cluster_name;
    QComboBox* bit;
    QCheckBox* enabled;
    QSpinBox* max_parallel_jobs;
    QComboBox* os;
    QLineEdit* username;
    QLineEdit* working_dir;
    QLineEdit* keyfile;
    QLineEdit* amazon_aws_config;

    explicit EC2Grid(QWidget *parent = 0);
    void populateValues(const Grid *g, const QString &selected);
    ~EC2Grid();
    
private:
    Ui::EC2Grid *ui;
};

#endif // EC2GRID_H
