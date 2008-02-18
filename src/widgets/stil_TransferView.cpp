/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
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

#include "stil_TransferView.h"
#include "myprogress.h"

int TransferView::connectionIndexes[] = { CONNECTION_COLUMN_USER, CONNECTION_COLUMN_STATUS, CONNECTION_COLUMN_SPEED, CONNECTION_COLUMN_CHUNK, CONNECTION_COLUMN_TRANSFERED, CONNECTION_COLUMN_QUEUED, CONNECTION_COLUMN_CIPHER, CONNECTION_COLUMN_IP };
int TransferView::connectionSizes[] = { 125, 375, 100, 125, 125, 75, 100, 100 };

int TransferView::downloadIndexes[] = { DOWNLOAD_COLUMN_FILE, DOWNLOAD_COLUMN_PATH, DOWNLOAD_COLUMN_STATUS, DOWNLOAD_COLUMN_TIMELEFT, DOWNLOAD_COLUMN_SPEED, DOWNLOAD_COLUMN_DONE, DOWNLOAD_COLUMN_SIZE };
int TransferView::downloadSizes[] = { 200, 300, 150, 200, 125, 100, 100 };

static const char* connectionNames[] = {
	N_("User"),
	N_("Status"),
	N_("Speed"),
	N_("Chunk"),
	N_("Transferred (Ratio)"),
	N_("Queued"),
	N_("Cipher"),
	N_("IP")
};

static const char* downloadNames[] = {
	N_("Filename"),
	N_("Path"),
	N_("Status"),
	N_("Time left"),
	N_("Speed"),
	N_("Done"),
	N_("Size")
};

void TransferView::handleDblClicked(const QModelIndex & mi)
{
	// case of "i_ACT_ON_DBLCLICK_TRANSFERVIEW"
	// 0 - Private Message
	// 1 - Public Message =--------------= NOT IMPLEMENTED YET
	// 2 - Get FileList
	// 3 - Copy Nick
	// 4 - Force Atempt
	// 5 - Close Connection
	// 6 - Run UserCommand =--------------= NOT IMPLEMENTED YET
	// 7 - Search For alternates
	// 8 - Add User To Favorites
	
		int ua = APPSETTING(i_ACT_ON_DBLCLICK_TRANSFERVIEW);
}

void TransferView::preClose()
{
	// CONNECTIONS
	// visibility
	QStringList vv;
	for (int i=0; i<CONNECTION_COLUMN_LAST; i++) 
		vv << QString::number(connections->header()->isSectionHidden(i));
	SETAPPSTRING(s_TRANSVIEW_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i=0; i<CONNECTION_COLUMN_LAST; i++) 
		w << QString::number(connections->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::CONNECTIONS_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i=0; i<CONNECTION_COLUMN_LAST; i++) 
		ww << QString::number(connections->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::CONNECTIONS_ORDER, ww.join(",").toStdString());
	
	// DOWNLOADS
	// visibility
	vv.clear();
	for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) vv << QString::number(downloads->header()->isSectionHidden(i));
	SETAPPSTRING(s_DLWINDOW_COLUMN_VISIBILITY, vv.join(","));
	// width
	w.clear();
	for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) w << QString::number(downloads->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::DOWNLOADS_WIDTHS, w.join(",").toStdString());
	// order
	ww.clear();
	for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) ww << QString::number(downloads->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::DOWNLOADS_ORDER, ww.join(",").toStdString());
	
	// REMOVING LISTENERS
	QueueManager::getInstance()->removeListener(this);
	ConnectionManager::getInstance()->removeListener(this);
	DownloadManager::getInstance()->removeListener(this);
	UploadManager::getInstance()->removeListener(this);
}

TransferView::TransferView(QWidget *parent) : QWidget(parent)
{
	connect(this, SIGNAL(sigSpeak()), this, SLOT(slotSpeak()), Qt::QueuedConnection);
	
	tabs = new TabWidget( this );

	//connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
	tabs->setOpt(0);
	
	connections = new QDLTreeWidget(tabs);
	downloads = new QDLTreeWidget(tabs);
	
	tabs->addTab( connections, tr("Connections") );
	tabs->addTab( downloads, tr("Downloads") );
	
	// CONNECTIONS
	// labels
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(connectionNames))
		columns << StilUtils::TstrtoQ(name);
	connections->setHeaderLabels(columns);
	// width
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(CONNECTIONS_WIDTHS))).split(",");
	if (clist.size() != CONNECTION_COLUMN_LAST)
		for (int i=0; i<CONNECTION_COLUMN_LAST; i++) connections->setColumnWidth(i, connectionSizes[i]);
	else 
		for (int i=0; i<CONNECTION_COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) connections->setColumnWidth(i, connectionSizes[i]);
				else connections->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	QStringList vlist = APPSTRING(s_TRANSVIEW_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == CONNECTION_COLUMN_LAST)
		for (int i=0; i<CONNECTION_COLUMN_LAST; i++) connections->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(CONNECTIONS_ORDER))).split(",");
	if (olist.size() == CONNECTION_COLUMN_LAST)
		for (int j=0; j<CONNECTION_COLUMN_LAST; j++) connections->header()->swapSections(connections->header()->visualIndex(olist[j].toInt()), j);
	
	// DOWNLOADS
	// labels
	columns.clear();
	foreach(tstring name, StilUtils::getStrings(downloadNames))
		columns << StilUtils::TstrtoQ(name);
	downloads->setHeaderLabels(columns);
	// width
	clist.clear(); 
	clist = StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOADS_WIDTHS))).split(",");
	if (clist.size() != DOWNLOAD_COLUMN_LAST)
		for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) downloads->setColumnWidth(i, downloadSizes[i]);
	else 
		for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) downloads->setColumnWidth(i, downloadSizes[i]);
				else downloads->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	vlist.clear();
	vlist = APPSTRING(s_DLWINDOW_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == DOWNLOAD_COLUMN_LAST)
		for (int i=0; i<DOWNLOAD_COLUMN_LAST; i++) downloads->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	olist.clear();
	olist = StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOADS_ORDER))).split(",");
	if (olist.size() == DOWNLOAD_COLUMN_LAST)
		for (int j=0; j<DOWNLOAD_COLUMN_LAST; j++) 
			downloads->header()->swapSections(downloads->header()->visualIndex(olist[j].toInt()), j);
	
	
	// COLUMN (HEADER) MENU
	//columnMenu = new QMenu();
	//connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	//header()->setContextMenuPolicy(Qt::CustomContextMenu);
	//connect(header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	// CONTEXT MENU
	//cnxtMenu = new QMenu();
	//setContextMenuPolicy(Qt::CustomContextMenu);
	//connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	//connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(handleDblClicked(QModelIndex)) );
	
	// ADDING LISTENERS
	ConnectionManager::getInstance()->addListener(this);
	DownloadManager::getInstance()->addListener(this);
	UploadManager::getInstance()->addListener(this);
	QueueManager::getInstance()->addListener(this);
}

void TransferView::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

//	if(index < 0) hideColumn(-index-1);
//	else showColumn(index);
}

void TransferView::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
/*	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(header()->mapToGlobal(point));
*/
}

void TransferView::makeContextMenu() 
{
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Force attempt")) ,this ,SLOT(handleForce()) );
	cnxtMenu->addSeparator();
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Add To Favorites")) ,this ,SLOT(handleAddToFav()) );
	cnxtMenu->addSeparator();
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Search for alternates")) ,this ,SLOT(handleSearchAlternates()) );
	cnxtMenu->addSeparator();
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Copy nick to clipboard")) ,this ,SLOT(handleCopyNick()) );
	cnxtMenu->addSeparator();
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Get file list")) ,this ,SLOT(handleGetFL()) );
	cnxtMenu->addSeparator();
	cnxtMenu->addAction(StilUtils::TstrtoQ(T_("Close connection")) ,this ,SLOT(handleRemove()) );
}

void TransferView::showCnxtMenu(const QPoint& point)
{
	cnxtMenu->clear();
	// NEED TO GENERATE MENU FORM ITEMINFO CLASS
	makeContextMenu();
	// SHOW MENU
	//if (indexAt(point).isValid()) cnxtMenu->exec(mapToGlobal(point));
}

TransferView::~TransferView() 
{
	//FIXME: segfaults here
	//delete cnxtMenu;
	//delete columnMenu;
	delete connections;
	delete downloads;
	delete tabs;
}

void TransferView::handleGetFL()
{
	
}

void TransferView::handleRemove() 
{
	
}

void TransferView::runUserCommand(const UserCommand& uc) 
{
	
}

void TransferView::handleForce()
{
	
}

void TransferView::handleCopyNick()
{
	
}

void TransferView::handleAddToFav()
{

}

void TransferView::handleSearchAlternates()
{

}

void TransferView::keyPressEvent(QKeyEvent *e)
{
	QWidget::keyPressEvent(e);
	if ( e->key() == Qt::Key_Delete) handleRemove();
}

void TransferView::slotSpeak()
{
	TaskQueue::List t;
	tasks.get(t);
	
	setUpdatesEnabled(false);
	
	for(TaskQueue::Iter i = t.begin(); i != t.end(); ++i) {
		if(i->first == CONNECTIONS_ADD) {
			boost::scoped_ptr<UpdateInfo> ui(static_cast<UpdateInfo*>(i->second));
			ConnectionInfo* ii = new ConnectionInfo(ui->user, ui->download);
			ii->update(*ui);
//			connections->insert(ii);
		} else if(i->first == CONNECTIONS_REMOVE) 
		{
			auto_ptr<UpdateInfo> ui(static_cast<UpdateInfo*>(i->second));
//			int ic = connections->size();
//			for(int i = 0; i < ic; ++i) 
//			{
//				ConnectionInfo* ii = connections->getData(i);
//				if(*ui == *ii) 
//				{
//					connections->erase(i);
//					break;
//				}
//			}
		} else if(i->first == CONNECTIONS_UPDATE) {
			boost::scoped_ptr<UpdateInfo> ui(static_cast<UpdateInfo*>(i->second));
//			int ic = connections->size();
//			for(int i = 0; i < ic; ++i) 
			{
//				ConnectionInfo* ii = connections->getData(i);
//				if(ii->download == ui->download && ii->user == ui->user) 
//					{
//					ii->update(*ui);
//					connections->update(i);
//					break;
//					}
			}
		} else if(i->first == DOWNLOADS_ADD_USER) {
			boost::scoped_ptr<TickInfo> ti(static_cast<TickInfo*>(i->second));
			int i = find(ti->path);
			if(i == -1) {
				int64_t size = QueueManager::getInstance()->getSize(ti->path);
				TTHValue tth;
				if(size != -1 && QueueManager::getInstance()->getTTH(ti->path, tth)) {
//					i = downloads->insert(new DownloadInfo(ti->path, size, tth));
				}
			} else {
//				downloads->getData(i)->users++;
//				downloads->update(i);
			}
		} else if(i->first == DOWNLOADS_TICK) {
			boost::scoped_ptr<TickInfo> ti(static_cast<TickInfo*>(i->second));
			int i = find(ti->path);
			if(i != -1) {
//				DownloadInfo* di = downloads->getData(i);
//				di->update(*ti);
//				downloads->update(i);
			}
		} else if(i->first == DOWNLOADS_REMOVE_USER) 
			{
			boost::scoped_ptr<TickInfo> ti(static_cast<TickInfo*>(i->second));
			int i = find(ti->path);
			
			if(i != -1)
				{
//				DownloadInfo* di = downloads->getData(i);
//				if(--di->users == 0) 
//				{
//					di->bps = 0;
//				}
//				di->update();
//				downloads->update(i);
				}
			} 
		else if(i->first == DOWNLOADS_REMOVED) 
		{
			boost::scoped_ptr<TickInfo> ti(static_cast<TickInfo*>(i->second));
			int i = find(ti->path);
			if(i != -1) {
				//downloads->erase(i);
			}
		}
	
	}

//	if(!t.empty()) connections->resort();
	setUpdatesEnabled(true);
}

int TransferView::find(const string& path)
{
/*	for(size_t i = 0; i < downloads->size(); ++i) {
		DownloadInfo* di = downloads->getData(i);
		if(Util::stricmp(di->path, path) == 0) {
			return i;
		}
	}
*/	return -1;
}

void TransferView::ConnectionInfo::disconnect() 
{
	ConnectionManager::getInstance()->disconnect(user, download);
}

TransferView::ConnectionInfo::ConnectionInfo(const UserPtr& u, bool aDownload) : 
	UserInfoBase(u), 
	download(aDownload), 
	transferFailed(false),
	status(STATUS_WAITING), 
	actual(0), 
	lastActual(0),
	transfered(0),
	lastTransfered(0),
	queued(0),
	speed(0)	
{
	columns[CONNECTION_COLUMN_USER] = StilUtils::getNicks(u);
	columns[CONNECTION_COLUMN_STATUS] = T_("Idle");
	columns[CONNECTION_COLUMN_TRANSFERED] = Text::toT(Util::toString(0));
	if(aDownload) {
		queued = QueueManager::getInstance()->getQueued(u);
		columns[CONNECTION_COLUMN_QUEUED] = Text::toT(Util::formatBytes(queued));
	}
}

void TransferView::ConnectionInfo::update(const UpdateInfo& ui) {
	if(ui.updateMask & UpdateInfo::MASK_STATUS) {
		lastTransfered = lastActual = 0;
		status = ui.status;
		if(download) {
			// Also update queued when status changes...
			queued = QueueManager::getInstance()->getQueued(user);
			columns[CONNECTION_COLUMN_QUEUED] = Text::toT(Util::formatBytes(queued));
		}
	}

	if(ui.updateMask & UpdateInfo::MASK_STATUS_STRING) {
		// No slots etc from transfermanager better than disconnected from connectionmanager
		if(!transferFailed)
			columns[CONNECTION_COLUMN_STATUS] = ui.statusString;
		transferFailed = ui.transferFailed;
	}
	
	if(ui.updateMask & UpdateInfo::MASK_TRANSFERED) {
		actual += ui.actual - lastActual;
		lastActual = ui.actual;
		transfered += ui.transfered - lastTransfered;
		lastTransfered = ui.transfered;
		if(actual == transfered) {
			columns[CONNECTION_COLUMN_TRANSFERED] = Text::toT(Util::formatBytes(transfered));
		} else {
			columns[CONNECTION_COLUMN_TRANSFERED] = str(TF_("%1% (%2$0.2f)") 
				% Text::toT(Util::formatBytes(transfered))
				% (static_cast<double>(actual) / transfered));
		}
	}
	
	if(ui.updateMask & UpdateInfo::MASK_CHUNK) {
		chunkPos = ui.chunkPos;
		chunk = ui.chunk;
		columns[CONNECTION_COLUMN_CHUNK] = Text::toT(Util::formatBytes(chunkPos) + "/" + Util::formatBytes(chunk));
	}
	
	if(ui.updateMask & UpdateInfo::MASK_SPEED) {
		speed = ui.speed;
		if (status == STATUS_RUNNING) {
			columns[CONNECTION_COLUMN_SPEED] = str(TF_("%1%/s") % Text::toT(Util::formatBytes(speed)));
		} else {
			columns[CONNECTION_COLUMN_SPEED] = Util::emptyStringT;
		}
	}
	
	if(ui.updateMask & UpdateInfo::MASK_IP) {
		columns[CONNECTION_COLUMN_IP] = ui.ip;
	}
	
	if(ui.updateMask & UpdateInfo::MASK_CIPHER) {
		columns[CONNECTION_COLUMN_CIPHER] = ui.cipher;
	}
}

int TransferView::ConnectionInfo::compareItems(ConnectionInfo* a, ConnectionInfo* b, int col) {
	if(BOOLSETTING(ALT_SORT_ORDER)) {
		if(a->download == b->download) {
			if(a->status != b->status) {
				return (a->status == ConnectionInfo::STATUS_RUNNING) ? -1 : 1;
			}
		} else {
			return a->download ? -1 : 1;
		}
	} else {
		if(a->status == b->status) {
			if(a->download != b->download) {
				return a->download ? -1 : 1;
			}
		} else {
			return (a->status == ConnectionInfo::STATUS_RUNNING) ? -1 : 1;
		}
	}
	switch(col) {
	case CONNECTION_COLUMN_STATUS: return 0;
	case CONNECTION_COLUMN_SPEED: return compare(a->speed, b->speed);
	case CONNECTION_COLUMN_TRANSFERED: return compare(a->transfered, b->transfered);
	case CONNECTION_COLUMN_QUEUED: return compare(a->queued, b->queued);
	case CONNECTION_COLUMN_CHUNK: return compare(a->chunk, b->chunk);
	default: return QString::compare(StilUtils::TstrtoQ(a->columns[col]), StilUtils::TstrtoQ(b->columns[col]));
	}
}

TransferView::DownloadInfo::DownloadInfo(const string& target, int64_t size_, const TTHValue& tth_) : 
	path(target), 
	done(QueueManager::getInstance()->getPos(target)), 
	size(size_), 
	users(1),
	tth(tth_)
{
	columns[DOWNLOAD_COLUMN_FILE] = Text::toT(Util::getFileName(target));
	columns[DOWNLOAD_COLUMN_PATH] = Text::toT(Util::getFilePath(target));
	columns[DOWNLOAD_COLUMN_SIZE] = Text::toT(Util::formatBytes(size));
	
	update();
}

void TransferView::DownloadInfo::update(const TransferView::TickInfo& ti) {
	done = ti.done + QueueManager::getInstance()->getInstance()->getPos(ti.path);
	bps = ti.bps;
	update();
}

void TransferView::DownloadInfo::update() {
	if(users == 0) {
		columns[DOWNLOAD_COLUMN_STATUS] = T_("Waiting for slot");
		columns[DOWNLOAD_COLUMN_TIMELEFT].clear();
		columns[DOWNLOAD_COLUMN_SPEED].clear();
	} else {
		columns[DOWNLOAD_COLUMN_STATUS] = str(TFN_("Downloading from %1% user", "Downloading from %1% users", users) % users);
		columns[DOWNLOAD_COLUMN_TIMELEFT] = Text::toT(Util::formatSeconds(static_cast<int64_t>(timeleft())));
		columns[DOWNLOAD_COLUMN_SPEED] = str(TF_("%1%/s") % Text::toT(Util::formatBytes(static_cast<int64_t>(bps))));
	}
	columns[DOWNLOAD_COLUMN_DONE] = Text::toT(Util::formatBytes(done));
}

static tstring getFile(Transfer* t)
 {
	tstring file;
	
	if(t->getType() == Transfer::TYPE_TREE) {
		file = str(TF_("TTH: %1%") % Text::toT(Util::getFileName(t->getPath())));
	} else if(t->getType() == Transfer::TYPE_FULL_LIST || t->getType() == Transfer::TYPE_PARTIAL_LIST) {
		file = T_("file list");
	} else {
		file = Text::toT(Util::getFileName(t->getPath()) + 
			" (" + Util::formatBytes(t->getStartPos()) + 
			" - " + Util::formatBytes(t->getStartPos() + t->getSize()) + ")");
	}
	return file;
}

void TransferView::starting(UpdateInfo* ui, Transfer* t) 
{
	ui->setStatus(ConnectionInfo::STATUS_RUNNING);
	ui->setTransfered(t->getPos(), t->getActual());
	ui->setChunk(t->getPos(), t->getSize());
	const UserConnection& uc = t->getUserConnection();
	ui->setCipher(Text::toT(uc.getCipherName()));
	tstring country = Text::toT(Util::getIpCountry(uc.getRemoteIp()));
	tstring ip = Text::toT(uc.getRemoteIp());
	if(country.empty()) {
		ui->setIP(ip);
	} else {
		ui->setIP(country + _T(" (") + ip + _T(")"));
	}
}

void TransferView::onTransferTick(Transfer* t)
 {
	UpdateInfo* ui = new UpdateInfo(t->getUser(), true);
	ui->setTransfered(t->getPos(), t->getActual());
	ui->setSpeed(t->getAverageSpeed());
	ui->setChunk(t->getPos(), t->getSize());
	tasks.add(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(ConnectionManagerListener::Added, ConnectionQueueItem* aCqi) throw() 
{
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());

	ui->setStatus(ConnectionInfo::STATUS_WAITING);
	ui->setStatusString(T_("Connecting"));
	speak(CONNECTIONS_ADD, ui);
}

void TransferView::on(ConnectionManagerListener::StatusChanged, ConnectionQueueItem* aCqi) throw()
{
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());

	ui->setStatusString((aCqi->getState() == ConnectionQueueItem::CONNECTING) ? T_("Connecting") : T_("Waiting to retry"));

	speak(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(ConnectionManagerListener::Removed, ConnectionQueueItem* aCqi) throw() 
{
	speak(CONNECTIONS_REMOVE, new UpdateInfo(aCqi->getUser(), aCqi->getDownload()));
}

void TransferView::on(ConnectionManagerListener::Failed, ConnectionQueueItem* aCqi, const string& aReason) throw() 
{
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());
	if(aCqi->getUser()->isSet(User::OLD_CLIENT)) {
		ui->setStatusString(T_("Remote client does not fully support TTH - cannot download"));
	} else {
		ui->setStatusString(Text::toT(aReason));
	}
	speak(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(DownloadManagerListener::Requesting, Download* d) throw()
{
	UpdateInfo* ui = new UpdateInfo(d->getUser(), true);
	
	starting(ui, d);

	ui->setStatusString(str(TF_("Requesting %1%") % getFile(d)));

	speak(CONNECTIONS_UPDATE, ui);
	
//	speak(DOWNLOADS_ADD_USER, new TickInfo(d->getPath()));
}

void TransferView::on(DownloadManagerListener::Starting, Download* d) throw()
{
	UpdateInfo* ui = new UpdateInfo(d->getUser(), true);
	
	tstring statusString;

	if(d->getUserConnection().isSecure()) {
		if(d->getUserConnection().isTrusted()) {
			statusString += _T("[S]");
		} else {
			statusString += _T("[U]");
		}
	}
	if(d->isSet(Download::FLAG_TTH_CHECK)) {
		statusString += _T("[T]");
	}
	if(d->isSet(Download::FLAG_ZDOWNLOAD)) {
		statusString += _T("[Z]");
	}
	if(!statusString.empty()) {
		statusString += _T(" ");
	}
	statusString += str(TF_("Downloading %1%") % getFile(d));
	
	ui->setStatusString(statusString);
	speak(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(DownloadManagerListener::Tick, const DownloadList& dl) throw()
{
	for(DownloadList::const_iterator i = dl.begin(); i != dl.end(); ++i) {
		onTransferTick(*i);
	}

	std::vector<TickInfo*> dis;
	for(DownloadList::const_iterator i = dl.begin(); i != dl.end(); ++i) {
		Download* d = *i;
		if(d->getType() != Transfer::TYPE_FILE) {
			continue;
		}
		
		TickInfo* ti = 0;
		for(std::vector<TickInfo*>::iterator j = dis.begin(); j != dis.end(); ++j) {
			TickInfo* ti2 = *j;
			if(Util::stricmp(ti2->path, d->getPath()) == 0) {
				ti = ti2;
				break;
			}
		}
		if(!ti) {
			ti = new TickInfo(d->getPath());
			dis.push_back(ti);
		}
		ti->bps += d->getAverageSpeed();
		ti->done += d->getPos();
	}
	for(std::vector<TickInfo*>::iterator i = dis.begin(); i != dis.end(); ++i) {
		tasks.add(DOWNLOADS_TICK, *i);
	}

	speak();
}

void TransferView::on(DownloadManagerListener::Failed, Download* d, const string& aReason) throw()
{
	UpdateInfo* ui = new UpdateInfo(d->getUser(), true, true);
	ui->setStatus(ConnectionInfo::STATUS_WAITING);
	ui->setStatusString(Text::toT(aReason));

	speak(CONNECTIONS_UPDATE, ui);
	
//	speak(DOWNLOADS_REMOVE_USER, new TickInfo(d->getPath()));
}

void TransferView::on(UploadManagerListener::Starting, Upload* u) throw()
{
	UpdateInfo* ui = new UpdateInfo(u->getUser(), false);

	starting(ui, u);

	tstring statusString;

	if(u->getUserConnection().isSecure()) {
		if(u->getUserConnection().isTrusted()) {
			statusString += _T("[S]");
		} else {
			statusString += _T("[U]");
		}
	}
	if(u->isSet(Upload::FLAG_ZUPLOAD)) {
		statusString += _T("[Z]");
	}
	if(!statusString.empty()) {
		statusString += _T(" ");
	}
	statusString += str(TF_("Uploading %1%") % getFile(u));

	ui->setStatusString(statusString);

	speak(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(UploadManagerListener::Tick, const UploadList& ul) throw()
{
	for(UploadList::const_iterator i = ul.begin(); i != ul.end(); ++i) {
		onTransferTick(*i);
	}

	speak();
}

void TransferView::on(DownloadManagerListener::Complete, Download* d) throw()
{ 
	onTransferComplete(d, false);

//	speak(DOWNLOADS_REMOVE_USER, new TickInfo(d->getPath()));
}

void TransferView::on(UploadManagerListener::Complete, Upload* aUpload) throw()
{ 
	onTransferComplete(aUpload, true); 
}

void TransferView::onTransferComplete(Transfer* aTransfer, bool isUpload)
{
	UpdateInfo* ui = new UpdateInfo(aTransfer->getUser(), !isUpload);

	ui->setStatus(ConnectionInfo::STATUS_WAITING);
	ui->setStatusString(T_("Idle"));
	ui->setChunk(aTransfer->getPos(), aTransfer->getSize());

	speak(CONNECTIONS_UPDATE, ui);
}

void TransferView::on(QueueManagerListener::Removed, QueueItem* qi) throw() 
{
//	speak(DOWNLOADS_REMOVED, new TickInfo(qi->getTarget()));
}

