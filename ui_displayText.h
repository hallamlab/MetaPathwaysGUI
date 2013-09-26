/********************************************************************************
** Form generated from reading UI file 'displayText.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYTEXT_H
#define UI_DISPLAYTEXT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ORFMap
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;
    QPushButton *orfMap;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ORFMap)
    {
        if (ORFMap->objectName().isEmpty())
            ORFMap->setObjectName(QString::fromUtf8("ORFMap"));
        ORFMap->resize(724, 498);
        gridLayout = new QGridLayout(ORFMap);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEdit = new QPlainTextEdit(ORFMap);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 2);

        orfMap = new QPushButton(ORFMap);
        orfMap->setObjectName(QString::fromUtf8("orfMap"));

        gridLayout->addWidget(orfMap, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(ORFMap);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 1, 1, 1);


        retranslateUi(ORFMap);
        QObject::connect(buttonBox, SIGNAL(accepted()), ORFMap, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ORFMap, SLOT(reject()));

        QMetaObject::connectSlotsByName(ORFMap);
    } // setupUi

    void retranslateUi(QDialog *ORFMap)
    {
        ORFMap->setWindowTitle(QApplication::translate("ORFMap", "Dialog", 0, QApplication::UnicodeUTF8));
        orfMap->setText(QApplication::translate("ORFMap", "Show ORFs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ORFMap: public Ui_ORFMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYTEXT_H
