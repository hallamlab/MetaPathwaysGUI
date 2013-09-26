/********************************************************************************
** Form generated from reading UI file 'gridsetup.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDSETUP_H
#define UI_GRIDSETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GridSetup
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *startMessage;
    QWidget *page_2;
    QPushButton *saveButton;
    QLabel *label_2;
    QToolButton *addGrid;
    QToolButton *deleteGrid;
    QFrame *line;
    QLabel *label;
    QComboBox *gridSelection;
    QPushButton *cancelButton;

    void setupUi(QWidget *GridSetup)
    {
        if (GridSetup->objectName().isEmpty())
            GridSetup->setObjectName(QString::fromUtf8("GridSetup"));
        GridSetup->resize(690, 505);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GridSetup->sizePolicy().hasHeightForWidth());
        GridSetup->setSizePolicy(sizePolicy);
        GridSetup->setMaximumSize(QSize(690, 727));
        gridLayout = new QGridLayout(GridSetup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(GridSetup);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        startMessage = new QLabel(page);
        startMessage->setObjectName(QString::fromUtf8("startMessage"));
        startMessage->setGeometry(QRect(80, 160, 511, 20));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 2, 0, 1, 8);

        saveButton = new QPushButton(GridSetup);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        gridLayout->addWidget(saveButton, 3, 5, 1, 3);

        label_2 = new QLabel(GridSetup);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        addGrid = new QToolButton(GridSetup);
        addGrid->setObjectName(QString::fromUtf8("addGrid"));

        gridLayout->addWidget(addGrid, 0, 6, 1, 1);

        deleteGrid = new QToolButton(GridSetup);
        deleteGrid->setObjectName(QString::fromUtf8("deleteGrid"));

        gridLayout->addWidget(deleteGrid, 0, 7, 1, 1);

        line = new QFrame(GridSetup);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 8);

        label = new QLabel(GridSetup);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(24);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        gridSelection = new QComboBox(GridSetup);
        gridSelection->setObjectName(QString::fromUtf8("gridSelection"));

        gridLayout->addWidget(gridSelection, 0, 4, 1, 2);

        cancelButton = new QPushButton(GridSetup);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 3, 4, 1, 1);

        QWidget::setTabOrder(gridSelection, addGrid);
        QWidget::setTabOrder(addGrid, deleteGrid);
        QWidget::setTabOrder(deleteGrid, cancelButton);
        QWidget::setTabOrder(cancelButton, saveButton);

        retranslateUi(GridSetup);

        QMetaObject::connectSlotsByName(GridSetup);
    } // setupUi

    void retranslateUi(QWidget *GridSetup)
    {
        GridSetup->setWindowTitle(QApplication::translate("GridSetup", "Form", 0, QApplication::UnicodeUTF8));
        startMessage->setText(QApplication::translate("GridSetup", "No existing grid configurations found in parameter file, please add a new one!", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("GridSetup", "Save and Continue", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GridSetup", "Selected Grid", 0, QApplication::UnicodeUTF8));
        addGrid->setText(QApplication::translate("GridSetup", "+", 0, QApplication::UnicodeUTF8));
        deleteGrid->setText(QApplication::translate("GridSetup", "-", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GridSetup", "Grid Setup", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("GridSetup", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GridSetup: public Ui_GridSetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDSETUP_H
