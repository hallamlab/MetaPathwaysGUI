#include "nonec2.h"
#include "ui_nonec2.h"
#include <QDebug>
#include "mainframe.h"

NonEC2::NonEC2(QWidget *parent) : QWidget(parent), ui(new Ui::NonEC2)
{
    ui->setupUi(this);

    bit = this->findChild<QComboBox *>("bit");
    enabled = this->findChild<QCheckBox *>("enabled");
    max_parallel_jobs = this->findChild<QSpinBox *>("max_parallel_jobs");
    os = this->findChild<QComboBox *>("os");
    server = this->findChild<QLineEdit *>("server");
    user = this->findChild<QLineEdit *>("user");
    working_dir = this->findChild<QLineEdit *>("working_directory");
}

void NonEC2::populateValues(const Grid *g, const QString &selected){
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
            temp->show();
        }
    }
}

NonEC2::~NonEC2()
{
    delete ui;
}
