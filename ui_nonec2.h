/********************************************************************************
** Form generated from reading UI file 'nonec2.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NONEC2_H
#define UI_NONEC2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NonEC2
{
public:
    QGridLayout *gridLayout;
    QSpinBox *max_parallel_jobs;
    QLabel *bitLabel;
    QLineEdit *name;
    QLabel *osLabel;
    QLabel *userLabel;
    QLineEdit *server;
    QLabel *working_dirLabel;
    QLineEdit *working_directory;
    QLineEdit *user;
    QLabel *serverLabel;
    QComboBox *bit;
    QLabel *max_parallel_jobsLabel;
    QLabel *nameLabel;
    QComboBox *os;
    QLabel *inputLabel;
    QCheckBox *enabled;

    void setupUi(QWidget *NonEC2)
    {
        if (NonEC2->objectName().isEmpty())
            NonEC2->setObjectName(QString::fromUtf8("NonEC2"));
        NonEC2->resize(676, 316);
        gridLayout = new QGridLayout(NonEC2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        max_parallel_jobs = new QSpinBox(NonEC2);
        max_parallel_jobs->setObjectName(QString::fromUtf8("max_parallel_jobs"));
        max_parallel_jobs->setMaximumSize(QSize(500, 16777215));
        max_parallel_jobs->setMinimum(3);
        max_parallel_jobs->setMaximum(999);
        max_parallel_jobs->setValue(20);

        gridLayout->addWidget(max_parallel_jobs, 1, 1, 1, 1);

        bitLabel = new QLabel(NonEC2);
        bitLabel->setObjectName(QString::fromUtf8("bitLabel"));

        gridLayout->addWidget(bitLabel, 6, 0, 1, 1);

        name = new QLineEdit(NonEC2);
        name->setObjectName(QString::fromUtf8("name"));
        name->setReadOnly(false);

        gridLayout->addWidget(name, 0, 1, 1, 1);

        osLabel = new QLabel(NonEC2);
        osLabel->setObjectName(QString::fromUtf8("osLabel"));

        gridLayout->addWidget(osLabel, 5, 0, 1, 1);

        userLabel = new QLabel(NonEC2);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));

        gridLayout->addWidget(userLabel, 2, 0, 1, 1);

        server = new QLineEdit(NonEC2);
        server->setObjectName(QString::fromUtf8("server"));
        server->setReadOnly(false);

        gridLayout->addWidget(server, 3, 1, 1, 1);

        working_dirLabel = new QLabel(NonEC2);
        working_dirLabel->setObjectName(QString::fromUtf8("working_dirLabel"));

        gridLayout->addWidget(working_dirLabel, 4, 0, 1, 1);

        working_directory = new QLineEdit(NonEC2);
        working_directory->setObjectName(QString::fromUtf8("working_directory"));
        working_directory->setReadOnly(false);

        gridLayout->addWidget(working_directory, 4, 1, 1, 1);

        user = new QLineEdit(NonEC2);
        user->setObjectName(QString::fromUtf8("user"));
        user->setReadOnly(false);

        gridLayout->addWidget(user, 2, 1, 1, 1);

        serverLabel = new QLabel(NonEC2);
        serverLabel->setObjectName(QString::fromUtf8("serverLabel"));

        gridLayout->addWidget(serverLabel, 3, 0, 1, 1);

        bit = new QComboBox(NonEC2);
        bit->setObjectName(QString::fromUtf8("bit"));

        gridLayout->addWidget(bit, 6, 1, 1, 1);

        max_parallel_jobsLabel = new QLabel(NonEC2);
        max_parallel_jobsLabel->setObjectName(QString::fromUtf8("max_parallel_jobsLabel"));

        gridLayout->addWidget(max_parallel_jobsLabel, 1, 0, 1, 1);

        nameLabel = new QLabel(NonEC2);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        gridLayout->addWidget(nameLabel, 0, 0, 1, 1);

        os = new QComboBox(NonEC2);
        os->setObjectName(QString::fromUtf8("os"));

        gridLayout->addWidget(os, 5, 1, 1, 1);

        inputLabel = new QLabel(NonEC2);
        inputLabel->setObjectName(QString::fromUtf8("inputLabel"));
        QFont font;
        font.setPointSize(14);
        inputLabel->setFont(font);

        gridLayout->addWidget(inputLabel, 7, 1, 1, 1);

        enabled = new QCheckBox(NonEC2);
        enabled->setObjectName(QString::fromUtf8("enabled"));

        gridLayout->addWidget(enabled, 7, 0, 1, 1);


        retranslateUi(NonEC2);

        QMetaObject::connectSlotsByName(NonEC2);
    } // setupUi

    void retranslateUi(QWidget *NonEC2)
    {
        NonEC2->setWindowTitle(QApplication::translate("NonEC2", "Form", 0, QApplication::UnicodeUTF8));
        bitLabel->setText(QApplication::translate("NonEC2", "System Bit Type", 0, QApplication::UnicodeUTF8));
        name->setText(QString());
        osLabel->setText(QApplication::translate("NonEC2", "Operating System", 0, QApplication::UnicodeUTF8));
        userLabel->setText(QApplication::translate("NonEC2", "Username", 0, QApplication::UnicodeUTF8));
        server->setText(QApplication::translate("NonEC2", "myserveraddress", 0, QApplication::UnicodeUTF8));
        working_dirLabel->setText(QApplication::translate("NonEC2", "Working Dir (optional)", 0, QApplication::UnicodeUTF8));
        working_directory->setText(QString());
        user->setText(QApplication::translate("NonEC2", "myusername", 0, QApplication::UnicodeUTF8));
        serverLabel->setText(QApplication::translate("NonEC2", "Server Address", 0, QApplication::UnicodeUTF8));
        bit->clear();
        bit->insertItems(0, QStringList()
         << QApplication::translate("NonEC2", "bit32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NonEC2", "bit64", 0, QApplication::UnicodeUTF8)
        );
        max_parallel_jobsLabel->setText(QApplication::translate("NonEC2", "Max Parallel Jobs", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QApplication::translate("NonEC2", "Grid Name", 0, QApplication::UnicodeUTF8));
        os->clear();
        os->insertItems(0, QStringList()
         << QApplication::translate("NonEC2", "linux", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NonEC2", "win", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("NonEC2", "mac", 0, QApplication::UnicodeUTF8)
        );
        inputLabel->setText(QApplication::translate("NonEC2", "All input must not contain spaces!", 0, QApplication::UnicodeUTF8));
        enabled->setText(QApplication::translate("NonEC2", "Enabled", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NonEC2: public Ui_NonEC2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NONEC2_H
