#############################################################################
# Makefile for building: MetaPathways2.app/Contents/MacOS/MetaPathways2
# Generated by qmake (2.01a) (Qt 4.8.3) on: Wed Apr 9 17:22:29 2014
# Project:  MetaPathways2.pro
# Template: app
# Command: /usr/bin/qmake -o MetaPathways2.xcodeproj/project.pbxproj MetaPathways2.pro
#############################################################################

MOC       = /Developer/Tools/Qt/moc
UIC       = /Developer/Tools/Qt/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_DECLARATIVE_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/usr/local/Qt4.8/mkspecs/macx-xcode -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/Library/Frameworks/QtDeclarative.framework/Versions/4/Headers -I/usr/include/QtDeclarative -I/usr/include -I. -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/Library/Frameworks
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./moc_Setup.cpp ./moc_SettingsTab.cpp ./moc_RunConfig.cpp\
	 ./moc_ProgressDialog.cpp ./moc_qcustomplot.cpp ./moc_resultwindow.cpp\
	 ./moc_tabledata.cpp ./moc_welcome.cpp ./moc_gridchoice.cpp\
	 ./moc_gridsetup.cpp ./moc_nonec2.cpp ./moc_ec2grid.cpp\
	 ./moc_gridprogress.cpp ./moc_graphdata.cpp ./moc_searchwidget.cpp\
	 ./moc_exportbox.cpp ./moc_popuplistener.cpp ./moc_displayinfo.cpp\
	 ./moc_selectsamples.cpp ./moc_graphicsrepresentation.cpp ./moc_genomeview.cpp\
	 ./moc_meganview.cpp ./moc_htabledata.cpp ./moc_mainframe.cpp\
	 ./moc_toolbarwidgets.cpp ./moc_toolbarmanager.cpp ./moc_mqtoolbar.cpp\
	 ./moc_progressview.cpp ./moc_meganexportbox.cpp ./moc_rundatastats.cpp ./qrc_Resources.cpp ./ui_Setup.h ./ui_SettingsTab.h ./ui_RunConfig.h\
	 ./ui_ProgressDialog.h ./ui_resultwindow.h ./ui_tabledata.h\
	 ./ui_welcome.h ./ui_gridchoice.h ./ui_gridsetup.h\
	 ./ui_nonec2.h ./ui_ec2grid.h ./ui_graphdata.h\
	 ./ui_searchwidget.h ./ui_selectsamples.h ./ui_exportwidget.h\
	 ./ui_displayText.h ./ui_displayinfo.h ./ui_graphicsrepresentation.h\
	 ./ui_meganbrowser.h ./ui_htabledata.h ./ui_mainframe.h
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_Setup.cpp moc_SettingsTab.cpp moc_RunConfig.cpp moc_ProgressDialog.cpp moc_qcustomplot.cpp moc_resultwindow.cpp moc_tabledata.cpp moc_welcome.cpp moc_gridchoice.cpp moc_gridsetup.cpp moc_nonec2.cpp moc_ec2grid.cpp moc_gridprogress.cpp moc_graphdata.cpp moc_searchwidget.cpp moc_exportbox.cpp moc_popuplistener.cpp moc_displayinfo.cpp moc_selectsamples.cpp moc_graphicsrepresentation.cpp moc_genomeview.cpp moc_meganview.cpp moc_htabledata.cpp moc_mainframe.cpp moc_toolbarwidgets.cpp moc_toolbarmanager.cpp moc_mqtoolbar.cpp moc_progressview.cpp moc_meganexportbox.cpp moc_rundatastats.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_Setup.cpp moc_SettingsTab.cpp moc_RunConfig.cpp moc_ProgressDialog.cpp moc_qcustomplot.cpp moc_resultwindow.cpp moc_tabledata.cpp moc_welcome.cpp moc_gridchoice.cpp moc_gridsetup.cpp moc_nonec2.cpp moc_ec2grid.cpp moc_gridprogress.cpp moc_graphdata.cpp moc_searchwidget.cpp moc_exportbox.cpp moc_popuplistener.cpp moc_displayinfo.cpp moc_selectsamples.cpp moc_graphicsrepresentation.cpp moc_genomeview.cpp moc_meganview.cpp moc_htabledata.cpp moc_mainframe.cpp moc_toolbarwidgets.cpp moc_toolbarmanager.cpp moc_mqtoolbar.cpp moc_progressview.cpp moc_meganexportbox.cpp moc_rundatastats.cpp
moc_Setup.cpp: mainframe.h \
		Setup.h \
		ui_Setup.h \
		rundata.h \
		types.h \
		utilities.h \
		resultwindow.h \
		ProgressDialog.h \
		resultpage.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		ui_resultwindow.h \
		dataviews/graphdata.h \
		qcustomplot.h \
		caching/tablemanager.h \
		dataviews/largetable.h \
		dataviews/tabledata.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		dataviews/meganview.h \
		dataviews/megandata.h \
		genebrowser/graphicsitemscollection.h \
		genebrowser/genebrowser.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		datamodel/rundatastats.h \
		popupviews/selectsamples.h \
		RunConfig.h \
		grid/gridsetup.h \
		grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		SettingsTab.h \
		welcome.h \
		grid/gridprogress.h \
		displaywidgets/mqtoolbar.h \
		Setup.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ Setup.h -o moc_Setup.cpp

moc_SettingsTab.cpp: RunConfig.h \
		grid/gridsetup.h \
		grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		rundata.h \
		types.h \
		utilities.h \
		SettingsTab.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ SettingsTab.h -o moc_SettingsTab.cpp

moc_RunConfig.cpp: grid/gridsetup.h \
		grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		rundata.h \
		types.h \
		utilities.h \
		RunConfig.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ RunConfig.h -o moc_RunConfig.cpp

moc_ProgressDialog.cpp: rundata.h \
		types.h \
		utilities.h \
		resultpage.h \
		ProgressDialog.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ProgressDialog.h -o moc_ProgressDialog.cpp

moc_qcustomplot.cpp: qcustomplot.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ qcustomplot.h -o moc_qcustomplot.cpp

moc_resultwindow.cpp: rundata.h \
		types.h \
		utilities.h \
		ProgressDialog.h \
		resultpage.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		ui_resultwindow.h \
		dataviews/graphdata.h \
		qcustomplot.h \
		caching/tablemanager.h \
		dataviews/largetable.h \
		dataviews/tabledata.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		dataviews/meganview.h \
		dataviews/megandata.h \
		genebrowser/graphicsitemscollection.h \
		genebrowser/genebrowser.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		datamodel/rundatastats.h \
		popupviews/selectsamples.h \
		resultwindow.h \
		resultwindow.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ resultwindow.h -o moc_resultwindow.cpp

moc_tabledata.cpp: dataviews/largetable.h \
		types.h \
		popupviews/searchwidget.h \
		dataviews/tabledata.h \
		popupviews/exportbox.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		rundata.h \
		utilities.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportsource.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		caching/tablemanager.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		dataviews/tabledata.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ dataviews/tabledata.h -o moc_tabledata.cpp

moc_welcome.cpp: welcome.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ welcome.h -o moc_welcome.cpp

moc_gridchoice.cpp: grid/gridsetup.h \
		grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		rundata.h \
		types.h \
		utilities.h \
		grid/gridchoice.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ grid/gridchoice.h -o moc_gridchoice.cpp

moc_gridsetup.cpp: grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		grid/gridsetup.h \
		rundata.h \
		types.h \
		utilities.h \
		grid/gridsetup.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ grid/gridsetup.h -o moc_gridsetup.cpp

moc_nonec2.cpp: grid/grid.h \
		grid/ec2grid.h \
		grid/nonec2.h \
		grid/nonec2.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ grid/nonec2.h -o moc_nonec2.cpp

moc_ec2grid.cpp: grid/grid.h \
		grid/ec2grid.h \
		grid/nonec2.h \
		grid/ec2grid.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ grid/ec2grid.h -o moc_ec2grid.cpp

moc_gridprogress.cpp: datamodel/samplercmanager.h \
		types.h \
		caching/fileindex.h \
		caching/fileindexmanager.h \
		displaywidgets/progressview.h \
		rundata.h \
		utilities.h \
		grid/gridprogress.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ grid/gridprogress.h -o moc_gridprogress.cpp

moc_graphdata.cpp: qcustomplot.h \
		caching/tablemanager.h \
		dataviews/largetable.h \
		types.h \
		dataviews/graphdata.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ dataviews/graphdata.h -o moc_graphdata.cpp

moc_searchwidget.cpp: dataviews/tabledata.h \
		dataviews/largetable.h \
		types.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		rundata.h \
		utilities.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		caching/tablemanager.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		popupviews/searchwidget.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/searchwidget.h -o moc_searchwidget.cpp

moc_exportbox.cpp: dataviews/tabledata.h \
		dataviews/largetable.h \
		types.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		rundata.h \
		utilities.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		caching/tablemanager.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		popupviews/exportbox.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/exportbox.h -o moc_exportbox.cpp

moc_popuplistener.cpp: popupviews/popuplistener.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/popuplistener.h -o moc_popuplistener.cpp

moc_displayinfo.cpp: types.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		utilities.h \
		popupviews/displayinfo.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/displayinfo.h -o moc_displayinfo.cpp

moc_selectsamples.cpp: resultwindow.h \
		rundata.h \
		types.h \
		utilities.h \
		ProgressDialog.h \
		resultpage.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		ui_resultwindow.h \
		dataviews/graphdata.h \
		qcustomplot.h \
		caching/tablemanager.h \
		dataviews/largetable.h \
		dataviews/tabledata.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		dataviews/meganview.h \
		dataviews/megandata.h \
		genebrowser/graphicsitemscollection.h \
		genebrowser/genebrowser.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		datamodel/rundatastats.h \
		popupviews/selectsamples.h \
		popupviews/selectsamples.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/selectsamples.h -o moc_selectsamples.cpp

moc_graphicsrepresentation.cpp: genebrowser/graphicsitemscollection.h \
		types.h \
		genebrowser/genebrowser.h \
		dataviews/megandata.h \
		utilities.h \
		genebrowser/genomeview.h \
		dataviews/meganview.h \
		datamodel/samplercmanager.h \
		caching/fileindex.h \
		caching/fileindexmanager.h \
		rundata.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		genebrowser/graphicsrepresentation.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ genebrowser/graphicsrepresentation.h -o moc_graphicsrepresentation.cpp

moc_genomeview.cpp: types.h \
		genebrowser/genomeview.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ genebrowser/genomeview.h -o moc_genomeview.cpp

moc_meganview.cpp: dataviews/megandata.h \
		genebrowser/graphicsitemscollection.h \
		types.h \
		genebrowser/genebrowser.h \
		utilities.h \
		datamodel/samplercmanager.h \
		caching/fileindex.h \
		caching/fileindexmanager.h \
		rundata.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		dataviews/meganview.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ dataviews/meganview.h -o moc_meganview.cpp

moc_htabledata.cpp: types.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		rundata.h \
		utilities.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		datamodel/htabledata.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		dataviews/tabledata.h \
		dataviews/largetable.h \
		popupviews/searchwidget.h \
		popupviews/exportsource.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		caching/tablemanager.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		datamodel/htabledata.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ datamodel/htabledata.h -o moc_htabledata.cpp

moc_mainframe.cpp: Setup.h \
		mainframe.h \
		resultwindow.h \
		rundata.h \
		types.h \
		utilities.h \
		ProgressDialog.h \
		resultpage.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		ui_resultwindow.h \
		dataviews/graphdata.h \
		qcustomplot.h \
		caching/tablemanager.h \
		dataviews/largetable.h \
		dataviews/tabledata.h \
		popupviews/searchwidget.h \
		datamodel/htabledata.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/toolbarwidgets.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		popupviews/exportsource.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		dataviews/meganview.h \
		dataviews/megandata.h \
		genebrowser/graphicsitemscollection.h \
		genebrowser/genebrowser.h \
		popupviews/meganexportbox.h \
		displaywidgets/progressview.h \
		datamodel/rundatastats.h \
		popupviews/selectsamples.h \
		RunConfig.h \
		grid/gridsetup.h \
		grid/ec2grid.h \
		grid/grid.h \
		grid/nonec2.h \
		grid/gridchoice.h \
		SettingsTab.h \
		welcome.h \
		grid/gridprogress.h \
		displaywidgets/mqtoolbar.h \
		ui_Setup.h \
		mainframe.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ mainframe.h -o moc_mainframe.cpp

moc_toolbarwidgets.cpp: displaywidgets/toolbarwidgets.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ displaywidgets/toolbarwidgets.h -o moc_toolbarwidgets.cpp

moc_toolbarmanager.cpp: displaywidgets/toolbarwidgets.h \
		datamodel/htabledata.h \
		types.h \
		datamodel/connector.h \
		datamodel/datamodeltypes.h \
		datamodel/htree.h \
		datamodel/datamanager.h \
		rundata.h \
		utilities.h \
		displaywidgets/toolbarmanager.h \
		displaywidgets/mdiareawidget.h \
		displaywidgets/widgetstacker.h \
		popupviews/exportbox.h \
		dataviews/tabledata.h \
		dataviews/largetable.h \
		popupviews/searchwidget.h \
		popupviews/exportsource.h \
		popupviews/displayinfo.h \
		caching/fileindex.h \
		genebrowser/genomeview.h \
		caching/tablemanager.h \
		datamodel/samplercmanager.h \
		caching/fileindexmanager.h \
		displaywidgets/toolbarmanager.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ displaywidgets/toolbarmanager.h -o moc_toolbarmanager.cpp

moc_mqtoolbar.cpp: displaywidgets/mqtoolbar.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ displaywidgets/mqtoolbar.h -o moc_mqtoolbar.cpp

moc_progressview.cpp: displaywidgets/progressview.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ displaywidgets/progressview.h -o moc_progressview.cpp

moc_meganexportbox.cpp: datamodel/samplercmanager.h \
		types.h \
		caching/fileindex.h \
		caching/fileindexmanager.h \
		displaywidgets/progressview.h \
		rundata.h \
		utilities.h \
		popupviews/meganexportbox.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ popupviews/meganexportbox.h -o moc_meganexportbox.cpp

moc_rundatastats.cpp: rundata.h \
		types.h \
		utilities.h \
		datamodel/samplercmanager.h \
		caching/fileindex.h \
		caching/fileindexmanager.h \
		datamodel/rundatastats.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ datamodel/rundatastats.h -o moc_rundatastats.cpp

compiler_rcc_make_all: qrc_Resources.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_Resources.cpp
qrc_Resources.cpp: Resources.qrc
	/Developer/Tools/Qt/rcc -name Resources Resources.qrc -o qrc_Resources.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all: ui_Setup.h ui_SettingsTab.h ui_RunConfig.h ui_ProgressDialog.h ui_resultwindow.h ui_tabledata.h ui_welcome.h ui_gridchoice.h ui_gridsetup.h ui_nonec2.h ui_ec2grid.h ui_graphdata.h ui_searchwidget.h ui_selectsamples.h ui_exportwidget.h ui_displayText.h ui_displayinfo.h ui_graphicsrepresentation.h ui_meganbrowser.h ui_htabledata.h ui_mainframe.h
compiler_uic_clean:
	-$(DEL_FILE) ui_Setup.h ui_SettingsTab.h ui_RunConfig.h ui_ProgressDialog.h ui_resultwindow.h ui_tabledata.h ui_welcome.h ui_gridchoice.h ui_gridsetup.h ui_nonec2.h ui_ec2grid.h ui_graphdata.h ui_searchwidget.h ui_selectsamples.h ui_exportwidget.h ui_displayText.h ui_displayinfo.h ui_graphicsrepresentation.h ui_meganbrowser.h ui_htabledata.h ui_mainframe.h
ui_Setup.h: Setup.ui
	/Developer/Tools/Qt/uic Setup.ui -o ui_Setup.h

ui_SettingsTab.h: SettingsTab.ui
	/Developer/Tools/Qt/uic SettingsTab.ui -o ui_SettingsTab.h

ui_RunConfig.h: RunConfig.ui
	/Developer/Tools/Qt/uic RunConfig.ui -o ui_RunConfig.h

ui_ProgressDialog.h: ProgressDialog.ui
	/Developer/Tools/Qt/uic ProgressDialog.ui -o ui_ProgressDialog.h

ui_resultwindow.h: resultwindow.ui
	/Developer/Tools/Qt/uic resultwindow.ui -o ui_resultwindow.h

ui_tabledata.h: dataviews/tabledata.ui
	/Developer/Tools/Qt/uic dataviews/tabledata.ui -o ui_tabledata.h

ui_welcome.h: welcome.ui
	/Developer/Tools/Qt/uic welcome.ui -o ui_welcome.h

ui_gridchoice.h: grid/gridchoice.ui
	/Developer/Tools/Qt/uic grid/gridchoice.ui -o ui_gridchoice.h

ui_gridsetup.h: grid/gridsetup.ui
	/Developer/Tools/Qt/uic grid/gridsetup.ui -o ui_gridsetup.h

ui_nonec2.h: grid/nonec2.ui
	/Developer/Tools/Qt/uic grid/nonec2.ui -o ui_nonec2.h

ui_ec2grid.h: grid/ec2grid.ui
	/Developer/Tools/Qt/uic grid/ec2grid.ui -o ui_ec2grid.h

ui_graphdata.h: dataviews/graphdata.ui
	/Developer/Tools/Qt/uic dataviews/graphdata.ui -o ui_graphdata.h

ui_searchwidget.h: popupviews/searchwidget.ui
	/Developer/Tools/Qt/uic popupviews/searchwidget.ui -o ui_searchwidget.h

ui_selectsamples.h: popupviews/selectsamples.ui
	/Developer/Tools/Qt/uic popupviews/selectsamples.ui -o ui_selectsamples.h

ui_exportwidget.h: popupviews/exportwidget.ui
	/Developer/Tools/Qt/uic popupviews/exportwidget.ui -o ui_exportwidget.h

ui_displayText.h: displayText.ui
	/Developer/Tools/Qt/uic displayText.ui -o ui_displayText.h

ui_displayinfo.h: popupviews/displayinfo.ui
	/Developer/Tools/Qt/uic popupviews/displayinfo.ui -o ui_displayinfo.h

ui_graphicsrepresentation.h: graphicsrepresentation.ui
	/Developer/Tools/Qt/uic graphicsrepresentation.ui -o ui_graphicsrepresentation.h

ui_meganbrowser.h: dataviews/meganbrowser.ui
	/Developer/Tools/Qt/uic dataviews/meganbrowser.ui -o ui_meganbrowser.h

ui_htabledata.h: datamodel/htabledata.ui
	/Developer/Tools/Qt/uic datamodel/htabledata.ui -o ui_htabledata.h

ui_mainframe.h: mainframe.ui
	/Developer/Tools/Qt/uic mainframe.ui -o ui_mainframe.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 
