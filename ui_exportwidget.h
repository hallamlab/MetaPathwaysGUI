/********************************************************************************
** Form generated from reading UI file 'exportwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTWIDGET_H
#define UI_EXPORTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExportWidget
{
public:
    QLabel *label;
    QLabel *label_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QRadioButton *radioButtonTsv;
    QRadioButton *radioButtonCsv;

    void setupUi(QWidget *ExportWidget)
    {
        if (ExportWidget->objectName().isEmpty())
            ExportWidget->setObjectName(QString::fromUtf8("ExportWidget"));
        ExportWidget->resize(569, 311);
        label = new QLabel(ExportWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 251, 21));
        QFont font;
        font.setPointSize(18);
        label->setFont(font);
        label_2 = new QLabel(ExportWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 62, 16));
        gridLayoutWidget = new QWidget(ExportWidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 70, 491, 181));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        radioButtonTsv = new QRadioButton(ExportWidget);
        radioButtonTsv->setObjectName(QString::fromUtf8("radioButtonTsv"));
        radioButtonTsv->setGeometry(QRect(30, 270, 102, 20));
        radioButtonCsv = new QRadioButton(ExportWidget);
        radioButtonCsv->setObjectName(QString::fromUtf8("radioButtonCsv"));
        radioButtonCsv->setGeometry(QRect(100, 270, 102, 20));

        retranslateUi(ExportWidget);

        QMetaObject::connectSlotsByName(ExportWidget);
    } // setupUi

    void retranslateUi(QWidget *ExportWidget)
    {
        ExportWidget->setWindowTitle(QApplication::translate("ExportWidget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ExportWidget", "Export Data", 0, QApplication::UnicodeUTF8));
        label_2->setText(QString());
        radioButtonTsv->setText(QApplication::translate("ExportWidget", "tsv", 0, QApplication::UnicodeUTF8));
        radioButtonCsv->setText(QApplication::translate("ExportWidget", "csv", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExportWidget: public Ui_ExportWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTWIDGET_H
