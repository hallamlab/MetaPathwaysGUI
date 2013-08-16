#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T12:33:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
    resultwindow.cpp \
    resultpage.cpp \
    tabledata.cpp \
    welcome.cpp \
    parentsettingsmainwindow.cpp \
    gridchoice.cpp \
    gridsetup.cpp \
    grid.cpp \
    nonec2.cpp \
    ec2grid.cpp \
    metawidget.cpp \
    graphdata.cpp


HEADERS  += mainwindow.h \
            parentwidget.h \
            Setup.h \
            SettingsTab.h \
            RunConfig.h \
            ProgressDialog.h \
            utilities.h \
    rundata.h \
    qcustomplot.h \
    resultwindow.h \
    resultpage.h \
    tabledata.h \
    welcome.h \
    parentsettingsmainwindow.h \
    gridchoice.h \
    gridsetup.h \
    grid.h \
    nonec2.h \
    ec2grid.h \
    metawidget.h \
    graphdata.h

FORMS    += mainwindow.ui \
            Setup.ui \
            SettingsTab.ui \
            RunConfig.ui \
            ProgressDialog.ui \
    parentwidget.ui \
    resultwindow.ui \
    resultpage.ui \
    tabledata.ui \
    welcome.ui \
    parentsettingsmainwindow.ui \
    gridchoice.ui \
    gridsetup.ui \
    nonec2.ui \
    ec2grid.ui \
    graphdata.ui

RESOURCES += \
    Resources.qrc
