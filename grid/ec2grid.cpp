#include "ec2grid.h"
#include "ui_ec2grid.h"
#include "mainwindow.h"
#include <QDebug>

EC2Grid::EC2Grid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EC2Grid)
{
    ui->setupUi(this);
}

void EC2Grid::populateValues(const Grid *g, const QString &selected){
    QHash<QString,QString>* values = g->values;
    QList<QWidget *>::iterator i;
    QList<QWidget *> formValues = this->findChildren<QWidget *>();

    for (i=formValues.begin(); i!= formValues.end(); ++i){
        QWidget *widget = *i;
        QString value = values->value(widget->objectName());

        if (qobject_cast<QComboBox *>(widget)!=0){
            QComboBox *temp = qobject_cast<QComboBox *>(widget);
            temp->setCurrentIndex(temp->findText(value));
            if (temp->currentIndex()==-1){
                temp->addItem(value);
            }
        }
        else if (qobject_cast<QLineEdit *>(widget)!=0){
            QLineEdit *temp = qobject_cast<QLineEdit *>(widget);
            temp->setText(value);
        }
        else if (qobject_cast<QCheckBox *>(widget)!=0){
            QCheckBox *temp = qobject_cast<QCheckBox *>(widget);
            if (g->enabled==true){
                temp->setChecked(true);
            }
        }
        else if (qobject_cast<QSpinBox *>(widget)!=0){
            QSpinBox *temp = qobject_cast<QSpinBox *>(widget);
            temp->setValue(value.toInt());
        }
    }
}


EC2Grid::~EC2Grid()
{
    delete ui;
}
