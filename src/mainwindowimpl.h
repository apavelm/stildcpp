/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *   Copyright (C) 2007 -, 2008 by Yakov Suraev aka BigBiker               *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <sstream>
#include <boost/scoped_ptr.hpp>

#include "defs.h"

#include "config.h"
#include "waitusr.h"
#include "iconset.h"
#include "frmAbout.h"
#include "preferencesdialog.h"
#include "mdi_c.h"
#include "favHubLstWin.h"
#include "SearchWin.h"
#include "hub_win.h"
#include "indexing.h"
#include "publichubs.h"
#include "FavUsr.h"
#include "IgnUsr.h"
#include "ulfin.h"
#include "mdi_notepad.h"
#include "dlfin.h"
#include "dl_queue.h"
#include "pm_win.h"
#include "adlsearch.h"
#include "mdi_filelist.h"
#include "sspy.h"
#include "tthdial.h"
#include "quickconnectdlg.h"
#include "widgets/tabwidget.h"
#include "widgets/stil_TransferView.h"
#include "mdi_syslog.h"
#include "TextWindow.h"
#include "stats.h"

#include "stilutils.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/QueueItem.h"
#include "client/SimpleXML.h"
#include "client/LogManager.h"
#include "client/ConnectionManager.h"
#include "client/StringTokenizer.h"
#include "client/MerkleTree.h"
#include "client/Util.h"
#include "client/Text.h"
#include "client/File.h"

#include "client/FavoriteManager.h"
#include "client/QueueManagerListener.h"
#include "client/version.h"
#include "client/Client.h"
#include "client/ShareManager.h"
#include "client/DownloadManager.h"
#include "client/SettingsManager.h"

#include "client/Download.h"
#include "client/UploadManager.h"


#include "client/HttpConnection.h"
#include "client/SearchManager.h"
#include "client/QueueManager.h"
#include "client/ClientManager.h"

#include "client/Singleton.h"
//

#include "ui_mainwindow.h"

using namespace std;
using namespace dcpp;

class ThreadGetTTH : public QThread
{
	Q_OBJECT
public:
	void run();
	void stop() { _stp = true; }
	const QString & getA() { return a; }
	void setA(QString s) { a = s; }
	const QString & getB() { return b; }
	const QString & getC() { return c; }
private:
	volatile bool _stp;
	QString a,b,c;
signals:
	void ready();
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow 
	,private HttpConnectionListener
	,private QueueManagerListener
	,private LogManagerListener
	,public  Singleton<MainWindowImpl>
{
	Q_OBJECT
protected:
	void closeEvent(QCloseEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	friend class Singleton<MainWindowImpl>;
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindowImpl();
	
	
	class DirectoryListInfo {
	public:
		DirectoryListInfo(const UserPtr& aUser, const tstring& aFile, const tstring& aDir, int64_t aSpeed) : user(aUser), file(aFile), dir(aDir), speed(aSpeed) { }
		UserPtr user;
		tstring file;
		tstring dir;
		int64_t speed;
	};
	
	class DirectoryBrowseInfo {
	public:
		DirectoryBrowseInfo(const UserPtr& ptr, string aText) : user(ptr), text(aText) { }
		UserPtr user;
		string text;
	};
	
	// Moved to stilutils.h
	/*enum Speaker {
		DOWNLOAD_LISTING,
		BROWSE_LISTING,
		AUTO_CONNECT,
		PARSE_COMMAND_LINE,
		VIEW_FILE_AND_DELETE,
		STATUS_MESSAGE,
		LAYOUT
	};
	*/
	
	enum Status {
		STATUS_STATUS,
		STATUS_AWAY,
		STATUS_COUNTS,
		STATUS_SLOTS,
		STATUS_DOWN_TOTAL,
		STATUS_UP_TOTAL,
		STATUS_DOWN_DIFF,
		STATUS_UP_DIFF,
		STATUS_DUMMY,
		STATUS_LAST
	};
	
	enum { MAX_CLIENT_LINES = 10 };
	
	//
	
	QAction *showhide;
	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	ThreadGetTTH thrdGetTTh;
	TabWidget *m_tabwin;
	
	
	void initMain();
	void createActions();
	void createTrayIcon();
	void createToolBars();
	void createStatusLabels();
	void clientInit();
	void startSocket();
	
	int FindWinByType(StilUtils::tabWinTypes type);
	
	
	QLabel *statusAwayLabel;
	QLabel *statusCountsLabel;
	QLabel *statusSlotsLabel;
	QLabel *statusDownTotalLabel;
	QLabel *statusUpTotalLabel;
	QLabel *statusDownDiffLabel;
	QLabel *statusUpDiffLabel;
	
	int64_t lastUp;
	int64_t lastDown;
	uint64_t lastTick;
	TStringList lastLinesList;
	tstring lastLines;

	void updateStatus();
	void setStatus(int s, const tstring& text);
	void autoConnect(const FavoriteHubEntryList& fl);
	
	// LogManagerListener
	virtual void on(LogManagerListener::Message, time_t t, const string& m) throw() 
	{ 
		speak(StilUtils::STATUS_MESSAGE, (qint64)new pair<time_t, tstring>(t, tstring(Text::toT(m)))); 
	}

	// HttpConnectionListener
	HttpConnection* c;
	string versionInfo;
	virtual void on(HttpConnectionListener::Complete, HttpConnection* conn, string const& /*aLine*/) throw();
	virtual void on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const uint8_t* buf, size_t len) throw();

	// QueueManagerListener
	virtual void on(QueueManagerListener::Finished, QueueItem* qi, const string& dir, int64_t speed) throw();
	virtual void on(PartialList, const UserPtr&, const string& text) throw();

signals:
	void sigSpeak(StilUtils::Speaker, qint64);
	void signalForceCloseHashDialog();
	
public slots:
	// global functions
	void OpenList(QWidget *, const tstring & , const UserPtr & , int64_t, const QString, bool silent = false);
	void OpenPM(const UserPtr& replyTo, const tstring& aMessage = Util::emptyStringT, bool silent = false);
	void OpenHub(const tstring& adr, bool silent = false);
	void OpenSearch(const tstring& str = Util::emptyStringT, int64_t size = 0, SearchManager::SizeModes mode = SearchManager::SIZE_ATLEAST, SearchManager::TypeModes type = SearchManager::TYPE_ANY, bool silent = false);
	void openTextWindow(const string& fileName, bool silent = false);
	void openTextWindow(const tstring& fileName, bool silent = false);
	void openTextWindow(const QString& fileName, bool silent = false);
	void ShowHashDlg(bool autoClose = false);
	void OpenSingleTab(StilUtils::tabWinTypes, bool silent = false);
	
private slots:
	void slotSpeak(StilUtils::Speaker, qint64);
	void speak(StilUtils::Speaker a, qint64 b = 0) { emit sigSpeak(a, b); }

	void setToolTip(const QString & title);
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint = 10000);
	void slotCurrentTabChanged(int index);
	void statusMessage(const QString & mes);
	void About();
	void updateWindowMenu();
	void openfilelistFunc();
	void OpenDownloadsFolderFunc();
	void RefreshOwnFileListFunc();
	void openownfilelistFunc();
	void DonateFunc();
	void indexingFunc();
	void HomepageFunc();
	void qcdconFunc(QString);
	void show_tthFunc();
	void showhideFunc();
	void GetTTHFunc();
	void PreferencesFunc();
	void reconnectFunc();
	void fQuickConFunc();
	void statusbarcheck();
	
	// slots for Closing Specific Tabs
	void slotCloseWinType(StilUtils::tabWinTypes type);
	void slotCloseDisconnectedHubs();
	void slotCloseOfflineLists();
	void slotCloseOfflineChat();
	void slotCloseWinTypeHub()	 { slotCloseWinType(StilUtils::WIN_TYPE_HUB); }
	void slotCloseWinTypePM()	 { slotCloseWinType(StilUtils::WIN_TYPE_PRIVATE_CHAT); }
	void slotCloseWinTypeSearch()	 { slotCloseWinType(StilUtils::WIN_TYPE_SEARCH); }
	void slotCloseWinTypeFL()	 { slotCloseWinType(StilUtils::WIN_TYPE_FILELIST); }
	
	// slots for opening Single Tabs
	void DLQueueFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_DL_QUEUE); }
	void notepadFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_NOTEPAD); }
	void SLogFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_SYSTEM_LOG); }
	void ADLFunc()			 { OpenSingleTab(StilUtils::WIN_TYPE_ADL_SEARCH); }
	void SSFunc()			 { OpenSingleTab(StilUtils::WIN_TYPE_SEARCH_SPY); }
	void ULFinFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_FINISHED_UL); }
	void WaitingFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_WAITING_USERS); }
	void DLFinFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_FINISHED_DL); }
	void PubHubFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_PUBLIC_HUBS); }
	void FavUsrFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_FAVORITE_USERS); }
	void StatsFunc()		 { OpenSingleTab(StilUtils::WIN_TYPE_NETWORK_STATS); }
	void IgnoredUsrFunc()	 { OpenSingleTab(StilUtils::WIN_TYPE_IGNORED_USERS); }
	void FavHubListFunc()	 { OpenSingleTab(StilUtils::WIN_TYPE_FAVORITE_HUB_LIST); }
};

#endif
