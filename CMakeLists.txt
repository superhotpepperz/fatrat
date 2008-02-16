project(fatrat)

cmake_minimum_required(VERSION 2.4.0)
find_package(Qt4 REQUIRED)

set(CMAKE_MODULE_PATH cmake_modules)

if(WITH_BITTORRENT)
	set(libtorrent_DIR ${CMAKE_MODULE_PATH})
	find_package(libtorrent REQUIRED)
	
	if(LIBTORRENT_FOUND)
		message(STATUS "libtorrent found OK")
		include_directories(${LIBTORRENT_INCLUDE_DIRS})
	else(LIBTORRENT_FOUND)
		message(FATAL_ERROR "No Rasterbar libtorrent")
	endif(LIBTORRENT_FOUND)
endif(WITH_BITTORRENT)

if(WITH_SFTP)
	set(libssh2_DIR ${CMAKE_MODULE_PATH})
	find_package(libssh2 REQUIRED)
	
	if(LIBSSH2_FOUND)
		message(STATUS "libssh2 found OK")
		include_directories(${LIBSSH2_INCLUDE_DIRS})
	else(LIBSSH2_FOUND)
		message(FATAL_ERROR "No libssh2")
	endif(LIBSSH2_FOUND)
endif(WITH_SFTP)

CONFIGURE_FILE(config.h.in config.h)

#QT4_ADD_DBUS_INTERFACE (fatrat_SRCS dbus/interface.xml dbusinterface)
#set(QT_USE_QTDBUS TRUE)

set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QTXML TRUE)
set(QT_USE_QTSVG TRUE)

set(QT_QTDBUS_LIBRARY QtDBus)

include( ${QT_USE_FILE} )
add_definitions(${QT_DEFINITIONS})
include_directories(./ ${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})

set(fatrat_SRCS
	AppTools.cpp
	DropBox.cpp
	fatrat.cpp
	InfoBar.cpp
	LimitedSocket.cpp
	MainTab.cpp
	MainWindow.cpp
	NetIface.cpp
	NewTransferDlg.cpp
	Queue.cpp
	QueueMgr.cpp
	QueueView.cpp
	SettingsDlg.cpp
	SettingsNetworkForm.cpp
	SimpleEmail.cpp
	SpeedGraph.cpp
	SpeedLimitWidget.cpp
	StatsWidget.cpp
	Transfer.cpp
	TransfersModel.cpp

	remote/GenericService.cpp
	remote/HttpService.cpp
	remote/XmlService.cpp

	tooltips/BaseToolTip.cpp
	tooltips/QueueToolTip.cpp
	tooltips/TrayToolTip.cpp

	engines/FtpClient.cpp
	engines/FtpUpload.cpp
	engines/GeneralDownload.cpp
	engines/HttpClient.cpp
	engines/HttpFtpSettings.cpp
	engines/RapidshareUpload.cpp

	dbus/DbusAdaptor.cpp
	dbus/DbusImpl.cpp
	tools/HashDlg.cpp
	tools/RapidTools.cpp
)

set(fatrat_MOC_HDRS
	dbus/DbusAdaptor.h
	dbus/DbusImpl.h
	
	TransfersModel.h
	QueueDlg.h
	QueueMgr.h
	SpeedGraph.h
	SettingsDropBoxForm.h
	SettingsNetworkForm.h
	StatsWidget.h
	ProxyDlg.h
	SpeedLimitWidget.h
	TransferLog.h
	SettingsGeneralForm.h
	Transfer.h
	SimpleEmail.h
	LimitedSocket.h
	QueueView.h
	MainWindow.h
	AboutDlg.h
	SettingsDlg.h
	TransfersView.h
	Queue.h
	MainTab.h
	DropBox.h
	InfoBar.h
	GenericOptsForm.h
	WidgetHostDlg.h
	UserAuthDlg.h
	NewTransferDlg.h
	
	tools/HashDlg.h
	tools/RapidTools.h
	
	engines/HttpFtpSettings.h
	engines/RapidshareUpload.h
	engines/FtpClient.h
	engines/GeneralDownload.h
	engines/HttpClient.h
	engines/FtpUpload.h
	
	tooltips/QueueToolTip.h
	tooltips/TrayToolTip.h
	tooltips/BaseToolTip.h
)

set(fatrat_UIS
	AboutDlg.ui
	CommentForm.ui
	GenericOptsForm.ui
	MainWindow.ui
	NewTransferDlg.ui
	ProxyDlg.ui
	QueueDlg.ui
	SettingsDlg.ui
	SettingsDropBoxForm.ui
	SettingsGeneralForm.ui
	SettingsNetworkForm.ui
	ScheduleForm.ui
	SpeedLimitWidget.ui
	UserAuthDlg.ui
	WidgetHostDlg.ui

	tools/HashDlg.ui
	tools/RapidTools.ui

	engines/FtpUploadOptsForm.ui
	engines/HttpOptsWidget.ui
	engines/HttpUrlOptsDlg.ui
	engines/SettingsHttpForm.ui
)

if(WITH_SFTP)
	set(fatrat_SRCS
		${fatrat_SRCS}
		engines/SftpClient.cpp
	)
	set(fatrat_MOC_HDRS
		${fatrat_MOC_HDRS}
		engines/SftpClient.h
	)
endif(WITH_SFTP)

if(WITH_BITTORRENT)
	set(fatrat_SRCS
		${fatrat_SRCS}
		engines/TorrentDownload.cpp
		engines/TorrentFilesModel.cpp
		engines/TorrentIPFilter.cpp
		engines/TorrentPeersModel.cpp
		engines/TorrentPiecesModel.cpp
		engines/TorrentProgressWidget.cpp
		engines/TorrentSettings.cpp
		tools/TorrentSearch.cpp
	)
	set(fatrat_MOC_HDRS
		${fatrat_MOC_HDRS}
		tools/TorrentSearch.h
		engines/TorrentPeersModel.h
		engines/TorrentDownload.h
		engines/TorrentProgressWidget.h
		engines/TorrentPiecesModel.h
		engines/TorrentFilesModel.h
		engines/TorrentSettings.h
	)
	set(fatrat_UIS
		${fatrat_UIS}
		tools/TorrentSearch.ui
		engines/TorrentDetailsForm.ui
		engines/TorrentOptsWidget.ui
		engines/SettingsTorrentForm.ui
	)
endif(WITH_BITTORRENT)

qt4_wrap_cpp(fatrat_MOC_SRCS ${fatrat_MOC_HDRS})

set(fatrat_RCS
	gfx/resources.qrc
)

qt4_wrap_ui(fatrat_UIS_H ${fatrat_UIS})
qt4_add_resources(fatrat_QRC_H gfx/resources.qrc)

add_executable(fatrat ${fatrat_SRCS} ${fatrat_MOC_SRCS} ${fatrat_UIS_H}
${fatrat_QRC_H})

target_link_libraries(fatrat ${QT_LIBRARIES} ${LIBTORRENT_LIBRARIES} ${LIBSSH2_LIBRARIES} ${QT_QTDBUS_LIBRARY})