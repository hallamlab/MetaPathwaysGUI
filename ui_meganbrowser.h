/********************************************************************************
** Form generated from reading UI file 'meganbrowser.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEGANBROWSER_H
#define UI_MEGANBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeganBrowser
{
public:
    QGridLayout *gridLayout;
    QPushButton *zoomIn;
    QSpinBox *spinBox;
    QGraphicsView *graphicsView;
    QLabel *label;
    QPushButton *zoomOut;
    QCheckBox *LineType;

    void setupUi(QWidget *MeganBrowser)
    {
        if (MeganBrowser->objectName().isEmpty())
            MeganBrowser->setObjectName(QString::fromUtf8("MeganBrowser"));
        MeganBrowser->resize(1264, 723);
        gridLayout = new QGridLayout(MeganBrowser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        zoomIn = new QPushButton(MeganBrowser);
        zoomIn->setObjectName(QString::fromUtf8("zoomIn"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(zoomIn->sizePolicy().hasHeightForWidth());
        zoomIn->setSizePolicy(sizePolicy);
        zoomIn->setMinimumSize(QSize(90, 32));

        gridLayout->addWidget(zoomIn, 2, 4, 1, 1);

        spinBox = new QSpinBox(MeganBrowser);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
        spinBox->setSizePolicy(sizePolicy1);
        spinBox->setValue(5);

        gridLayout->addWidget(spinBox, 2, 3, 1, 1);

        graphicsView = new QGraphicsView(MeganBrowser);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 6);

        label = new QLabel(MeganBrowser);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label, 2, 2, 1, 1);

        zoomOut = new QPushButton(MeganBrowser);
        zoomOut->setObjectName(QString::fromUtf8("zoomOut"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(zoomOut->sizePolicy().hasHeightForWidth());
        zoomOut->setSizePolicy(sizePolicy3);
        zoomOut->setMinimumSize(QSize(101, 32));

        gridLayout->addWidget(zoomOut, 2, 5, 1, 1);

        LineType = new QCheckBox(MeganBrowser);
        LineType->setObjectName(QString::fromUtf8("LineType"));

        gridLayout->addWidget(LineType, 2, 1, 1, 1);


        retranslateUi(MeganBrowser);

        QMetaObject::connectSlotsByName(MeganBrowser);
    } // setupUi

    void retranslateUi(QWidget *MeganBrowser)
    {
        MeganBrowser->setWindowTitle(QApplication::translate("MeganBrowser", "Form", 0, QApplication::UnicodeUTF8));
        zoomIn->setText(QApplication::translate("MeganBrowser", "Zoom In", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MeganBrowser", "Tree depth :", 0, QApplication::UnicodeUTF8));
        zoomOut->setText(QApplication::translate("MeganBrowser", "Zoom Out", 0, QApplication::UnicodeUTF8));
        LineType->setText(QApplication::translate("MeganBrowser", "Line style", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MeganBrowser: public Ui_MeganBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEGANBROWSER_H
