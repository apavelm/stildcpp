TEMPLATE = app
QT = gui core xml
CONFIG += qt \
 warn_on \
 console \
 build_all \
 debug \
 exception \
 silent
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
 ui/about.ui \
 ui/preferencesdialog.ui \
 ui/PMWindow.ui \
 ui/HUBWindow.ui \
 ui/search.ui \
 ui/favhublist.ui \
 ui/quickconnect.ui \
 ui/tthdialog.ui \
 ui/filelistdialog.ui \
 ui/dlqueue.ui \
 ui/fav_users.ui \
 ui/ign_users.ui \
 ui/pubhub.ui \
 ui/ul_finished.ui \
 ui/dl_finished.ui \
 ui/adl.ui \
 ui/searchspy.ui \
 ui/notepad.ui \
 ui/TextWindow.ui \
 ui/syslog.ui \
 ui/DownLoads.ui \
 ui/hashman.ui
HEADERS = src/mainwindowimpl.h \
 src/frmAbout.h \
 src/mdi_c.h \
 src/preferencesdialog.h \
 src/config.h \
 src/quickconnectdlg.h \
 src/hub_win.h \
 src/highlighter.h \
 src/tthdial.h \
 src/mdi_filelist.h \
 src/defs.h \
 src/pm_win.h \
 src/widgets/tabwidget.h \
 src/favHubLstWin.h \
 src/SearchWin.h \
 src/dl_queue.h \
 src/FavUsr.h \
 src/IgnUsr.h \
 src/publichubs.h \
 src/dlfin.h \
 src/ulfin.h \
 src/adlsearch.h \
 src/sspy.h \
 src/client/AdcCommand.h \
 src/client/AdcHub.h \
 src/client/ADLSearch.h \
 src/client/BitInputStream.h \
 src/client/BitOutputStream.h \
 src/client/BloomFilter.h \
 src/client/BufferedSocket.h \
 src/client/BufferedSocketListener.h \
 src/client/BZUtils.h \
 src/client/CID.h \
 src/client/Client.h \
 src/client/ClientListener.h \
 src/client/ClientManager.h \
 src/client/ClientManagerListener.h \
 src/client/ConnectionManager.h \
 src/client/ConnectionManagerListener.h \
 src/client/CriticalSection.h \
 src/client/CryptoManager.h \
 src/client/DCPlusPlus.h \
 src/client/DirectoryListing.h \
 src/client/Download.h \
 src/client/DownloadManager.h \
 src/client/DownloadManagerListener.h \
 src/client/Encoder.h \
 src/client/Exception.h \
 src/client/FastAlloc.h \
 src/client/FavoriteManager.h \
 src/client/FavoriteManagerListener.h \
 src/client/FavoriteUser.h \
 src/client/File.h \
 src/client/FilteredFile.h \
 src/client/FinishedManager.h \
 src/client/FinishedManagerListener.h \
 src/client/Flags.h \
 src/client/forward.h \
 src/client/HashManager.h \
 src/client/HashManagerListener.h \
 src/client/HashValue.h \
 src/client/HttpConnection.h \
 src/client/HubEntry.h \
 src/client/LogManager.h \
 src/client/MerkleCheckOutputStream.h \
 src/client/MerkleTree.h \
 src/client/NmdcHub.h \
 src/client/Pointer.h \
 src/client/QueueItem.h \
 src/client/QueueManager.h \
 src/client/QueueManagerListener.h \
 src/client/ResourceManager.h \
 src/client/SearchManager.h \
 src/client/SearchManagerListener.h \
 src/client/Segment.h \
 src/client/Semaphore.h \
 src/client/ServerSocket.h \
 src/client/SettingsManager.h \
 src/client/SFVReader.h \
 src/client/ShareManager.h \
 src/client/SimpleXML.h \
 src/client/Singleton.h \
 src/client/Socket.h \
 src/client/Speaker.h \
 src/client/SSL.h \
 src/client/SSLSocket.h \
 src/client/stdinc.h \
 src/client/Streams.h \
 src/client/StringSearch.h \
 src/client/StringTokenizer.h \
 src/client/TaskQueue.h \
 src/client/Text.h \
 src/client/Thread.h \
 src/client/TigerHash.h \
 src/client/TimerManager.h \
 src/client/Transfer.h \
 src/client/Upload.h \
 src/client/UploadManager.h \
 src/client/UploadManagerListener.h \
 src/client/User.h \
 src/client/UserCommand.h \
 src/client/UserConnection.h \
 src/client/UserConnectionListener.h \
 src/client/Util.h \
 src/client/version.h \
 src/client/ZUtils.h \
 src/stilutils.h \
 src/mdi_notepad.h \
 src/TextWindow.h \
 src/mdi_syslog.h \
 src/UserInfoBase.h \
 src/indexing.h \
 src/widgets/stil_textview.h \
 src/widgets/stil_chatctrls.h \
 src/widgets/stil_richtext.h \
 src/iconset.h \
 src/textutil.h \
 src/rtparse.h \
 src/zstream.h \
 src/useractmenu.h \
 src/client/HashBloom.h \
 src/userlist.h \
 src/userlistsortingmodel.h \
 src/widgets/stil_TransferView.h \
 src/widgets/myprogress.h \
 src/DownLoads.h \
 src/widgets/stil_qdltreewidget.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/frmAbout.cpp \
 src/mdi_c.cpp \
 src/preferencesdialog.cpp \
 src/config.cpp \
 src/quickconnectdlg.cpp \
 src/hub_win.cpp \
 src/highlighter.cpp \
 src/tthdial.cpp \
 src/mdi_filelist.cpp \
 src/defs.cpp \
 src/pm_win.cpp \
 src/widgets/tabwidget.cpp \
 src/favHubLstWin.cpp \
 src/SearchWin.cpp \
 src/dl_queue.cpp \
 src/FavUsr.cpp \
 src/IgnUsr.cpp \
 src/publichubs.cpp \
 src/dlfin.cpp \
 src/ulfin.cpp \
 src/adlsearch.cpp \
 src/sspy.cpp \
 src/client/AdcCommand.cpp \
 src/client/AdcHub.cpp \
 src/client/ADLSearch.cpp \
 src/client/BufferedSocket.cpp \
 src/client/BZUtils.cpp \
 src/client/Client.cpp \
 src/client/ClientManager.cpp \
 src/client/ConnectionManager.cpp \
 src/client/CryptoManager.cpp \
 src/client/DCPlusPlus.cpp \
 src/client/DirectoryListing.cpp \
 src/client/Download.cpp \
 src/client/DownloadManager.cpp \
 src/client/Encoder.cpp \
 src/client/Exception.cpp \
 src/client/FavoriteManager.cpp \
 src/client/File.cpp \
 src/client/FinishedManager.cpp \
 src/client/HashManager.cpp \
 src/client/HttpConnection.cpp \
 src/client/LogManager.cpp \
 src/client/NmdcHub.cpp \
 src/client/QueueItem.cpp \
 src/client/QueueManager.cpp \
 src/client/ResourceManager.cpp \
 src/client/SearchManager.cpp \
 src/client/ServerSocket.cpp \
 src/client/SettingsManager.cpp \
 src/client/SFVReader.cpp \
 src/client/ShareManager.cpp \
 src/client/SimpleXML.cpp \
 src/client/Socket.cpp \
 src/client/SSL.cpp \
 src/client/SSLSocket.cpp \
 src/client/stdinc.cpp \
 src/client/StringTokenizer.cpp \
 src/client/Text.cpp \
 src/client/Thread.cpp \
 src/client/TigerHash.cpp \
 src/client/TimerManager.cpp \
 src/client/Transfer.cpp \
 src/client/Upload.cpp \
 src/client/UploadManager.cpp \
 src/client/User.cpp \
 src/client/UserConnection.cpp \
 src/client/Util.cpp \
 src/client/ZUtils.cpp \
 src/stilutils.cpp \
 src/mdi_notepad.cpp \
 src/TextWindow.cpp \
 src/mdi_syslog.cpp \
 src/UserInfoBase.cpp \
 src/indexing.cpp \
 src/widgets/stil_textview.cpp \
 src/widgets/stil_chatctrls.cpp \
 src/widgets/stil_richtext.cpp \
 src/iconset.cpp \
 src/textutil.cpp \
 src/rtparse.cpp \
 src/zstream.cpp \
 src/useractmenu.cpp \
 src/client/HashBloom.cpp \
 src/userlist.cpp \
 src/userlistsortingmodel.cpp \
 src/widgets/stil_TransferView.cpp \
 src/widgets/myprogress.cpp \
 src/DownLoads.cpp \
 src/widgets/stil_qdltreewidget.cpp
RESOURCES += src/stildcpp.qrc src/langs.qrc
LIBS += -lssl -lbz2 -lcrypto
TRANSLATIONS = lang/stildcpp_ru.ts
DEFINES += _DEBUG \
 BOOST_HAS_GCC_TR1 \
 UNICODE \
 _UNICODE \
 BUILDING_DCPP \
 DEBUG \
 USE_SYS_STL=1 \
 BUILDING_DCPP
