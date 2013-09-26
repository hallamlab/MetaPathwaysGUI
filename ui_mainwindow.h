/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionRun_Settings;
    QAction *actionSetup;
    QWidget *centralwidget;
    QGraphicsView *logo;
    QLabel *titleLabel;
    QLabel *storyLabel;
    QPushButton *startButton;
    QPushButton *setupButton;
    QFrame *line;
    QLabel *warningLabel;
    QPushButton *remoteButton;
    QMenuBar *menubar;
    QMenu *menuConfiguration;
    QMenu *menuAbout;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(786, 258);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionRun_Settings = new QAction(MainWindow);
        actionRun_Settings->setObjectName(QString::fromUtf8("actionRun_Settings"));
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName(QString::fromUtf8("actionSetup"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        logo = new QGraphicsView(centralwidget);
        logo->setObjectName(QString::fromUtf8("logo"));
        logo->setGeometry(QRect(10, 10, 200, 200));
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setGeometry(QRect(230, 10, 531, 31));
        QFont font;
        font.setPointSize(24);
        titleLabel->setFont(font);
        storyLabel = new QLabel(centralwidget);
        storyLabel->setObjectName(QString::fromUtf8("storyLabel"));
        storyLabel->setGeometry(QRect(230, 40, 541, 101));
        storyLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        storyLabel->setWordWrap(true);
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(660, 180, 114, 32));
        setupButton = new QPushButton(centralwidget);
        setupButton->setObjectName(QString::fromUtf8("setupButton"));
        setupButton->setGeometry(QRect(540, 180, 114, 32));
        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(230, 160, 541, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        warningLabel = new QLabel(centralwidget);
        warningLabel->setObjectName(QString::fromUtf8("warningLabel"));
        warningLabel->setGeometry(QRect(230, 135, 541, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        warningLabel->setFont(font1);
        warningLabel->setWordWrap(true);
        remoteButton = new QPushButton(centralwidget);
        remoteButton->setObjectName(QString::fromUtf8("remoteButton"));
        remoteButton->setGeometry(QRect(220, 180, 114, 32));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 786, 25));
        menuConfiguration = new QMenu(menubar);
        menuConfiguration->setObjectName(QString::fromUtf8("menuConfiguration"));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuConfiguration->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuConfiguration->addAction(actionRun_Settings);
        menuConfiguration->addAction(actionSetup);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionRun_Settings->setText(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
        actionSetup->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        titleLabel->setText(QApplication::translate("MainWindow", "MetaPathways", 0, QApplication::UnicodeUTF8));
        storyLabel->setText(QApplication::translate("MainWindow", "MetaPathways is a modular software pipeline for integrated analysis of environmental metagenomic samples. The software performs a series of popular analyses for taxonomic profiling and functional potential with limited data handling, allowing researchers to spend their time analyzing their data instead of performing complicated data transformations.", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        setupButton->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        warningLabel->setText(QApplication::translate("MainWindow", "MetaPathways needs your input to work. You must run Setup before proceeding.", 0, QApplication::UnicodeUTF8));
        remoteButton->setText(QApplication::translate("MainWindow", "Remote Jobs", 0, QApplication::UnicodeUTF8));
        menuConfiguration->setTitle(QApplication::translate("MainWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
