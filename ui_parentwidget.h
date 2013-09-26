/********************************************************************************
** Form generated from reading UI file 'parentwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARENTWIDGET_H
#define UI_PARENTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParentWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *backButton;
    QPushButton *cancelButton;
    QPushButton *continueButton;
    QTabWidget *parentTabWidget;
    QWidget *runParameterTab;
    QWidget *runStagesTab;

    void setupUi(QWidget *ParentWidget)
    {
        if (ParentWidget->objectName().isEmpty())
            ParentWidget->setObjectName(QString::fromUtf8("ParentWidget"));
        ParentWidget->resize(569, 891);
        gridLayout = new QGridLayout(ParentWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        backButton = new QPushButton(ParentWidget);
        backButton->setObjectName(QString::fromUtf8("backButton"));

        gridLayout->addWidget(backButton, 1, 0, 1, 1);

        cancelButton = new QPushButton(ParentWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 1, 1, 1);

        continueButton = new QPushButton(ParentWidget);
        continueButton->setObjectName(QString::fromUtf8("continueButton"));

        gridLayout->addWidget(continueButton, 1, 2, 1, 1);

        parentTabWidget = new QTabWidget(ParentWidget);
        parentTabWidget->setObjectName(QString::fromUtf8("parentTabWidget"));
        runParameterTab = new QWidget();
        runParameterTab->setObjectName(QString::fromUtf8("runParameterTab"));
        parentTabWidget->addTab(runParameterTab, QString());
        runStagesTab = new QWidget();
        runStagesTab->setObjectName(QString::fromUtf8("runStagesTab"));
        parentTabWidget->addTab(runStagesTab, QString());

        gridLayout->addWidget(parentTabWidget, 0, 0, 1, 3);


        retranslateUi(ParentWidget);

        parentTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ParentWidget);
    } // setupUi

    void retranslateUi(QWidget *ParentWidget)
    {
        ParentWidget->setWindowTitle(QApplication::translate("ParentWidget", "Form", 0, QApplication::UnicodeUTF8));
        backButton->setText(QApplication::translate("ParentWidget", "Back", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ParentWidget", "Cancel", 0, QApplication::UnicodeUTF8));
        continueButton->setText(QApplication::translate("ParentWidget", "Continue", 0, QApplication::UnicodeUTF8));
        parentTabWidget->setTabText(parentTabWidget->indexOf(runParameterTab), QApplication::translate("ParentWidget", "Run Parameters", 0, QApplication::UnicodeUTF8));
        parentTabWidget->setTabText(parentTabWidget->indexOf(runStagesTab), QApplication::translate("ParentWidget", "Run Stages", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ParentWidget: public Ui_ParentWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARENTWIDGET_H
