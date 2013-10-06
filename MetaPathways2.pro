#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T12:33:22
#
#-------------------------------------------------

QT       += core gui
QT += declarative



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MetaPathways2
TEMPLATE = app

#CONFIG += console



SOURCES += main.cpp\
        parentwidget.cpp \
        Setup.cpp \
        SettingsTab.cpp \
        RunConfig.cpp \
        ProgressDialog.cpp \
        utilities.cpp \
    rundata.cpp \
    qcustomplot.cpp \
    resultwindow.cpp \
    dataviews/tabledata.cpp \
    welcome.cpp \
    grid/gridchoice.cpp \
    grid/gridsetup.cpp \
    grid/grid.cpp \
    grid/nonec2.cpp \
    grid/ec2grid.cpp \
    metawidget.cpp \
    dataviews/graphdata.cpp \
    dataviews/largetable.cpp \
    popupviews/searchwidget.cpp \
    popupviews/exportwidget.cpp \
    popupviews/exportbox.cpp \
    popupviews/popuplistener.cpp \
    popupviews/displayinfo.cpp \
    popupviews/selectsamples.cpp \
    caching/fileindexmanager.cpp \
    caching/fileindex.cpp \
    caching/tablemanager.cpp \
    genebrowser/graphicsrepresentation.cpp \
    genebrowser/graphicsitemscollection.cpp \
    genebrowser/genebrowser.cpp \
    genebrowser/genomeview.cpp \
    dataviews/megandata.cpp \
    dataviews/meganview.cpp \
    datamodel/datamanager.cpp \
    datamodel/htree.cpp \
    datamodel/connector.cpp \
    datamodel/htabledata.cpp \
    mainframe.cpp \
    displaywidgets/toolbarwidgets.cpp



HEADERS  += \
            parentwidget.h \
            Setup.h \
            SettingsTab.h \
            RunConfig.h \
            ProgressDialog.h \
            utilities.h \
    rundata.h \
    qcustomplot.h \
    resultwindow.h \
    dataviews/tabledata.h \
    welcome.h \
    grid/gridchoice.h \
    grid/gridsetup.h \
    grid/grid.h \
    grid/nonec2.h \
    grid/ec2grid.h \
    metawidget.h \
    dataviews/graphdata.h \
    dataviews/largetable.h \
    popupviews/searchwidget.h \
    popupviews/exportwidget.h \
    popupviews/exportbox.h \
    popupviews/popuplistener.h \
    popupviews/displayinfo.h \
    popupviews/selectsamples.h \
    caching/fileindexmanager.h \
    caching/fileindex.h \
    types.h \
    caching/tablemanager.h \
    genebrowser/graphicsrepresentation.h \
    genebrowser/graphicsitemscollection.h \
    genebrowser/genebrowser.h \
    genebrowser/genomeview.h \
    dataviews/megandata.h \
    dataviews/meganview.h \
    datamodel/datamanager.h \
    datamodel/htree.h \
    datamodel/connector.h \
    datamodel/datamodeltypes.h\
    datamodel/htabledata.h \
    mainframe.h \
    displaywidgets/toolbarwidgets.h




FORMS    += \
            Setup.ui \
            SettingsTab.ui \
            RunConfig.ui \
            ProgressDialog.ui \
    parentwidget.ui \
    resultwindow.ui \
    dataviews/tabledata.ui \
    welcome.ui \
    grid/gridchoice.ui \
    grid/gridsetup.ui \
    grid/nonec2.ui \
    grid/ec2grid.ui \
    dataviews/graphdata.ui \
    popupviews/searchwidget.ui \
    popupviews/selectsamples.ui \
    popupviews/exportwidget.ui \
    displayText.ui \
    popupviews/displayinfo.ui \
    graphicsrepresentation.ui \
    dataviews/meganbrowser.ui \
    datamodel/htabledata.ui \
    mainframe.ui


RESOURCES += \
    Resources.qrc



