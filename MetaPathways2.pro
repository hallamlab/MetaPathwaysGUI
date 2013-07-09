#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T12:33:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MetaPathways2
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        parentwidget.cpp \
        mainwindow.cpp \
        Setup.cpp \
        SettingsTab.cpp \
        RunConfig.cpp \
        ProgressDialog.cpp \
        utilities.cpp \
    rundata.cpp \
    qcustomplot.cpp \
    tabfactory.cpp \
    resultwindow.cpp \
    resultpage.cpp \
    tabledata.cpp


HEADERS  += mainwindow.h \
            parentwidget.h \
            Setup.h \
            SettingsTab.h \
            RunConfig.h \
            ProgressDialog.h \
            utilities.h \
    rundata.h \
    qcustomplot.h \
    tabfactory.h \
    resultwindow.h \
    resultpage.h \
    tabledata.h

FORMS    += mainwindow.ui \
            Setup.ui \
            SettingsTab.ui \
            RunConfig.ui \
            ProgressDialog.ui \
    parentwidget.ui \
    resultwindow.ui \
    resultpage.ui \
    tabledata.ui
