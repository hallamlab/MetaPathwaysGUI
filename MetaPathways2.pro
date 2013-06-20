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
        mainwindow.cpp \
        Setup.cpp \
        SettingsTab.cpp \
        AdvancedSettingsTab.cpp \
        RunConfig.cpp \
        ProgressDialog.cpp \
        RemoteForm.cpp \
        utilities.cpp \
    parentwidget.cpp \
    resultwidget.cpp \
    rundata.cpp


HEADERS  += mainwindow.h \
            Setup.h \
            SettingsTab.h \
            AdvancedSettingsTab.h \
            RunConfig.h \
            ProgressDialog.h \
            RemoteForm.h \
            utilities.h \
    parentwidget.h \
    resultwidget.h \
    rundata.h

FORMS    += mainwindow.ui \
            Setup.ui \
            SettingsTab.ui \
            RunConfig.ui \
            ProgressDialog.ui \
            RemoteForm.ui \
            AdvancedSettingsTab.ui \
    parentwidget.ui \
    resultwidget.ui
