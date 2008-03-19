/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
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

#include "mainwindowimpl.h"

MainWindowImpl::~MainWindowImpl()
{
	thrdGetTTh.stop();
	trans_view->preClose();
	SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_POS_X, static_cast<int>(this->x()));
	SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_POS_Y, static_cast<int>(this->y()));
	SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_SIZE_X, static_cast<int>(this->size().width()));
	SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_SIZE_Y, static_cast<int>(this->size().height()));
	SettingsManager::getInstance()->set(SettingsManager::MAIN_WINDOW_STATE, (int)this->isMaximized());

	QueueManager::getInstance()->saveQueue();
	
	delete showhide;
	delete trayIcon;
	delete trayIconMenu;
	delete statusAwayLabel;
	delete statusCountsLabel;
	delete statusSlotsLabel;
	delete statusDownTotalLabel;
	delete statusUpTotalLabel;
	delete statusDownDiffLabel;
	delete statusUpDiffLabel;
	delete m_tabwin;
	thrdGetTTh.wait();
	
	if (c != NULL) 
	{
		c->removeListener(this);
		delete c;
		c = NULL;
	}
	
	LogManager::getInstance()->removeListener(this);
	QueueManager::getInstance()->removeListener(this);
	
	SearchManager::getInstance()->disconnect();
	ConnectionManager::getInstance()->disconnect();
}

void MainWindowImpl::initMain()
{
	setupUi(this);
	
	m_tabwin = new TabWidget( this );
	connect(m_tabwin, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
	setCentralWidget(m_tabwin);
	m_tabwin->setTabPosition((QTabWidget::TabPosition)APPSETTING(i_TABPOSIOTION));
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

	connect(this, SIGNAL(sigSpeak(StilUtils::Speaker, qint64)), this, SLOT(slotSpeak(StilUtils::Speaker, qint64)),Qt::QueuedConnection);
//	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));	
	updateWindowMenu();
	connect(menuWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
	
	setToolTip(tr(APPLICATIONNAME));
	setWindowTitle(tr(APPLICATIONNAME));

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
	
	statusBar()->addPermanentWidget(statusAwayLabel);
	statusBar()->addPermanentWidget(statusCountsLabel);
	statusBar()->addPermanentWidget(statusSlotsLabel);
	statusBar()->addPermanentWidget(statusDownTotalLabel);
	statusBar()->addPermanentWidget(statusUpTotalLabel);
	statusBar()->addPermanentWidget(statusDownDiffLabel);
	statusBar()->addPermanentWidget(statusUpDiffLabel);
	
}

void MainWindowImpl::timerEvent(QTimerEvent *)
{
	updateStatus();
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
	setStatus(STATUS_DOWN_DIFF, Text::toT("D: " + Util::formatBytes((downdiff*1000)/tdiff) + "/s (" + Util::toString(DownloadManager::getInstance()->getDownloadCount()) + ")"));
	setStatus(STATUS_UP_DIFF, Text::toT("U: " + Util::formatBytes((updiff*1000)/tdiff) + "/s (" + Util::toString(UploadManager::getInstance()->getUploadCount()) + ")"));
}

void MainWindowImpl::clientInit()
{
	updateStatus();
	
	QueueManager::getInstance()->addListener(this);
	LogManager::getInstance()->addListener(this);
	
	TimerManager::getInstance()->start();
	
	c = new HttpConnection;
	c->addListener(this);
	c->downloadFile("http://dcplusplus.sourceforge.net/version.xml");
	
	File::ensureDirectory(SETTING(LOG_DIRECTORY));
	startSocket();
	
	if(BOOLSETTING(OPEN_SYSTEM_LOG))			OpenSingleTab(StilUtils::WIN_TYPE_SYSTEM_LOG, true);
	if(BOOLSETTING(OPEN_FAVORITE_USERS))		OpenSingleTab(StilUtils::WIN_TYPE_FAVORITE_USERS, true);
	if(BOOLSETTING(OPEN_QUEUE))				OpenSingleTab(StilUtils::WIN_TYPE_DL_QUEUE, true);
	if(BOOLSETTING(OPEN_FINISHED_DOWNLOADS))	OpenSingleTab(StilUtils::WIN_TYPE_FINISHED_DL, true);
	if(BOOLSETTING(OPEN_WAITING_USERS))		OpenSingleTab(StilUtils::WIN_TYPE_WAITING_USERS, true);
	if(BOOLSETTING(OPEN_FINISHED_UPLOADS))		OpenSingleTab(StilUtils::WIN_TYPE_FINISHED_UL, true);
	if(BOOLSETTING(OPEN_SEARCH_SPY))			OpenSingleTab(StilUtils::WIN_TYPE_SEARCH_SPY, true);
	if(BOOLSETTING(OPEN_NETWORK_STATISTICS))	OpenSingleTab(StilUtils::WIN_TYPE_NETWORK_STATS, true);
	if(BOOLSETTING(OPEN_NOTEPAD))				OpenSingleTab(StilUtils::WIN_TYPE_NOTEPAD, true);
	if(BOOLSETTING(OPEN_PUBLIC))				OpenSingleTab(StilUtils::WIN_TYPE_PUBLIC_HUBS, true);
	if(BOOLSETTING(OPEN_FAVORITE_HUBS))		OpenSingleTab(StilUtils::WIN_TYPE_FAVORITE_HUB_LIST, true);
	
	// If First-time launch
	if(SETTING(NICK).empty()) 
	{ 
		// open settings dialog
		PreferencesFunc();
	}
	else
		speak(StilUtils::AUTO_CONNECT);	
}

void MainWindowImpl::startSocket() 
{
	SearchManager::getInstance()->disconnect();
	ConnectionManager::getInstance()->disconnect();

	if (ClientManager::getInstance()->isActive()) 
	{
		ConnectionManager::getInstance()->listen();
		SearchManager::getInstance()->listen();
	}
}

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	initMain();
	startTimer(1000);
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
			if (BOOLSETTING(CONFIRM_EXIT))
			{
				QMessageBox::StandardButton reply;
				reply = QMessageBox::question(this, tr("StilDC++"), tr("Do you realy want to exit?"), QMessageBox::Yes | QMessageBox::No);
				if (reply == QMessageBox::Yes)
				{
					emit signalForceCloseHashDialog();
					event->accept();
				}
				else event->ignore();
			} else
			{
				emit signalForceCloseHashDialog();
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
	connect(actionSysLog, SIGNAL(triggered()), this, SLOT(SLogFunc()));
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(PreferencesFunc()));
	connect(actionReconnect, SIGNAL(triggered()), this, SLOT(reconnectFunc()));
	connect(actionDonate, SIGNAL(triggered()), this, SLOT(DonateFunc()));
	connect(actionHomepage, SIGNAL(triggered()), this, SLOT(HomepageFunc()));
	connect(actionSearch, SIGNAL(triggered()), this, SLOT(OpenSearch()));
	connect(actionGet_TTH_for_file, SIGNAL(triggered()), this, SLOT(GetTTHFunc()));
	connect(actionFavorite_Hubs, SIGNAL(triggered()), this, SLOT(FavHubListFunc()));
	connect(actionPublic_Hubs, SIGNAL(triggered()), this, SLOT(PubHubFunc()));
	connect(actionQuick_Connect, SIGNAL(triggered()), this, SLOT(fQuickConFunc()));
	connect(actionDownload_Queue, SIGNAL(triggered()), this, SLOT(DLQueueFunc()));
	connect(actionFavorite_Users, SIGNAL(triggered()), this, SLOT(FavUsrFunc()));
	connect(actionIgnored_Users, SIGNAL(triggered()), this, SLOT(IgnoredUsrFunc()));
	connect(actionFinished_downloads, SIGNAL(triggered()), this, SLOT(DLFinFunc()));
	connect(actionFinished_Uploads, SIGNAL(triggered()), this, SLOT(ULFinFunc()));
	connect(actionWaiting, SIGNAL(triggered()), this, SLOT(WaitingFunc()));
	connect(actionNotePad, SIGNAL(triggered()), this, SLOT(notepadFunc()));
	connect(actionNetStats, SIGNAL(triggered()), this, SLOT(StatsFunc()));
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
	toolBar->addAction(actionWaiting);
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

int MainWindowImpl::FindWinByType(StilUtils::tabWinTypes type)
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
	StilUtils::openLink(APPDONATEPAGE);
}

void MainWindowImpl::HomepageFunc()
{
	StilUtils::openLink(APPHOMEPAGE);
}

void MainWindowImpl::ShowHashDlg(bool autoClose)
{
	HashDlg * h = new HashDlg(this);
	connect(MainWindowImpl::getInstance(), SIGNAL(signalForceCloseHashDialog()), h, SLOT(close()));
	h->sshow(autoClose);
}

void MainWindowImpl::indexingFunc()
{
	ShowHashDlg(false);
}

void MainWindowImpl::qcdconFunc(QString adr)
{
	OpenHub(StilUtils::QtoTstr(adr));
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

void MainWindowImpl::updateWindowMenu()
{
	if (!m_tabwin) return;
	int tst[StilUtils::WIN_TYPE_LAST];
	for (int i=0; i<StilUtils::WIN_TYPE_LAST; i++) tst[i]=0;
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
	
	actionClose_All_Hub_Windows->setEnabled(tst[StilUtils::WIN_TYPE_HUB]!=0);
	actionClose_Disconnected_Hub_Windows->setEnabled(tst[StilUtils::WIN_TYPE_HUB]!=0);
	
	actionClose_All_Private_Chat_Windows->setEnabled(tst[StilUtils::WIN_TYPE_PRIVATE_CHAT]!=0);
	actionClose_Private_Chat_with_Offline_Users->setEnabled(tst[StilUtils::WIN_TYPE_PRIVATE_CHAT]!=0);
	
	actionClose_All_Search_Windows->setEnabled(tst[StilUtils::WIN_TYPE_SEARCH]!=0);
	
	actionClose_All_FileList_Windows->setEnabled(tst[StilUtils::WIN_TYPE_FILELIST]!=0);
	actionClose_FileLists_Offline_Users->setEnabled(tst[StilUtils::WIN_TYPE_FILELIST]!=0);
}

void MainWindowImpl::slotCloseOfflineLists()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==StilUtils::WIN_TYPE_FILELIST)
		{
			FileListDlg *v = qobject_cast<FileListDlg *>(p);
			if (!v->isConnected()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseOfflineChat()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==StilUtils::WIN_TYPE_PRIVATE_CHAT)
		{
			PMWindow *v = qobject_cast<PMWindow *>(p);
			if (!v->isOnline()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseDisconnectedHubs()
{
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==StilUtils::WIN_TYPE_HUB)
		{
			HubWindow *v = qobject_cast<HubWindow *>(p);
			if (!v->isConnected()) delete m_tabwin->widget(i);
		}
	}
}

void MainWindowImpl::slotCloseWinType(StilUtils::tabWinTypes type)
{
	for (int j=0; j<5; j++) // @BUG : WHY THIS LINE NESSESARY???
	for (int i=0; i<m_tabwin->count(); i++)
	{
		MdiChild *p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type==type) delete m_tabwin->widget(i);
	}
}

void ThreadGetTTH::run()
{	
	_stp = false;
	b="";
	c="";
	if (!a.isEmpty()) {
//		AutoArray<char> buf(512*1024);
		boost::scoped_array<char> buf(new char[512*1024]);

		try {
			QFile f(a);
			QFileInfo fi(f);
			TigerTree tth(TigerTree::calcBlockSize(f.size(), 1));

			f.open(QIODevice::ReadOnly);
			if (fi.size() > 0) {
				qint64 n = 512*1024;
				while( (n = f.read(&buf[0], n)) > 0) {
					if (_stp) break;
					tth.update(&buf[0], n);
					n = 512*1024;
					if (_stp) break;
				}
			} else tth.update("", 0);
			tth.finalize();

			b = QString(tth.getRoot().toBase32().c_str());
			
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
	QWidget * w = m_tabwin->currentWidget();
	if ( w )
	{
		MdiChild *p = qobject_cast<MdiChild *>(w);
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
		tstring strFile(StilUtils::QtoTstr(fn));
		
		if (Text::fromT(strFile) == ShareManager::getInstance()->getOwnListFile()) openownfilelistFunc();
		
		UserPtr u = DirectoryListing::getUserFromFilename(Text::fromT(strFile));
		
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
	const string ownFileList = ShareManager::getInstance()->getOwnListFile();
	
	if(!ownFileList.empty())
		OpenList(this, Text::toT(ownFileList), ClientManager::getInstance()->getMe(), 0, StilUtils::TstrtoQ(Text::toT(SETTING(NICK))) );
}

void MainWindowImpl::RefreshOwnFileListFunc()
{
	// Refreshes (rehash) own filelist
	ShareManager::getInstance()->setDirty();
	ShareManager::getInstance()->refresh(true);
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
				OpenHub(Text::toT(entry->getServer()));
			}
		}
	}
}

void MainWindowImpl::slotSpeak(StilUtils::Speaker type, qint64 lParam)
{
	switch (type)
	{
		case StilUtils::DOWNLOAD_LISTING: 
		{
			boost::scoped_ptr<DirectoryListInfo> i(reinterpret_cast<DirectoryListInfo*>(lParam));
			//DirectoryListingFrame::openWindow(getMDIParent(), i->file, i->dir, i->user, i->speed);
			QString NickName = StilUtils::TstrtoQ(Text::toT((FileListDlg::getNickFromFilename(Text::fromT(i->file)))));
			OpenList(this, i->file, i->user, i->speed, NickName);
		}
			break;
		case StilUtils::BROWSE_LISTING: 
		{/*
			boost::scoped_ptr<DirectoryBrowseInfo> i(reinterpret_cast<DirectoryBrowseInfo*>(lParam));
			//DirectoryListingFrame::openWindow(getMDIParent(), i->user, i->text, 0);
			OpenList(this, i->user, i->text, 0, QString(""));
		*/}
			break;
		case StilUtils::AUTO_CONNECT:
		{
			autoConnect(FavoriteManager::getInstance()->getFavoriteHubs());
		}
			break;
		case StilUtils::PARSE_COMMAND_LINE:
		{
			//parseCommandLine(GetCommandLine());
		}
			break;
		case StilUtils::VIEW_FILE_AND_DELETE:
		{
			boost::scoped_ptr<std::string> file(reinterpret_cast<std::string*>(lParam));
			openTextWindow(*file);
			File::deleteFile(*file);
		}
			break;
		case StilUtils::STATUS_MESSAGE:
		{
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
		case StilUtils::LAYOUT: 
		{
				//layout();
		}
			break;
		default:
			break;
	}
}

void MainWindowImpl::setStatus(int s, const tstring& text)
{
	switch(s)
	{
		case STATUS_STATUS:
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

// Listeners

void MainWindowImpl::on(HttpConnectionListener::Data, HttpConnection* /*conn*/, const uint8_t* buf, size_t len) throw()
{
	versionInfo += string((const char*)buf, len);
}

void MainWindowImpl::on(HttpConnectionListener::Complete, HttpConnection* /*aConn*/, const string&) throw()
{
	// Cheking update: downloading xml-file from internet and parse it;
	
	/*
	qDebug() << "HTTP";
	QFile xmlfile(QString::fromStdString(versionInfo));
	xmlfile.open(QIODevice::ReadOnly);
	qDebug() << xmlfile.fileName();
	QByteArray data = xmlfile.readAll();
	xmlfile.close();
	QDomDocument xml;
	xml.setContent(data);
	qDebug() << xml.toByteArray();*/
}

void MainWindowImpl::on(QueueManagerListener::Finished, QueueItem* qi, const string& dir, int64_t speed) throw()
{
	if (qi->isSet(QueueItem::FLAG_CLIENT_VIEW))
	{
		if (qi->isSet(QueueItem::FLAG_USER_LIST))
		{
			// This is a file listing, show it...
			DirectoryListInfo* i = new DirectoryListInfo(qi->getDownloads().at(0)->getUser(), Text::toT(qi->getListName()), Text::toT(dir), speed);

			speak(StilUtils::DOWNLOAD_LISTING, (qint64)i);
		}
		else if (qi->isSet(QueueItem::FLAG_TEXT))
		{
			speak(StilUtils::VIEW_FILE_AND_DELETE, reinterpret_cast<qint64>(new std::string(qi->getTarget())));
		}
	}
}

void MainWindowImpl::on(PartialList, const UserPtr& aUser, const string& text) throw()
{
	speak(StilUtils::BROWSE_LISTING, (qint64)new DirectoryBrowseInfo(aUser, text));
}

void MainWindowImpl::OpenSingleTab(StilUtils::tabWinTypes type, bool silent)
{
	int z = FindWinByType(type);
	switch (type)
	{
		case StilUtils::WIN_TYPE_FINISHED_DL:
		{
			if (z == -1)
			{
				DownloadFinishedWindow *p = new DownloadFinishedWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Finished Downloads"));
				p->setTabIcon(actionFinished_downloads->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_FINISHED_UL:
		{
			if (z == -1)
			{
				UploadFinishedWindow *p = new UploadFinishedWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Finished Uploads"));
				p->setTabIcon(actionFinished_Uploads->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_SYSTEM_LOG:
		{
			if (z == -1)
			{
				SysLogWindow *p = new SysLogWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("System Log"));
				p->setTabIcon(actionSysLog->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_WAITING_USERS:
		{
			if (z == -1)
			{
				WaitingUsersWindow *p = new WaitingUsersWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Waiting Users"));
				p->setTabIcon(actionWaiting->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_NETWORK_STATS:
		{
			if (z == -1)
			{
				StatsWindow *p = new StatsWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Network Statistics"));
				p->setTabIcon(actionNetStats->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_ADL_SEARCH:
		{
			if (z == -1)
			{
				ADLSearchWindow *p = new ADLSearchWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Automatic Directory Listing Search"));
				p->setTabIcon(actionADL_Search->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_SEARCH_SPY:
		{
			if (z == -1)
			{
				SearchSpyWindow *p = new SearchSpyWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Search Spy"));
				p->setTabIcon(actionSearch_Spy->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_IGNORED_USERS:
		{
			if (z == -1)
			{
				IgnoredUsersWindow *p = new IgnoredUsersWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Ign_Usr"));
				p->setTabIcon(actionIgnored_Users->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_FAVORITE_USERS:
		{
			if (z == -1)
			{
				FavoriteUsersWindow *p = new FavoriteUsersWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Favorite Users"));
				p->setTabIcon(actionFavorite_Users->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_FAVORITE_HUB_LIST:
		{
			if (z == -1)
			{
				FavoriteHubListWindow *p = new FavoriteHubListWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("Favorite Hubs"));
				p->setTabIcon(actionFavorite_Hubs->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_PUBLIC_HUBS:
		{
			if (z == -1)
			{
				PublicHubWindow *p = new PublicHubWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(StilUtils::TstrtoQ(T_("Public Hubs")));
				p->setTabIcon(actionPublic_Hubs->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_DL_QUEUE:
		{
			if (z == -1)
			{
				DownLoadQueueWindow *p = new DownLoadQueueWindow(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("DL_Queue"));
				p->setTabIcon(actionDownload_Queue->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		case StilUtils::WIN_TYPE_NOTEPAD:
		{
			if (z == -1)
			{
				NotePad *p = new NotePad(m_tabwin);
				if (silent)
					m_tabwin->addTab(p);
				else
					m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
				p->setTabText(tr("NotePad"));
				p->setTabIcon(actionNotePad->icon());
			}
			else m_tabwin->setCurrentIndex(z);
		} break;
		
		
		// garbage not used! 'cause it isn't single tabs
		case StilUtils::WIN_TYPE_NONE: break;
		case StilUtils::WIN_TYPE_HUB: break;
		case StilUtils::WIN_TYPE_PRIVATE_CHAT: break;
		case StilUtils::WIN_TYPE_SEARCH: break;
		case StilUtils::WIN_TYPE_FILELIST: break;
		case StilUtils::WIN_TYPE_TEXT_WINDOW: break;
		case StilUtils::WIN_TYPE_LAST: break;
	}
}

void MainWindowImpl::OpenList(QWidget *parent, const tstring & aFile, const UserPtr & aUser, int64_t aSpeed, const QString aTitle, bool silent)
{
	// Function to open filelists
	tstring t = aFile;
	UserPtr u = aUser;
	
	FileListDlg *p = new FileListDlg(parent, u, aSpeed, t);
	
	if (silent)
		m_tabwin->addTab( p, QIcon(":/images/file_list.png"), aTitle );
	else
		m_tabwin->setCurrentIndex( m_tabwin->addTab( p, QIcon(":/images/file_list.png"), aTitle ) );
}

void MainWindowImpl::OpenHub(const tstring& adr, bool silent)
{
	HubWindow *p = new HubWindow(m_tabwin, adr);
	if (silent)
		{
			m_tabwin->addTab(p);
		}
	else
		{
			m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
		}
		
	p->setTabText(tr("Hub: ")+StilUtils::TstrtoQ(adr));
	p->setTabIcon(QIcon(":/images/hub.png"));
}

PMWindow * MainWindowImpl::OpenPM(const UserPtr& replyTo, bool silent)
{
	PMWindow *p = new PMWindow(m_tabwin, replyTo);
	if (silent)
		{
			m_tabwin->addTab(p);
		}
	else
		{
			m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
		}
	p->setTabText(tr("Private Chat with: ")+StilUtils::TstrtoQ(StilUtils::getNicks(replyTo)));
	p->setTabIcon(QIcon(":/images/private_chat.png"));
	return p;
}

void MainWindowImpl::OpenSearch(const tstring& str, int64_t size, SearchManager::SizeModes mode, SearchManager::TypeModes type, bool silent)
{
	SearchWindow *p = new SearchWindow(m_tabwin, str, size, mode, type);
	if (silent)
		m_tabwin->addTab(p);
	else
		m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
	p->setTabText(tr("Search for: ")+StilUtils::TstrtoQ(str));
	p->setTabIcon(actionSearch->icon());
}

void MainWindowImpl::openTextWindow(const QString& fileName, bool silent)
{
	TextWindow *p = new TextWindow(m_tabwin, fileName);
	if (silent)
		m_tabwin->addTab(p);
	else
		m_tabwin->setCurrentIndex(m_tabwin->addTab(p));
	p->setTabText(tr("View As Text : ") + fileName);
	p->setTabIcon(QIcon(":/images/view_as_text.png"));
}

void MainWindowImpl::openTextWindow(const tstring& fileName, bool silent)
{
	openTextWindow(StilUtils::TstrtoQ(fileName), silent);
}

void MainWindowImpl::openTextWindow(const string& fileName, bool silent)
{
	openTextWindow(Text::toT(fileName), silent);
}

// PMs
int MainWindowImpl::isPM_Open(const UserPtr& u)
{
	for (int i=0; i < m_tabwin->count(); i++)
	{
		MdiChild * p = qobject_cast<MdiChild *>(m_tabwin->widget(i));
		if (p->type == StilUtils::WIN_TYPE_PRIVATE_CHAT) 
			{
				PMWindow * w = qobject_cast<PMWindow*>(p);
				if (w->getUser() == u) return i;
			}
	}
	return -1;
}

void MainWindowImpl::gotPrivateMessage(const UserPtr& /*from*/, const UserPtr& to, const UserPtr& replyTo, const tstring& aMessage)
{
	const UserPtr& user = (replyTo == ClientManager::getInstance()->getMe()) ? to : replyTo;
	int i = isPM_Open(user);
	
	PMWindow * p = NULL;
	
	if(i == -1) 
	{
		p = OpenPM(user, BOOLSETTING(POPUNDER_PM));
		p->addChat(aMessage);
		if(Util::getAway()) 
			{
				if(!(BOOLSETTING(NO_AWAYMSG_TO_BOTS) && user->isSet(User::BOT)))
					p->sendMessage(Text::toT(Util::getAwayMessage()));
			}
		
		if(BOOLSETTING(PRIVATE_MESSAGE_BEEP) || BOOLSETTING(PRIVATE_MESSAGE_BEEP_OPEN)) {
			if (SETTING(BEEPFILE).empty())
				qApp->beep();
			//else
			//	::PlaySound(Text::toT(SETTING(BEEPFILE)).c_str(), NULL, SND_FILENAME | SND_ASYNC);
		}
	} 
	else 
		{
			p = qobject_cast<PMWindow *>(m_tabwin->widget(i));
			if(BOOLSETTING(PRIVATE_MESSAGE_BEEP)) 
			{
				if (SETTING(BEEPFILE).empty())
					qApp->beep();
			//	else
			//		::PlaySound(Text::toT(SETTING(BEEPFILE)).c_str(), NULL, SND_FILENAME | SND_ASYNC);
			}
			p->addChat(aMessage);
		}
}
