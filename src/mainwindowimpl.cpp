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

#include "mainwindowimpl.h"

#include <QtDebug>

using namespace std;
using namespace dcpp;
//
MainWindowImpl::~MainWindowImpl()
{
	thrdGetTTh.stop();
	
	trans_view->preClose();
	
	dcpp::SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_POS_X, static_cast<int>(this->x()));
	dcpp::SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_POS_Y, static_cast<int>(this->y()));
	dcpp::SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_SIZE_X, static_cast<int>(this->size().width()));
	dcpp::SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_SIZE_Y, static_cast<int>(this->size().height()));
	dcpp::SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_STATE, (int)this->isMaximized());
	
	dcpp::SettingsManager::getInstance()->save();
	AppSettings::AppSettingsMgr::getInstance()->save();
	QueueManager::getInstance()->saveQueue();
	
	delete showhide;
	delete trayIcon;
	delete trayIconMenu;
	//delete statusStatusLabel;
	delete statusAwayLabel;
	delete statusCountsLabel;
	delete statusSlotsLabel;
	delete statusDownTotalLabel;
	delete statusUpTotalLabel;
	delete statusDownDiffLabel;
	delete statusUpDiffLabel;
	//delete shareStatusLbl;
	delete m_tabwin;
	thrdGetTTh.wait();
	
	dcpp::LogManager::getInstance()->removeListener(this);
	dcpp::QueueManager::getInstance()->removeListener(this);
	dcpp::TimerManager::getInstance()->removeListener(this);	
	
	SearchManager::getInstance()->disconnect();
	ConnectionManager::getInstance()->disconnect();
	AppSettings::AppSettingsMgr::deleteInstance();
	dcpp::shutdown();
}

void MainWindowImpl::initMain()
{
	setupUi(this);
	
	m_tabwin = new TabWidget( this );
	connect(m_tabwin, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
	setCentralWidget(m_tabwin);
	m_tabwin->setOpt(APPSETTING(i_TABPOSIOTION));
	
	int pos_x= SETTING(MAIN_WINDOW_POS_X);
	int pos_y= SETTING(MAIN_WINDOW_POS_Y);
	int size_x= SETTING(MAIN_WINDOW_SIZE_X);
	int size_y= SETTING(MAIN_WINDOW_SIZE_Y);
	int maxzd= SETTING(MAIN_WINDOW_STATE);
	this->setGeometry(pos_x,pos_y,size_x,size_y);
	if (maxzd) this->setWindowState(Qt::WindowMaximized);

	
	createActions();
	createToolBars();
	createTrayIcon();

	connect(this, SIGNAL(speakerSignal(unsigned int, long)), this, SLOT(handleSpeaker(unsigned int, long)),Qt::QueuedConnection);
//	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));	
	updateWindowMenu();
	connect(menuWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
	
	setToolTip(tr(APPLICATIONNAME));
	setWindowTitle(tr(APPLICATIONNAME));
	
	//shareStatusLbl = new QLabel;
	//setShareSize(tr("Total shared: 0b"));
	//statusBar()->addPermanentWidget(shareStatusLbl);
	createStatusLabels();
	
	trayIcon->setIcon(QIcon(":/images/icon.png"));
	
	setWindowIcon(QIcon(":/images/icon.png"));
	
	clientInit();
	
	if (APPSETTING(i_USETRAY)) 
		trayIcon->show();

	if (!APPSETTING(i_STARTHIDDEN))
		this->show();
	else
		if (!APPSETTING(i_USETRAY)) this->show();
}

void MainWindowImpl::createStatusLabels()
{
	//This is stupid code, but this is also workaround for setting labels size
	 
	//statusStatusLabel = new QLabel;
	statusAwayLabel = new QLabel(tr(" AWAY "));
	statusAwayLabel->setAlignment(Qt::AlignHCenter);
	statusAwayLabel->setMinimumSize(statusAwayLabel->sizeHint());

	statusCountsLabel = new QLabel(" 9/9/9 ");
	statusCountsLabel->setAlignment(Qt::AlignHCenter);
	statusCountsLabel->setMinimumSize(statusCountsLabel->sizeHint());
	
	statusSlotsLabel = new QLabel("Slots: 99/99");
	statusSlotsLabel->setMinimumSize(statusSlotsLabel->sizeHint());
	
	statusDownTotalLabel = new QLabel("D: 999.99MiB");
	statusDownTotalLabel->setMinimumSize(statusDownTotalLabel->sizeHint());
	
	statusUpTotalLabel = new QLabel("U: 999.99MiB");
	statusUpTotalLabel->setMinimumSize(statusUpTotalLabel->sizeHint());
	
	statusDownDiffLabel = new QLabel("D: 999.99MiB/s (99)");
	statusDownDiffLabel->setMinimumSize(statusDownDiffLabel->sizeHint());
	
	statusUpDiffLabel = new QLabel("U: 999.99MiB/s (99)");
	statusUpDiffLabel->setMinimumSize(statusUpDiffLabel->sizeHint());
	
	//statusBar()->addWidget(statusStatusLabel, 1);
	statusBar()->addPermanentWidget(statusAwayLabel);
	statusBar()->addPermanentWidget(statusCountsLabel);
	statusBar()->addPermanentWidget(statusSlotsLabel);
	statusBar()->addPermanentWidget(statusDownTotalLabel);
	statusBar()->addPermanentWidget(statusUpTotalLabel);
	statusBar()->addPermanentWidget(statusDownDiffLabel);
	statusBar()->addPermanentWidget(statusUpDiffLabel);
	
}

void MainWindowImpl::initSecond()
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
	timer->start(1000);
}

bool MainWindowImpl::eachSecond()
{
	updateStatus();
	return true;
}

void MainWindowImpl::updateStatus()
{
	uint64_t now = GET_TICK();
	uint64_t tdiff = now - lastTick;
	if (tdiff < 100) {
		tdiff = 1;
	}

	uint64_t up = Socket::getTotalUp();
	uint64_t down = Socket::getTotalDown();
	uint64_t updiff = up - lastUp;
	uint64_t downdiff = down - lastDown;
	
	lastTick = now;
	lastUp = up;
	lastDown = down;

	/** @todo move this to client/ */
	SettingsManager::getInstance()->set(SettingsManager::TOTAL_UPLOAD, SETTING(TOTAL_UPLOAD) + static_cast<int64_t>(updiff));
	SettingsManager::getInstance()->set(SettingsManager::TOTAL_DOWNLOAD, SETTING(TOTAL_DOWNLOAD) + static_cast<int64_t>(downdiff));

	setStatus(STATUS_AWAY, Util::getAway() ? T_("AWAY") : _T(""));
	setStatus(STATUS_COUNTS, Text::toT(Client::getCounts()));
	setStatus(STATUS_SLOTS, Text::toT(_( "Slots: ") + Util::toString(UploadManager::getInstance()->getFreeSlots()) + '/' + Util::toString(SETTING(SLOTS))));
	setStatus(STATUS_DOWN_TOTAL, Text::toT("D: " + Util::formatBytes(down)));
	setStatus(STATUS_UP_TOTAL, Text::toT("U: " + Util::formatBytes(up)));
	setStatus(STATUS_DOWN_DIFF, Text::toT("D: " + Util::formatBytes((downdiff*1000)/tdiff) + "/s ("
	    + Util::toString(DownloadManager::getInstance()->getDownloadCount()) + ")"));
	setStatus(STATUS_UP_DIFF, Text::toT("U: " + Util::formatBytes((updiff*1000)/tdiff) + "/s ("
	    + Util::toString(UploadManager::getInstance()->getUploadCount()) + ")"));
}

void MainWindowImpl::clientInit()
{

	updateStatus();
	
	dcpp::QueueManager::getInstance()->addListener(this);
	dcpp::LogManager::getInstance()->addListener(this);
	
	TimerManager::getInstance()->start();
	
	dcpp::TimerManager::getInstance()->addListener(this);
	
	startSocket();
	
	//if(BOOLSETTING(OPEN_SYSTEM_LOG)) postMessage(WM_COMMAND, IDC_SYSTEM_LOG);
	if(BOOLSETTING(OPEN_FAVORITE_USERS)) FavUsrFunc();
	if(BOOLSETTING(OPEN_QUEUE)) DLQueueFunc();
	if(BOOLSETTING(OPEN_FINISHED_DOWNLOADS)) DLFinFunc();
	//if(BOOLSETTING(OPEN_WAITING_USERS)) postMessage(WM_COMMAND, IDC_WAITING_USERS);
	if(BOOLSETTING(OPEN_FINISHED_UPLOADS)) ULFinFunc();
	if(BOOLSETTING(OPEN_SEARCH_SPY)) SSFunc();
	//if(BOOLSETTING(OPEN_NETWORK_STATISTICS)) postMessage(WM_COMMAND, IDC_NET_STATS);
	if(BOOLSETTING(OPEN_NOTEPAD)) notepadFunc();
	if(BOOLSETTING(OPEN_PUBLIC)) PubHubFunc();
	if(BOOLSETTING(OPEN_FAVORITE_HUBS)) FavHubListFunc();
	
	speak(AUTO_CONNECT);
	
	// If First-time launch
	if(SETTING(NICK).empty()) 
	{ 
		// open settings dialog
		//PreferencesFunc();
	}
	
}

void MainWindowImpl::startSocket() 
{
	dcpp::SearchManager::getInstance()->disconnect();
	dcpp::ConnectionManager::getInstance()->disconnect();

	if (dcpp::ClientManager::getInstance()->isActive()) 
	{
		dcpp::ConnectionManager::getInstance()->listen();
		dcpp::SearchManager::getInstance()->listen();
	}

}

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	initMain();
	initSecond();
}

void MainWindowImpl::closeEvent(QCloseEvent *event)
{
	if (APPSETTING(i_HIDEONCLOSE))
	{
		if (trayIcon->isVisible()) 
			{
				hide();
				event->ignore();
			}
	} 
	else 
		{
			if (APPSETTING(i_PROMPTONCLOSE))
			{
				QMessageBox::StandardButton reply;
				reply = QMessageBox::question(this, tr("StilDC++"), tr("Do you realy want to exit?"), QMessageBox::Yes | QMessageBox::No);
				if (reply == QMessageBox::Yes)
				{
					event->accept();
				}
				else event->ignore();
			} else
			{
				event->accept();
			}
		}
}

void MainWindowImpl::setToolTip(const QString & title)
{
	trayIcon->setToolTip(title);
}

void MainWindowImpl::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) 
	{
		case QSystemTrayIcon::Trigger:
		{	// Left Mouse Button
			if (this->isHidden()) this->show(); else this->hide();
		}; break;
//		case QSystemTrayIcon::DoubleClick:
//			showMessage("Title","Text",0,2000);
	case QSystemTrayIcon::MiddleClick:
			statusMessage("Ready"); break;
	default:
	;
	}
}

void MainWindowImpl::showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint)
{
// Show message in Tray Icon
QSystemTrayIcon::MessageIcon icon;
// icon = { NoIcon, Information, Warning, Critical }
	switch (type) {
		case 0: icon = QSystemTrayIcon::NoIcon; break;
		case 1: icon = QSystemTrayIcon::Information; break;
		case 2: icon = QSystemTrayIcon::Warning; break;
		case 3: icon = QSystemTrayIcon::Critical; break;
		default: icon = QSystemTrayIcon::Information;
	}
	trayIcon->showMessage(title, message, icon, millisecondsTimeoutHint);
}

void MainWindowImpl::createActions()
{	
	showhide = new QAction(tr("Show / Hide"), this);
	showhide->setStatusTip(tr("Show / Hide application window"));
	showhide->setToolTip(tr("Show / Hide application window"));
	connect(showhide, SIGNAL(triggered()), this, SLOT(showhideFunc()));
	
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	
	connect(actionOpen_filelist, SIGNAL(triggered()), this, SLOT(openfilelistFunc()));
	connect(actionOpen_own_filelist, SIGNAL(triggered()), this, SLOT(openownfilelistFunc()));
	connect(actionOpen_Downloads_Folder, SIGNAL(triggered()), this, SLOT(OpenDownloadsFolderFunc()));
	connect(actionRefresh_own_filelist, SIGNAL(triggered()), this, SLOT(RefreshOwnFileListFunc()));

	connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));	
	connect(aboutqtact, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(actionStatusBar, SIGNAL(triggered()), this, SLOT(statusbarcheck()));

	
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(PreferencesFunc()));
	connect(actionReconnect, SIGNAL(triggered()), this, SLOT(reconnectFunc()));
	

	connect(actionDonate, SIGNAL(triggered()), this, SLOT(DonateFunc()));
	connect(actionHomepage, SIGNAL(triggered()), this, SLOT(HomepageFunc()));
	connect(actionSearch, SIGNAL(triggered()), this, SLOT(SearchFunc()));
	connect(actionGet_TTH_for_file, SIGNAL(triggered()), this, SLOT(GetTTHFunc()));
	
	connect(actionFavorite_Hubs, SIGNAL(triggered()), this, SLOT(FavHubListFunc()));
	connect(actionPublic_Hubs, SIGNAL(triggered()), this, SLOT(PubHubFunc()));
	connect(actionQuick_Connect, SIGNAL(triggered()), this, SLOT(fQuickConFunc()));
	connect(actionDownload_Queue, SIGNAL(triggered()), this, SLOT(DLQueueFunc()));
	connect(actionFavorite_Users, SIGNAL(triggered()), this, SLOT(FavUsrFunc()));
	connect(actionIgnored_Users, SIGNAL(triggered()), this, SLOT(IgnoredUsrFunc()));
	
	connect(actionFinished_downloads, SIGNAL(triggered()), this, SLOT(DLFinFunc()));
	connect(actionFinished_Uploads, SIGNAL(triggered()), this, SLOT(ULFinFunc()));
	connect(actionNotePad, SIGNAL(triggered()), this, SLOT(notepadFunc()));
	connect(actionIndexing_Progress, SIGNAL(triggered()), this, SLOT(indexingFunc()));
	
	
	connect(actionADL_Search, SIGNAL(triggered()), this, SLOT(ADLFunc()));
	connect(actionSearch_Spy, SIGNAL(triggered()), this, SLOT(SSFunc()));
	
	connect(&thrdGetTTh, SIGNAL(ready()), this, SLOT(show_tthFunc()));
	
	connect(actionClose_All, SIGNAL(triggered()), m_tabwin, SLOT(slotCloseAllTab()));
	
	connect(actionClose_All_Hub_Windows, SIGNAL(triggered()), this, SLOT(slotCloseWinTypeHub()));
	connect(actionClose_All_Private_Chat_Windows, SIGNAL(triggered()), this, SLOT(slotCloseWinTypePM()));
	connect(actionClose_All_Search_Windows, SIGNAL(triggered()), this, SLOT(slotCloseWinTypeSearch()));
	connect(actionClose_All_FileList_Windows, SIGNAL(triggered()), this, SLOT(slotCloseWinTypeFL()));
	connect(actionClose_Disconnected_Hub_Windows, SIGNAL(triggered()), this, SLOT(slotCloseDisconnectedHubs()));
	connect(actionClose_FileLists_Offline_Users, SIGNAL(triggered()), this, SLOT(slotCloseOfflineLists()));
	connect(actionClose_Private_Chat_with_Offline_Users, SIGNAL(triggered()), this, SLOT(slotCloseOfflineChat()));
	
}

void MainWindowImpl::showhideFunc()
{
	if (this->isHidden()) this->show(); else this->hide();
}

void MainWindowImpl::createTrayIcon()
{	
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(showhide);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionSettings);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionExit);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}

void MainWindowImpl::createToolBars()
{
	toolBar->setToolButtonStyle( Qt::ToolButtonStyle(APPSETTING(i_TOOLBARICONSTYLE)) );
	toolBar->setIconSize(QSize(APPSETTING(i_TOOLBARICONSIZE), APPSETTING(i_TOOLBARICONSIZE)));
	QAction *p = toolBar->toggleViewAction();
	p->setShortcut(QKeySequence(tr("Ctrl+2")));
	menuView->addAction(p);
	p = TransDock->toggleViewAction();
	p->setShortcut(QKeySequence(tr("Ctrl+3")));
	menuView->addAction(p);
	
	// Insert Actions to "Main Bar"
	toolBar->addAction(actionPublic_Hubs);
	toolBar->addAction(actionReconnect);
	toolBar->addAction(actionFollow_last_redirect);
	toolBar->addSeparator();
	toolBar->addAction(actionFavorite_Hubs);
	toolBar->addAction(actionFavorite_Users);
	toolBar->addAction(actionIgnored_Users);
	toolBar->addSeparator();
	toolBar->addAction(actionDownload_Queue);
	toolBar->addAction(actionFinished_downloads);
	toolBar->addAction(actionFinished_Uploads);
	toolBar->addSeparator();
	toolBar->addAction(actionSearch);
	toolBar->addAction(actionADL_Search);
	toolBar->addAction(actionSearch_Spy);
	toolBar->addSeparator();
	toolBar->addAction(actionSettings);
	toolBar->addAction(actionOpen_filelist);
	toolBar->addAction(actionGet_TTH_for_file);
	//toolBar->addSeparator();

}

void MainWindowImpl::statusMessage(const QString & mes)
{
	statusbar->showMessage(mes);
}

void MainWindowImpl::About()
{
	new AboutDialog(this);
}

int MainWindowImpl::FindWinByType(int type)
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==type)
		{
			return i;
		}
	}
	return -1;
}

void MainWindowImpl::PreferencesFunc()
{
	new PreferencesDialog(this);
}

void MainWindowImpl::DonateFunc()
{
	QUrl url = QUrl(APPDONATEPAGE);
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::HomepageFunc()
{
	QUrl url = QUrl(APPHOMEPAGE);
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::OpenList(QWidget *parent, const dcpp::tstring & aFile, const dcpp::UserPtr & aUser, int64_t aSpeed, const QString aTitle)
{
	// Function to open filelists
	tstring t = aFile;
	dcpp::UserPtr u = aUser;
		
	m_tabwin->setCurrentIndex( m_tabwin->addTab( (new FileListDlg(parent, u, aSpeed, t) ), aTitle ) );
}

void MainWindowImpl::OpenHub(QWidget *parent, const dcpp::tstring& adr)
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new HubWindow(m_tabwin, adr)),"Hub"));
}

void MainWindowImpl::OpenPM(QWidget *parent, const QString &name)
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new PMWindow(m_tabwin,name)),"PM"));
}

void MainWindowImpl::SearchFunc()
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new SearchWindow(m_tabwin,"")),"search"));
}

void MainWindowImpl::ADLFunc()
{
	if (FindWinByType(12)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new ADLSearchWindow(m_tabwin)),"ADL Search"));
	else m_tabwin->setCurrentIndex(FindWinByType(12));
}

void MainWindowImpl::SSFunc()
{
	if (FindWinByType(13)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new SearchSpyWindow(m_tabwin)),"Search Spy"));
	else m_tabwin->setCurrentIndex(FindWinByType(13));
}

void MainWindowImpl::ULFinFunc()
{
	if (FindWinByType(11)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new UploadFinishedWindow(m_tabwin)),"UL_Fin"));
	else m_tabwin->setCurrentIndex(FindWinByType(11));
}

void MainWindowImpl::DLFinFunc()
{
	if (FindWinByType(10)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new DownloadFinishedWindow(m_tabwin)),"DL_Fin"));
	else m_tabwin->setCurrentIndex(FindWinByType(10));
}

void MainWindowImpl::IgnoredUsrFunc()
{
	if (FindWinByType(8)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new IgnoredUsersWindow(m_tabwin)),"Ign_Usr"));
	else m_tabwin->setCurrentIndex(FindWinByType(8));
}

void MainWindowImpl::FavUsrFunc()
{
	if (FindWinByType(7)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new FavoriteUsersWindow(m_tabwin)),"Fav_Usr"));
	else m_tabwin->setCurrentIndex(FindWinByType(7));
}

void MainWindowImpl::FavHubListFunc()
{
	if (FindWinByType(4)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new FavoriteHubListWindow(m_tabwin)),"Fav_Hub"));
	else m_tabwin->setCurrentIndex(FindWinByType(4));
}

void MainWindowImpl::PubHubFunc()
{
	if (FindWinByType(9)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new PublicHubWindow(m_tabwin)),"Pub Hub"));
	else m_tabwin->setCurrentIndex(FindWinByType(9));
}

void MainWindowImpl::DLQueueFunc()
{
	if (FindWinByType(6)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new DownLoadQueueWindow(m_tabwin)),"DL Queue"));
	else m_tabwin->setCurrentIndex(FindWinByType(6));
}

void MainWindowImpl::notepadFunc()
{
	if (FindWinByType(14)==-1)
		m_tabwin->setCurrentIndex(m_tabwin->addTab((new NotePad(m_tabwin)),"NotePad"));
	else m_tabwin->setCurrentIndex(FindWinByType(14));
}

void MainWindowImpl::indexingFunc()
{
	HashDlg *hd = new HashDlg(this, false);
	hd->show();
}

void MainWindowImpl::qcdconFunc(QString adr)
{
	OpenHub(this, StilUtils::QtoTstr(adr));
}

void MainWindowImpl::fQuickConFunc()
{
	TQuickConnectDialog *qcd = new TQuickConnectDialog(this);
	connect(qcd,SIGNAL(con_pressed(QString)),this,SLOT(qcdconFunc(QString)));
}

void MainWindowImpl::statusbarcheck()
{
	if (actionStatusBar->isChecked()) statusbar->setVisible(true); else statusbar->setVisible(false);
}

void MainWindowImpl::setShareSize(const QString &sz)
{
//	shareStatusLbl->setText(sz);
}

void MainWindowImpl::updateWindowMenu()
{
	if (!m_tabwin) return;
	int tst[20]; // 20 for reserv - Count of type mdi_child. see "mdi_c.h" for details
	for (int i=0; i<20; i++) tst[i]=0;
	actionClose_All->setEnabled(m_tabwin->count()>0);
	actionClose_All_Hub_Windows->setEnabled(m_tabwin->count()>0);
	actionClose_All_Private_Chat_Windows->setEnabled(m_tabwin->count()>0);
	actionClose_All_Search_Windows->setEnabled(m_tabwin->count()>0);
	actionClose_All_FileList_Windows->setEnabled(m_tabwin->count()>0);
	actionClose_Disconnected_Hub_Windows->setEnabled(m_tabwin->count()>0);
	actionClose_FileLists_Offline_Users->setEnabled(m_tabwin->count()>0);
	actionClose_Private_Chat_with_Offline_Users->setEnabled(m_tabwin->count()>0);
	
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		tst[p->type]++;
	}
	
	actionClose_All_Hub_Windows->setEnabled(tst[1]!=0);
	actionClose_Disconnected_Hub_Windows->setEnabled(tst[1]!=0);
	
	actionClose_All_Private_Chat_Windows->setEnabled(tst[2]!=0);
	actionClose_Private_Chat_with_Offline_Users->setEnabled(tst[2]!=0);
	
	actionClose_All_Search_Windows->setEnabled(tst[3]!=0);
	
	actionClose_All_FileList_Windows->setEnabled(tst[5]!=0);
	actionClose_FileLists_Offline_Users->setEnabled(tst[5]!=0);
}

void MainWindowImpl::slotCloseOfflineLists()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==5)
		{
			FileListDlg *v = qobject_cast<FileListDlg *>(p);
			//if (!v->isConnected()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseOfflineChat()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==2)
		{
			PMWindow *v = qobject_cast<PMWindow *>(p);
			//if (!v->isOnline()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseDisconnectedHubs()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==1)
		{
			HubWindow *v = qobject_cast<HubWindow *>(p);
			if (!v->isConnected()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseWinType(int type)
{
	for (int j=0; j<5; j++)
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==type) delete m_tabwin->widget(i);
	}
}

void MainWindowImpl::slotCloseWinTypeHub()
{
	slotCloseWinType(1);
}

void MainWindowImpl::slotCloseWinTypePM()
{
	slotCloseWinType(2);
}

void MainWindowImpl::slotCloseWinTypeSearch()
{
	slotCloseWinType(3);
}

void MainWindowImpl::slotCloseWinTypeFL()
{
	slotCloseWinType(5);
}

const QString & ThreadGetTTH::getA()
{
	return a;
}

void ThreadGetTTH::setA(QString s)
{
	a=s;
}

const QString & ThreadGetTTH::getB()
{
	return b;
}

const QString & ThreadGetTTH::getC()
{
	return c;
}

void ThreadGetTTH::stop()
{
	 _stp = true;
}

void ThreadGetTTH::run()
{	
	_stp = false;
	b="";
	c="";
	if (!a.isEmpty()) {
		AutoArray<char> buf(512*1024);

		try {
			QFile f(a);
			QFileInfo fi(f);
			TigerTree tth(TigerTree::calcBlockSize(f.size(), 1));

			f.open(QIODevice::ReadOnly);
			if (fi.size() > 0) {
				qint64 n = 512*1024;
				while( (n = f.read(buf, n)) > 0) {
					if (_stp) break;
					tth.update(buf, n);
					n = 512*1024;
					if (_stp) break;
				}
			} else tth.update("", 0);
			tth.finalize();

			b = tr(tth.getRoot().toBase32().c_str());
			
			c = "magnet:?xt=urn:tree:tiger:" + b + "&xl=" + QString::number(fi.size()) + "&dn=" + (Util::encodeURI(fi.fileName().toLocal8Bit().data())).c_str();
			f.close();
			if (_stp) return;
		} catch(...) { }
	emit ready();
	}
	return ;
}

void MainWindowImpl::GetTTHFunc()
{	
	actionGet_TTH_for_file->setEnabled(false);
	QString selectedFilter="";
	QFileDialog::Options options;
	//options |= QFileDialog::DontUseNativeDialog;
	thrdGetTTh.setA( QFileDialog::getOpenFileName(this, tr("Select File"),"", tr("All Files (*)"), &selectedFilter, options) );
	
	thrdGetTTh.start(QThread::LowPriority);
}

void MainWindowImpl::show_tthFunc()
{
	new TthDialog(this,thrdGetTTh.getA(),thrdGetTTh.getB(),thrdGetTTh.getC());
	actionGet_TTH_for_file->setEnabled(true);
}

void MainWindowImpl::reconnectFunc()
{
	int i = m_tabwin->currentIndex();
	if ( (i>-1)&&(m_tabwin->count()>0) )
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==1)
		{
			HubWindow *v = qobject_cast<HubWindow *>(p);
			v->ReConnect();
		}
	}
}

void MainWindowImpl::openfilelistFunc()
{
	// Open Custom FileList
	
	QString selectedFilter="";
	QFileDialog::Options options;
	//options |= QFileDialog::DontUseNativeDialog;
	QString fn = QFileDialog::getOpenFileName(this, tr("Select File"),"", tr("File lists (*.xml.bz2);;All Files (*)"), &selectedFilter, options);
	if (!fn.isEmpty()) 
	{
		dcpp::tstring strFile(StilUtils::QtoTstr(fn));
		
		if (Text::fromT(strFile) == ShareManager::getInstance()->getOwnListFile()) openownfilelistFunc();
		
		dcpp::UserPtr u = dcpp::DirectoryListing::getUserFromFilename(dcpp::Text::fromT(strFile));
		
		if(u) 
		{
			const string nick = FileListDlg::getNickFromFilename(Text::fromT(strFile));
			QString NickName = StilUtils::TstrtoQ(Text::toT(nick));
			OpenList(this, strFile, u, 0, NickName);
		}
			
	}
}

void MainWindowImpl::openownfilelistFunc()
{
	const string ownFileList = dcpp::ShareManager::getInstance()->getOwnListFile();
	
	if(!ownFileList.empty())
		OpenList(this, dcpp::Text::toT(ownFileList), dcpp::ClientManager::getInstance()->getMe(), 0, StilUtils::TstrtoQ(Text::toT(SETTING(NICK))) );
}

void MainWindowImpl::RefreshOwnFileListFunc()
{
	// Refreshes (rehash) own filelist
	dcpp::ShareManager::getInstance()->setDirty();
	dcpp::ShareManager::getInstance()->refresh(true);
}

void MainWindowImpl::OpenDownloadsFolderFunc()
{
	QUrl url = QUrl(StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOAD_DIRECTORY)))).toLocalFile();
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::slotCurrentTabChanged(int index)
{
	// On Tab Changed
	setWindowTitle(tr(APPLICATIONNAME)+" - [" + m_tabwin->tabText(index) + "]");
	setToolTip(tr(APPLICATIONNAME)+" - [" + m_tabwin->tabToolTip(index) + "]");
}

void MainWindowImpl::autoConnect(const FavoriteHubEntryList& fl)
{	
	for (FavoriteHubEntryList::const_iterator i = fl.begin(); i != fl.end(); ++i)
	{
		FavoriteHubEntry* entry = *i;
		if (entry->getConnect())
		{
			if (!entry->getNick().empty() || !SETTING(NICK).empty())
			{
				//HubFrame::openWindow(getMDIParent(), entry->getServer());
				OpenHub(this, Text::toT(entry->getServer()));
			}
		}
	}
}

bool MainWindowImpl::speak(unsigned int w, long l)
{
	emit speakerSignal(w, l);
}

//
void MainWindowImpl::on(QueueManagerListener::Finished, QueueItem* qi, const string& dir, int64_t speed) throw()
{
	if (qi->isSet(QueueItem::FLAG_CLIENT_VIEW))
	{
		if (qi->isSet(QueueItem::FLAG_USER_LIST))
		{
			// This is a file listing, show it...
			DirectoryListInfo* i = new DirectoryListInfo(qi->getDownloads().at(0)->getUser(), Text::toT(qi->getListName()), Text::toT(dir), speed);

			speak(DOWNLOAD_LISTING, (long)i);
		}
		else if (qi->isSet(QueueItem::FLAG_TEXT))
		{
			speak(VIEW_FILE_AND_DELETE, reinterpret_cast<long>(new std::string(qi->getTarget())));
		}
	}
}

void MainWindowImpl::on(PartialList, const UserPtr& aUser, const string& text) throw()
{
	speak(BROWSE_LISTING, (long)new DirectoryBrowseInfo(aUser, text));
}

int MainWindowImpl::handleSpeaker(unsigned int wParam, long lParam)
{
	Speaker s = static_cast<Speaker>(wParam);

	switch (s) {
	case DOWNLOAD_LISTING: {
		boost::scoped_ptr<DirectoryListInfo> i(reinterpret_cast<DirectoryListInfo*>(lParam));
		//DirectoryListingFrame::openWindow(getMDIParent(), i->file, i->dir, i->user, i->speed);
		QString NickName = StilUtils::TstrtoQ(Text::toT((FileListDlg::getNickFromFilename(Text::fromT(i->file)))));
		OpenList(this, i->file, i->user, i->speed, NickName);
	}
		break;
	case BROWSE_LISTING: {/*
		boost::scoped_ptr<DirectoryBrowseInfo> i(reinterpret_cast<DirectoryBrowseInfo*>(lParam));
		//DirectoryListingFrame::openWindow(getMDIParent(), i->user, i->text, 0);
		OpenList(this, i->user, i->text, 0, QString(""));
	}*/
		break;
	case AUTO_CONNECT: {
		autoConnect(FavoriteManager::getInstance()->getFavoriteHubs());
	}
		break;
	case PARSE_COMMAND_LINE: {
		//parseCommandLine(GetCommandLine());
	}
		break;
	case VIEW_FILE_AND_DELETE: {/*
		boost::scoped_ptr<std::string> file(reinterpret_cast<std::string*>(lParam));
		new TextFrame(this->getMDIParent(), *file);
		File::deleteFile(*file);*/
	}
		break;
	case STATUS_MESSAGE: {
		boost::scoped_ptr<pair<time_t, tstring> > msg(reinterpret_cast<std::pair<time_t, tstring>*>(lParam));
		tstring line = Text::toT("[" + Util::getShortTimeString(msg->first) + "] ") + msg->second;

		setStatus(STATUS_STATUS, line);
		while (lastLinesList.size() + 1> MAX_CLIENT_LINES)
			lastLinesList.erase(lastLinesList.begin());
		if (line.find(_T('\r')) == tstring::npos) {
			lastLinesList.push_back(line);
		} else {
			lastLinesList.push_back(line.substr(0, line.find(_T('\r'))));
		}
	}
		break;
	case LAYOUT: {
		//layout();
	}
		break;
	}
	return 0;
}
}

void MainWindowImpl::setStatus(int s, const tstring& text)
{
	switch(s)
	{
		case STATUS_STATUS:
			//statusStatusLabel->setText(StilUtils::TstrtoQ(text));
			statusBar()->showMessage(StilUtils::TstrtoQ(text));
			break;
		case STATUS_AWAY:
			statusAwayLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_COUNTS:
			statusCountsLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_SLOTS:
			statusSlotsLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_DOWN_TOTAL:
			statusDownTotalLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_UP_TOTAL:
			statusUpTotalLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_DOWN_DIFF:
			statusDownDiffLabel->setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_UP_DIFF:
			statusUpDiffLabel->setText(StilUtils::TstrtoQ(text));
			break;
	}
}
