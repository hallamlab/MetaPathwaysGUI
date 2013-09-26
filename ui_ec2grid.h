/********************************************************************************
** Form generated from reading UI file 'ec2grid.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EC2GRID_H
#define UI_EC2GRID_H

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
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EC2Grid
{
public:
    QGridLayout *gridLayout;
    QLabel *serverLabel;
    QLabel *bitLabel;
    QLineEdit *user;
    QLabel *working_dirLabel;
    QLineEdit *amazon_aws_config;
    QLabel *userLabel;
    QLineEdit *cluster_name;
    QLabel *osLabel;
    QLineEdit *keyfile;
    QPushButton *amazon_aws_configButton;
    QLabel *awsConfigLabel;
    QLabel *cluster_nameLabel;
    QPushButton *keyFileButton;
    QComboBox *os;
    QLabel *max_parallel_jobsLabel;
    QComboBox *bit;
    QLineEdit *working_directory;
    QCheckBox *enabled;
    QSpinBox *max_parallel_jobs;

    void setupUi(QWidget *EC2Grid)
    {
        if (EC2Grid->objectName().isEmpty())
            EC2Grid->setObjectName(QString::fromUtf8("EC2Grid"));
        EC2Grid->resize(673, 329);
        gridLayout = new QGridLayout(EC2Grid);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        serverLabel = new QLabel(EC2Grid);
        serverLabel->setObjectName(QString::fromUtf8("serverLabel"));

        gridLayout->addWidget(serverLabel, 3, 0, 1, 1);

        bitLabel = new QLabel(EC2Grid);
        bitLabel->setObjectName(QString::fromUtf8("bitLabel"));

        gridLayout->addWidget(bitLabel, 8, 0, 1, 1);

        user = new QLineEdit(EC2Grid);
        user->setObjectName(QString::fromUtf8("user"));
        user->setReadOnly(false);

        gridLayout->addWidget(user, 2, 1, 1, 1);

        working_dirLabel = new QLabel(EC2Grid);
        working_dirLabel->setObjectName(QString::fromUtf8("working_dirLabel"));

        gridLayout->addWidget(working_dirLabel, 6, 0, 1, 1);

        amazon_aws_config = new QLineEdit(EC2Grid);
        amazon_aws_config->setObjectName(QString::fromUtf8("amazon_aws_config"));
        amazon_aws_config->setReadOnly(false);

        gridLayout->addWidget(amazon_aws_config, 5, 1, 1, 1);

        userLabel = new QLabel(EC2Grid);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));

        gridLayout->addWidget(userLabel, 2, 0, 1, 1);

        cluster_name = new QLineEdit(EC2Grid);
        cluster_name->setObjectName(QString::fromUtf8("cluster_name"));
        cluster_name->setReadOnly(false);

        gridLayout->addWidget(cluster_name, 0, 1, 1, 1);

        osLabel = new QLabel(EC2Grid);
        osLabel->setObjectName(QString::fromUtf8("osLabel"));

        gridLayout->addWidget(osLabel, 7, 0, 1, 1);

        keyfile = new QLineEdit(EC2Grid);
        keyfile->setObjectName(QString::fromUtf8("keyfile"));
        keyfile->setReadOnly(false);

        gridLayout->addWidget(keyfile, 3, 1, 1, 1);

        amazon_aws_configButton = new QPushButton(EC2Grid);
        amazon_aws_configButton->setObjectName(QString::fromUtf8("amazon_aws_configButton"));

        gridLayout->addWidget(amazon_aws_configButton, 5, 2, 1, 1);

        awsConfigLabel = new QLabel(EC2Grid);
        awsConfigLabel->setObjectName(QString::fromUtf8("awsConfigLabel"));

        gridLayout->addWidget(awsConfigLabel, 5, 0, 1, 1);

        cluster_nameLabel = new QLabel(EC2Grid);
        cluster_nameLabel->setObjectName(QString::fromUtf8("cluster_nameLabel"));

        gridLayout->addWidget(cluster_nameLabel, 0, 0, 1, 1);

        keyFileButton = new QPushButton(EC2Grid);
        keyFileButton->setObjectName(QString::fromUtf8("keyFileButton"));

        gridLayout->addWidget(keyFileButton, 3, 2, 1, 1);

        os = new QComboBox(EC2Grid);
        os->setObjectName(QString::fromUtf8("os"));

        gridLayout->addWidget(os, 7, 1, 1, 1);

        max_parallel_jobsLabel = new QLabel(EC2Grid);
        max_parallel_jobsLabel->setObjectName(QString::fromUtf8("max_parallel_jobsLabel"));

        gridLayout->addWidget(max_parallel_jobsLabel, 1, 0, 1, 1);

        bit = new QComboBox(EC2Grid);
        bit->setObjectName(QString::fromUtf8("bit"));

        gridLayout->addWidget(bit, 8, 1, 1, 1);

        working_directory = new QLineEdit(EC2Grid);
        working_directory->setObjectName(QString::fromUtf8("working_directory"));
        working_directory->setReadOnly(false);

        gridLayout->addWidget(working_directory, 6, 1, 1, 1);

        enabled = new QCheckBox(EC2Grid);
        enabled->setObjectName(QString::fromUtf8("enabled"));

        gridLayout->addWidget(enabled, 9, 1, 1, 1);

        max_parallel_jobs = new QSpinBox(EC2Grid);
        max_parallel_jobs->setObjectName(QString::fromUtf8("max_parallel_jobs"));
        max_parallel_jobs->setMinimum(1);
        max_parallel_jobs->setMaximum(999);
        max_parallel_jobs->setValue(20);

        gridLayout->addWidget(max_parallel_jobs, 1, 1, 1, 1);

        QWidget::setTabOrder(cluster_name, max_parallel_jobs);
        QWidget::setTabOrder(max_parallel_jobs, user);
        QWidget::setTabOrder(user, keyfile);
        QWidget::setTabOrder(keyfile, keyFileButton);
        QWidget::setTabOrder(keyFileButton, amazon_aws_config);
        QWidget::setTabOrder(amazon_aws_config, amazon_aws_configButton);
        QWidget::setTabOrder(amazon_aws_configButton, working_directory);
        QWidget::setTabOrder(working_directory, os);
        QWidget::setTabOrder(os, bit);
        QWidget::setTabOrder(bit, enabled);

        retranslateUi(EC2Grid);

        QMetaObject::connectSlotsByName(EC2Grid);
    } // setupUi

    void retranslateUi(QWidget *EC2Grid)
    {
        EC2Grid->setWindowTitle(QApplication::translate("EC2Grid", "Form", 0, QApplication::UnicodeUTF8));
        serverLabel->setText(QApplication::translate("EC2Grid", "EC2 Keyfile", 0, QApplication::UnicodeUTF8));
        bitLabel->setText(QApplication::translate("EC2Grid", "System Bit Type", 0, QApplication::UnicodeUTF8));
        user->setText(QApplication::translate("EC2Grid", "sgeadmin", 0, QApplication::UnicodeUTF8));
        working_dirLabel->setText(QApplication::translate("EC2Grid", "Working Dir (optional)", 0, QApplication::UnicodeUTF8));
        userLabel->setText(QApplication::translate("EC2Grid", "Username", 0, QApplication::UnicodeUTF8));
        osLabel->setText(QApplication::translate("EC2Grid", "Operating System", 0, QApplication::UnicodeUTF8));
        amazon_aws_configButton->setText(QApplication::translate("EC2Grid", "Browse", 0, QApplication::UnicodeUTF8));
        awsConfigLabel->setText(QApplication::translate("EC2Grid", "Amazon AWS Config File", 0, QApplication::UnicodeUTF8));
        cluster_nameLabel->setText(QApplication::translate("EC2Grid", "EC2 Cluster Name", 0, QApplication::UnicodeUTF8));
        keyFileButton->setText(QApplication::translate("EC2Grid", "Browse", 0, QApplication::UnicodeUTF8));
        os->clear();
        os->insertItems(0, QStringList()
         << QApplication::translate("EC2Grid", "awslinux", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EC2Grid", "linux", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EC2Grid", "win", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EC2Grid", "mac", 0, QApplication::UnicodeUTF8)
        );
        max_parallel_jobsLabel->setText(QApplication::translate("EC2Grid", "Max Parallel Jobs", 0, QApplication::UnicodeUTF8));
        bit->clear();
        bit->insertItems(0, QStringList()
         << QApplication::translate("EC2Grid", "bit64", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EC2Grid", "bit32", 0, QApplication::UnicodeUTF8)
        );
        working_directory->setText(QApplication::translate("EC2Grid", "/home/sgeadmin/", 0, QApplication::UnicodeUTF8));
        enabled->setText(QApplication::translate("EC2Grid", "Enabled", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EC2Grid: public Ui_EC2Grid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EC2GRID_H
