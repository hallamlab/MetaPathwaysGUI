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
        tabs/Setup.cpp \
        tabs/SettingsTab.cpp \
        tabs/RunConfig.cpp \
        tabs/ProgressDialog.cpp \
    helper/utilities.cpp \
    helper/rundata.cpp \
    helper/qcustomplot.cpp \
    tabs/resultwindow.cpp \
    dataviews/tabledata.cpp \
    tabs/welcome.cpp \
    grid/gridchoice.cpp \
    grid/gridsetup.cpp \
    grid/grid.cpp \
    grid/nonec2.cpp \
    grid/ec2grid.cpp \
    grid/gridprogress.cpp\
    dataviews/graphdata.cpp \
    dataviews/largetable.cpp \
    popupviews/searchwidget.cpp \
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
    displaywidgets/toolbarwidgets.cpp \
    displaywidgets/toolbarmanager.cpp \
    displaywidgets/mdiareawidget.cpp \
    displaywidgets/widgetstacker.cpp \
    displaywidgets/mqtoolbar.cpp \
    popupviews/exportsource.cpp \
    datamodel/samplercmanager.cpp \
    displaywidgets/progressview.cpp \
    popupviews/meganexportbox.cpp \
    datamodel/rundatastats.cpp \
    helper/progressdisplaydata.cpp \
    displaywidgets/createwidgets.cpp \
    caching/memorymanager.cpp \
    dataviews/statusview.cpp \
    datamodel/treeitem.cpp \
    datamodel/treemodel.cpp \
    helper/types.cpp \
    tabs/simpletabgroups.cpp



HEADERS  += \
            tabs/Setup.h \
            tabs/SettingsTab.h \
            tabs/RunConfig.h \
            tabs/ProgressDialog.h \
    helper/utilities.h \
    helper/rundata.h \
    helper/qcustomplot.h \
    tabs/resultwindow.h \
    dataviews/tabledata.h \
    tabs/welcome.h \
    grid/gridchoice.h \
    grid/gridsetup.h \
    grid/grid.h \
    grid/nonec2.h \
    grid/ec2grid.h \
    grid/gridprogress.h\
    dataviews/graphdata.h \
    dataviews/largetable.h \
    popupviews/searchwidget.h \
    popupviews/exportbox.h \
    popupviews/popuplistener.h \
    popupviews/displayinfo.h \
    popupviews/selectsamples.h \
    caching/fileindexmanager.h \
    caching/fileindex.h \
    helper/types.h \
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
    displaywidgets/toolbarwidgets.h \
    displaywidgets/toolbarmanager.h \
    displaywidgets/mdiareawidget.h \
    displaywidgets/widgetstacker.h \
    displaywidgets/mqtoolbar.h \
    popupviews/exportsource.h \
    datamodel/samplercmanager.h \
    displaywidgets/progressview.h \
    popupviews/meganexportbox.h \
    datamodel/rundatastats.h \
    helper/progressdisplaydata.h \
    displaywidgets/createwidgets.h \
    caching/memorymanager.h \
    dataviews/statusview.h\
    datamodel/treeitem.h\
    datamodel/treemodel.h \
    helper/constants.h \
    tabs/simpletabgroups.h




FORMS    += \
            tabs/Setup.ui \
            tabs/SettingsTab.ui \
            tabs/RunConfig.ui \
            tabs/ProgressDialog.ui \
    tabs/resultwindow.ui \
    dataviews/tabledata.ui \
    tabs/welcome.ui \
    grid/gridchoice.ui \
    grid/gridsetup.ui \
    grid/nonec2.ui \
    grid/ec2grid.ui \
    dataviews/graphdata.ui \
    popupviews/searchwidget.ui \
    popupviews/selectsamples.ui \
    popupviews/exportwidget.ui \
    popupviews/displayinfo.ui \
    graphicsrepresentation.ui \
    dataviews/meganbrowser.ui \
    datamodel/htabledata.ui \
    mainframe.ui


RESOURCES += \
    Resources.qrc

RC_FILE += MetaPathways2.rc
ICON += MetaPathways2.icns


