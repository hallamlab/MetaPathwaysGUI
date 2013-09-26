/********************************************************************************
** Form generated from reading UI file 'welcome.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOME_H
#define UI_WELCOME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Welcome
{
public:
    QLabel *logo;

    void setupUi(QWidget *Welcome)
    {
        if (Welcome->objectName().isEmpty())
            Welcome->setObjectName(QString::fromUtf8("Welcome"));
        Welcome->resize(524, 444);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Welcome->sizePolicy().hasHeightForWidth());
        Welcome->setSizePolicy(sizePolicy);
        Welcome->setStyleSheet(QString::fromUtf8("border:1"));
        logo = new QLabel(Welcome);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(10, 10, 500, 433));
        logo->setFrameShape(QFrame::Box);
        logo->setFrameShadow(QFrame::Sunken);

        retranslateUi(Welcome);

        QMetaObject::connectSlotsByName(Welcome);
    } // setupUi

    void retranslateUi(QWidget *Welcome)
    {
        Welcome->setWindowTitle(QApplication::translate("Welcome", "Form", 0, QApplication::UnicodeUTF8));
        logo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Welcome: public Ui_Welcome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOME_H
