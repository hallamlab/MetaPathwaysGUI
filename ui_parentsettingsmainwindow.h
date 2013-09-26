/********************************************************************************
** Form generated from reading UI file 'parentsettingsmainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARENTSETTINGSMAINWINDOW_H
#define UI_PARENTSETTINGSMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParentSettingsMainWindow
{
public:
    QAction *openSetup;
    QAction *openHelp;
    QAction *openGridSetup;
    QAction *openSetupAction;
    QAction *openGridSetupAction;
    QAction *openSetupWindow;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuAbout;
    QMenu *menuSettings;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ParentSettingsMainWindow)
    {
        if (ParentSettingsMainWindow->objectName().isEmpty())
            ParentSettingsMainWindow->setObjectName(QString::fromUtf8("ParentSettingsMainWindow"));
        ParentSettingsMainWindow->resize(569, 891);
        openSetup = new QAction(ParentSettingsMainWindow);
        openSetup->setObjectName(QString::fromUtf8("openSetup"));
        openHelp = new QAction(ParentSettingsMainWindow);
        openHelp->setObjectName(QString::fromUtf8("openHelp"));
        openGridSetup = new QAction(ParentSettingsMainWindow);
        openGridSetup->setObjectName(QString::fromUtf8("openGridSetup"));
        openSetupAction = new QAction(ParentSettingsMainWindow);
        openSetupAction->setObjectName(QString::fromUtf8("openSetupAction"));
        openGridSetupAction = new QAction(ParentSettingsMainWindow);
        openGridSetupAction->setObjectName(QString::fromUtf8("openGridSetupAction"));
        openSetupWindow = new QAction(ParentSettingsMainWindow);
        openSetupWindow->setObjectName(QString::fromUtf8("openSetupWindow"));
        centralwidget = new QWidget(ParentSettingsMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ParentSettingsMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ParentSettingsMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 569, 22));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        ParentSettingsMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ParentSettingsMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ParentSettingsMainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuAbout->addAction(openHelp);
        menuSettings->addAction(openSetupWindow);
        menuSettings->addAction(openGridSetupAction);

        retranslateUi(ParentSettingsMainWindow);

        QMetaObject::connectSlotsByName(ParentSettingsMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ParentSettingsMainWindow)
    {
        ParentSettingsMainWindow->setWindowTitle(QApplication::translate("ParentSettingsMainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        openSetup->setText(QApplication::translate("ParentSettingsMainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        openHelp->setText(QApplication::translate("ParentSettingsMainWindow", "Help", 0, QApplication::UnicodeUTF8));
        openGridSetup->setText(QApplication::translate("ParentSettingsMainWindow", "Grid Setup", 0, QApplication::UnicodeUTF8));
        openSetupAction->setText(QApplication::translate("ParentSettingsMainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        openGridSetupAction->setText(QApplication::translate("ParentSettingsMainWindow", "Grid Settings", 0, QApplication::UnicodeUTF8));
        openSetupWindow->setText(QApplication::translate("ParentSettingsMainWindow", "Open Setup", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("ParentSettingsMainWindow", "About", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("ParentSettingsMainWindow", "Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ParentSettingsMainWindow: public Ui_ParentSettingsMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARENTSETTINGSMAINWINDOW_H
