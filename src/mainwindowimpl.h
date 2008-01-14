/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
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


#include "client/TimerManager.h"
#include "client/SearchManager.h"
#include "client/QueueManager.h"
#include "client/ClientManager.h"

//

#include "ui_mainwindow.h"

class ThreadGetTTH : public QThread
{
	Q_OBJECT
public:
	void run();
	void stop();
	const QString & getA();
	void setA(QString);
	const QString & getB();
	const QString & getC();
private:
	volatile bool _stp;
	QString a,b,c;
signals:
	void ready();
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow 
	,private dcpp::TimerManagerListener
	,private dcpp::QueueManagerListener
	,private dcpp::LogManagerListener
{
Q_OBJECT
public:
	void initMain();
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindowImpl();
	
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

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void setToolTip(const QString & title);	
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint = 10000);
	void slotCloseWinTypeHub();
	void slotCloseWinTypePM();
	void slotCloseWinTypeSearch();
	void slotCloseWinTypeFL();
	void slotCloseWinType(int type);
	void slotCloseDisconnectedHubs();
	void slotCloseOfflineLists();
	void slotCloseOfflineChat();
	void slotCurrentTabChanged(int index);
	void statusMessage(const QString & mes);
	void About();
	void updateWindowMenu();
	void openfilelistFunc();
	void OpenDownloadsFolderFunc();
	void RefreshOwnFileListFunc();
	void openownfilelistFunc();
	void DonateFunc();
	void DLQueueFunc();
	void notepadFunc();
	void indexingFunc();
	void ADLFunc();
	void SSFunc();
	void ULFinFunc();
	void DLFinFunc();
	void PubHubFunc();
	void HomepageFunc();
	void FavUsrFunc();
	void IgnoredUsrFunc();
	void SearchFunc();
	void qcdconFunc(QString);
	void show_tthFunc();
	void showhideFunc();
	void GetTTHFunc();
	void PreferencesFunc();
	void FavHubListFunc();
	void reconnectFunc();
	void fQuickConFunc();
	void statusbarcheck(); // if StatusBar Checked in menu @View@
	
	int handleSpeaker(unsigned int wParam, long lParam);
	bool eachSecond();
	
signals:
	int speakerSignal(unsigned int, long=0);
	
public slots:
	void OpenList(QWidget *, const dcpp::tstring & , const dcpp::UserPtr & , int64_t, const QString);
	void OpenPM(QWidget *parent, const QString &);
	void OpenHub(QWidget *parent, const dcpp::tstring& adr);

private:
	void createActions();
	void createTrayIcon();
	void createToolBars();
	void createStatusLabels();
	void clientInit();
	void startSocket();
	void setShareSize(const QString &sz);
	
	int FindWinByType(int type);

	QAction *showhide;

	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	//QLabel *shareStatusLbl;
	
	ThreadGetTTH thrdGetTTh;
	TabWidget *m_tabwin;
	
	//PORTED CODE
	//QLabel *statusStatusLabel;
	QLabel *statusAwayLabel;
	QLabel *statusCountsLabel;
	QLabel *statusSlotsLabel;
	QLabel *statusDownTotalLabel;
	QLabel *statusUpTotalLabel;
	QLabel *statusDownDiffLabel;
	QLabel *statusUpDiffLabel;
	
	enum { MAX_CLIENT_LINES = 10 };
	int64_t lastUp;
	int64_t lastDown;
	uint64_t lastTick;
	TStringList lastLinesList;
	tstring lastLines;
	bool speak(unsigned int, long=0);
	void initSecond();
	void updateStatus();
	void setStatus(int s, const tstring& text);
	void autoConnect(const FavoriteHubEntryList& fl);
	// LogManagerListener
	virtual void on(LogManagerListener::Message, time_t t, const string& m) throw() 
	{ speak(STATUS_MESSAGE, (long)new pair<time_t, tstring>(t, tstring(Text::toT(m)))); }

	// HttpConnectionListener
	//virtual void on(HttpConnectionListener::Complete, HttpConnection* conn, string const& /*aLine*/) throw();
	//virtual void on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const uint8_t* buf, size_t len) throw();

	// QueueManagerListener
	virtual void on(QueueManagerListener::Finished, QueueItem* qi, const string& dir, int64_t speed) throw();
	virtual void on(PartialList, const UserPtr&, const string& text) throw();
	
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
	
	enum Speaker {
		DOWNLOAD_LISTING,
		BROWSE_LISTING,
		AUTO_CONNECT,
		PARSE_COMMAND_LINE,
		VIEW_FILE_AND_DELETE,
		STATUS_MESSAGE,
		LAYOUT
	};
};

#endif

