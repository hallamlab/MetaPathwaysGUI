/********************************************************************************
** Form generated from reading UI file 'resultwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTWINDOW_H
#define UI_RESULTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ResultWindow
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *sampleSelect;
    QTabWidget *resultTabs;
    QWidget *tab;
    QWidget *tab_2;

    void setupUi(QWidget *ResultWindow)
    {
        if (ResultWindow->objectName().isEmpty())
            ResultWindow->setObjectName(QString::fromUtf8("ResultWindow"));
        ResultWindow->resize(1141, 1001);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ResultWindow->sizePolicy().hasHeightForWidth());
        ResultWindow->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ResultWindow);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ResultWindow);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        sampleSelect = new QComboBox(ResultWindow);
        sampleSelect->setObjectName(QString::fromUtf8("sampleSelect"));

        gridLayout->addWidget(sampleSelect, 0, 1, 1, 1);

        resultTabs = new QTabWidget(ResultWindow);
        resultTabs->setObjectName(QString::fromUtf8("resultTabs"));
        resultTabs->setUsesScrollButtons(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setEnabled(true);
        resultTabs->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        resultTabs->addTab(tab_2, QString());

        gridLayout->addWidget(resultTabs, 1, 0, 1, 2);


        retranslateUi(ResultWindow);

        resultTabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ResultWindow);
    } // setupUi

    void retranslateUi(QWidget *ResultWindow)
    {
        ResultWindow->setWindowTitle(QApplication::translate("ResultWindow", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ResultWindow", "Current Sample", 0, QApplication::UnicodeUTF8));
        resultTabs->setTabText(resultTabs->indexOf(tab), QApplication::translate("ResultWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        resultTabs->setTabText(resultTabs->indexOf(tab_2), QApplication::translate("ResultWindow", "Tab 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ResultWindow: public Ui_ResultWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTWINDOW_H
