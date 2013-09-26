/********************************************************************************
** Form generated from reading UI file 'displayinfo.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DISPLAYINFO_H
#define UI_DISPLAYINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DisplayInfo
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textOut;
    QPushButton *orfMap;
    QDialogButtonBox *okAndCancel;

    void setupUi(QDialog *DisplayInfo)
    {
        if (DisplayInfo->objectName().isEmpty())
            DisplayInfo->setObjectName(QString::fromUtf8("DisplayInfo"));
        DisplayInfo->resize(681, 489);
        gridLayout = new QGridLayout(DisplayInfo);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textOut = new QTextEdit(DisplayInfo);
        textOut->setObjectName(QString::fromUtf8("textOut"));

        gridLayout->addWidget(textOut, 0, 0, 1, 2);

        orfMap = new QPushButton(DisplayInfo);
        orfMap->setObjectName(QString::fromUtf8("orfMap"));

        gridLayout->addWidget(orfMap, 1, 0, 1, 1);

        okAndCancel = new QDialogButtonBox(DisplayInfo);
        okAndCancel->setObjectName(QString::fromUtf8("okAndCancel"));
        okAndCancel->setOrientation(Qt::Horizontal);
        okAndCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(okAndCancel, 1, 1, 1, 1);


        retranslateUi(DisplayInfo);
        QObject::connect(okAndCancel, SIGNAL(accepted()), DisplayInfo, SLOT(accept()));
        QObject::connect(okAndCancel, SIGNAL(rejected()), DisplayInfo, SLOT(reject()));

        QMetaObject::connectSlotsByName(DisplayInfo);
    } // setupUi

    void retranslateUi(QDialog *DisplayInfo)
    {
        DisplayInfo->setWindowTitle(QApplication::translate("DisplayInfo", "Dialog", 0, QApplication::UnicodeUTF8));
        orfMap->setText(QApplication::translate("DisplayInfo", "Show ORFs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DisplayInfo: public Ui_DisplayInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAYINFO_H
