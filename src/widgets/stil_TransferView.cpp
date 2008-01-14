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

#include "stil_TransferView.h"
#include "myprogress.h"


int TransferView::columnSizes[] = { 170, 80, 320, 100, 100, 223, 150, 250, 90, 100, 150, 150 };

static ResourceManager::Strings columnNames[] = { ResourceManager::USER, ResourceManager::HUB, ResourceManager::STATUS,
ResourceManager::TIME_LEFT, ResourceManager::SPEED, ResourceManager::FILENAME, ResourceManager::SIZE, ResourceManager::PATH,
ResourceManager::IP_BARE, ResourceManager::RATIO, ResourceManager::CID, ResourceManager::CIPHER };

void TransferView::preClose()
{
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) w << QString::number(columnWidth(i));
	QString r = w.join(",");
	SettingsManager::getInstance()->set(SettingsManager::MAINFRAME_WIDTHS, r.toStdString());
	
	//SettingsManager::getInstance()->set(SettingsManager::MAINFRAME_ORDER, WinUtil::toString(transfers->getColumnOrder()));
	
	DownloadManager::getInstance()->removeListener(this);
	UploadManager::getInstance()->removeListener(this);
	ConnectionManager::getInstance()->removeListener(this);
}

void TransferView::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) hideColumn(-index-1);
	else showColumn(index);
}

void TransferView::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, ResourceManager::getInstance()->getStrings(columnNames))
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
}

void TransferView::showCnxtMenu(const QPoint& point)
{
	// HERE NEED TO GENERATE MENU FORM ITEMINFO CLASS
	
	// SHOW MENU
	if (indexAt(point).isValid()) cnxtMenu->exec(mapToGlobal(point));
}

TransferView::~TransferView() 
{
	datalist.clear();
	datalistitem.clear();
	delete cnxtMenu;
	delete columnMenu;
}

TransferView::TransferView(QWidget *parent) : QTreeWidget(parent)
{
	datalist.clear();
	datalistitem.clear();
	connect(this, SIGNAL(sigSpeak()), this, SLOT(slotSpeak()), Qt::QueuedConnection);
	
	QStringList columns;
	foreach(tstring name, ResourceManager::getInstance()->getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	setHeaderLabels(columns);
	
	QStringList clist = QString::fromStdString(SETTING(MAINFRAME_WIDTHS)).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) setColumnWidth(i, columnSizes[i]);
	else 
		for (int i=0; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() < 0)) setColumnWidth(i, columnSizes[i]);
				else setColumnWidth(i, clist[i].toInt());
			}
			
	
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	cnxtMenu = new QMenu();
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
//	transfers->setColumnOrder(WinUtil::splitTokens(SETTING(MAINFRAME_ORDER), columnIndexes));
//	transfers->setColor(WinUtil::textColor, WinUtil::bgColor);
//	transfers->setSort(COLUMN_USER);
//	transfers->onContextMenu(std::tr1::bind(&TransferView::handleContextMenu, this, _1));
//	transfers->onKeyDown(std::tr1::bind(&TransferView::handleKeyDown, this, _1));
//	transfers->onDblClicked(std::tr1::bind(&TransferView::handleDblClicked, this));
	
//	onRaw(std::tr1::bind(&TransferView::handleDestroy, this, _1, _2), SmartWin::Message(WM_DESTROY));
//	noEraseBackground();
		
	ConnectionManager::getInstance()->addListener(this);
	DownloadManager::getInstance()->addListener(this);
	UploadManager::getInstance()->addListener(this);
}
/*

bool TransferView::handleContextMenu(SmartWin::ScreenCoordinate pt) {
	if (transfers->hasSelection()) {
		if(pt.x() == -1 && pt.y() == -1) {
			pt = transfers->getContextMenuPos();
		}

		/// @todo Fix multiple selection menu...
		int i = -1;
		ItemInfo* ii = transfers->getSelectedData();
		WidgetMenuPtr contextMenu = makeContextMenu(ii);
		contextMenu->trackPopupMenu(this, pt, TPM_LEFTALIGN | TPM_RIGHTBUTTON);

		return true;
	}
	return false;
}


TransferView::WidgetMenuPtr TransferView::makeContextMenu(ItemInfo* ii) {
	WidgetMenuPtr menu = createMenu(true);
	
	appendUserItems(mdi, menu);
	menu->appendSeparatorItem();
	
	menu->appendItem(IDC_FORCE, TSTRING(FORCE_ATTEMPT), std::tr1::bind(&TransferView::handleForce, this));
	if(ii->download) {
		menu->appendItem(IDC_SEARCH_ALTERNATES, CTSTRING(SEARCH_FOR_ALTERNATES), std::tr1::bind(&TransferView::handleSearchAlternates, this));
	}
	menu->appendItem(IDC_COPY_NICK, TSTRING(COPY_NICK), std::tr1::bind(&TransferView::handleCopyNick, this));
	menu->appendSeparatorItem();
	menu->appendItem(IDC_REMOVE, TSTRING(CLOSE_CONNECTION), std::tr1::bind(&TransferView::handleRemove, this));
	menu->setDefaultItem(IDC_PRIVATEMESSAGE);
	return menu;
}
*/

void TransferView::handleRemove() 
{
	QList<QTreeWidgetItem *> lt = selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		ItemInfo* ii = getItemInfoFromItem(lt[i]); 
		ii->disconnect();
	}
}

void TransferView::runUserCommand(const UserCommand& uc) 
{
	/*
	if(!WinUtil::getUCParams(this, uc, ucLineParams))
		return;

	StringMap ucParams = ucLineParams;

	int i = -1;
	while((i = transfers->getNext(i, LVNI_SELECTED)) != -1) {
		ItemInfo* itemI = transfers->getData(i);
		if(!itemI->user->isOnline())
			continue;

		StringMap tmp = ucParams;
		/// @todo tmp["fileFN"] = Text::fromT(itemI->path + itemI->file);

		// compatibility with 0.674 and earlier
		ucParams["file"] = ucParams["fileFN"];

		ClientManager::getInstance()->userCommand(itemI->user, uc, tmp, true);
	}
	*/
}

/*
bool TransferView::handleKeyDown(int c) {
	if(c == VK_DELETE) {
		transfers->forEachSelected(&ItemInfo::disconnect);
	}
	return true;
}
*/

void TransferView::handleForce()
{
	QList<QTreeWidgetItem *> lt = selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		ItemInfo* ii = getItemInfoFromItem(lt[i]); 
		//->setText(i, COLUMN_STATUS, TSTRING(CONNECTING_FORCED));
		ConnectionManager::getInstance()->force(ii->user);
	}
}

void TransferView::handleCopyNick()
{
	QTreeWidgetItem * lt = currentItem();
	ItemInfo* ii = getItemInfoFromItem(lt);
/*	int i = -1;

	/// @todo Fix when more items are selected
	while( (i = transfers->getNext(i, LVNI_SELECTED)) != -1) {
		WinUtil::setClipboard(WinUtil::getNicks(transfers->getData(i)->user));
	}
*/
}

void TransferView::handleDblClicked()
{
	QList<QTreeWidgetItem *> lt = selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		ItemInfo* ii = getItemInfoFromItem(lt[i]); 
		if(ii) ii->pm(this); // WARN:  NOT SURE ABOUT "this"
	}
}

void TransferView::handleSearchAlternates()
{
	QList<QTreeWidgetItem *> lt = selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		ItemInfo* ii = getItemInfoFromItem(lt[i]); 
		string target = Text::fromT(ii->getText(COLUMN_PATH) + ii->getText(COLUMN_FILE));
		TTHValue tth;
		if(QueueManager::getInstance()->getTTH(target, tth))
			; 
			//WinUtil::searchHash(tth);
			// - --- - - - - - OR - - - - --  - -- - - -
			// TTHValue aHash = TTH;
			// SearchFrame::openWindow(mainWindow->getMDIParent(), Text::toT(aHash.toBase32()), 0, SearchManager::SIZE_DONTCARE, SearchManager::TYPE_TTH);
	}
}

int TransferView::ItemInfo::compareItems(ItemInfo* a, ItemInfo* b, int col) {
	if(BOOLSETTING(ALT_SORT_ORDER)) {
		if(a->download == b->download) {
			if(a->status != b->status) {
				return (a->status == ItemInfo::STATUS_RUNNING) ? -1 : 1;
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
			return (a->status == ItemInfo::STATUS_RUNNING) ? -1 : 1;
		}
	}
	switch(col) {
	case COLUMN_STATUS: return 0;
	case COLUMN_TIMELEFT: return compare(a->timeLeft, b->timeLeft);
	case COLUMN_SPEED: return compare(a->speed, b->speed);
	case COLUMN_SIZE: return compare(a->size, b->size);
	case COLUMN_RATIO: return compare(a->getRatio(), b->getRatio());
	#ifdef UNICODE
	default: return wcscasecmp(a->columns[col].c_str(), b->columns[col].c_str());
	#else
	default: return strcasecmp(a->columns[col].c_str(), b->columns[col].c_str());
	#endif
	}
}

TransferView::ItemInfo * TransferView::getItemInfoFromItem(QTreeWidgetItem * item)
{
	if (datalistitem.isEmpty()) return NULL;
	QModelIndex mi = indexFromItem(item);
	for (int i = 0; i < datalistitem.size(); i++) 
		if ( datalistitem[i] == mi ) return datalist[i];
	return NULL;
}

void TransferView::IIinsert(ItemInfo* ii)
{
	datalist << ii;
	QStringList lst;
	for (int i=COLUMN_FIRST; i<COLUMN_LAST; i++) 
		if (i!=COLUMN_STATUS) lst << StilUtils::TstrtoQ(ii->columns[i]); else lst << "";
	setUpdatesEnabled(false);
	QTreeWidgetItem *fItem = new QTreeWidgetItem(this, lst);
	
	MyProgressBar *pb = new MyProgressBar();
	pb->setMaximum(ii->size);
	pb->setTextVisible(true);
	pb->setFormat(StilUtils::TstrtoQ(ii->columns[COLUMN_STATUS]));
	pb->setValue(0);
	if (ii->download) fItem->setIcon(0,QIcon(":images/trans_DL.png")); else fItem->setIcon(0,QIcon(":images/trans_UL.png"));
	//COLORREF barBase = ii->download ? SETTING(DOWNLOAD_BAR_COLOR) : SETTING(UPLOAD_BAR_COLOR);
	setItemWidget ( fItem, COLUMN_STATUS, pb );
	
	datalistitem << indexFromItem(fItem);
	setUpdatesEnabled(true);
}

void TransferView::IIupdate(int index)
{
	setUpdatesEnabled(false);
	for (int i=COLUMN_FIRST; i<COLUMN_LAST; i++)
	{
		if (i != COLUMN_STATUS)
			itemFromIndex(datalistitem[index])->setText(i, StilUtils::TstrtoQ(datalist[index]->columns[i]) );
		else // If current_column is ProgressBar
			{
				MyProgressBar *pb = qobject_cast<MyProgressBar *>( itemWidget(itemFromIndex(datalistitem[index]), COLUMN_STATUS) );
				pb->setMaximum(datalist[index]->size);
				pb->setFormat(StilUtils::TstrtoQ(datalist[index]->columns[COLUMN_STATUS]));
				pb->setValue(datalist[index]->pos);
			}
	}
	setUpdatesEnabled(true);
}

void TransferView::IIerase(int index)
{
	if ( (datalist.isEmpty()) || (datalistitem.isEmpty()) ) return;
	if ( (index < 0)||(index >= datalist.size()) ) return;

	setUpdatesEnabled(false);
	if (datalist[index])
		{
			QTreeWidgetItem *w = itemFromIndex(datalistitem[index]);
			if (w) delete w;
			datalist.removeAt(index);
			datalistitem.removeAt(index);
		}
	setUpdatesEnabled(true);
}

void TransferView::slotSpeak()
{
	TaskQueue::List t;
	tasks.get(t);
	

	for(TaskQueue::Iter i = t.begin(); i != t.end(); ++i) {
		if(i->first == ADD_ITEM) {
			auto_ptr<UpdateInfo> ui(reinterpret_cast<UpdateInfo*>(i->second));
			ItemInfo* ii = new ItemInfo(ui->user, ui->download);
			ii->update(*ui);
			IIinsert(ii);
		} else if(i->first == REMOVE_ITEM) {
			auto_ptr<UpdateInfo> ui(reinterpret_cast<UpdateInfo*>(i->second));
			int ic = datalist.size();
			for(int i = 0; i < ic; ++i) {
				ItemInfo* ii = datalist[i];
				if(*ui == *ii) {
					IIerase(i);
					break;
				}
			}
		} else if(i->first == UPDATE_ITEM) {
			auto_ptr<UpdateInfo> ui(reinterpret_cast<UpdateInfo*>(i->second));
			int ic = datalist.size();
			for(int i = 0; i < ic; ++i) {
				ItemInfo* ii = datalist[i];
				if(ii->download == ui->download && ii->user == ui->user) {
					ii->update(*ui);
					IIupdate(i);
					break;
				}
			}
		}
	}

	//if(!t.empty()) { transfers->resort(); }
}

TransferView::ItemInfo::ItemInfo(const UserPtr& u, bool aDownload) : UserInfoBase(u), download(aDownload), transferFailed(false),
	status(STATUS_WAITING), pos(0), size(0), start(0), actual(0), speed(0), timeLeft(0)
{
	columns[COLUMN_USER] = Text::toT(StilUtils::getNicks(u));
	columns[COLUMN_HUB] = (StilUtils::getHubNames(u)).first;
	columns[COLUMN_CID] = Text::toT(u->getCID().toBase32());
}

void TransferView::ItemInfo::update(const UpdateInfo& ui)
{
	if(ui.updateMask & UpdateInfo::MASK_STATUS) {
		status = ui.status;
	}
	if(ui.updateMask & UpdateInfo::MASK_STATUS_STRING) {
		// No slots etc from transfermanager better than disconnected from connectionmanager
		if(!transferFailed)
			columns[COLUMN_STATUS] = ui.statusString;
		transferFailed = ui.transferFailed;
	}
	if(ui.updateMask & UpdateInfo::MASK_SIZE) {
		size = ui.size;
		columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(size));
	}
	if(ui.updateMask & UpdateInfo::MASK_START) {
		start = ui.start;
	}
	if(ui.updateMask & UpdateInfo::MASK_POS) {
		pos = start + ui.pos;
	}
	if(ui.updateMask & UpdateInfo::MASK_ACTUAL) {
		actual = start + ui.actual;
		columns[COLUMN_RATIO] = Text::toT(Util::toString(getRatio()));
	}
	if(ui.updateMask & UpdateInfo::MASK_SPEED) {
		speed = ui.speed;
		if (status == STATUS_RUNNING) {
			columns[COLUMN_SPEED] = Text::toT(Util::formatBytes(speed) + "/s");
		} else {
			columns[COLUMN_SPEED] = Util::emptyStringT;
		}
	}
	if(ui.updateMask & UpdateInfo::MASK_FILE) {
		columns[COLUMN_FILE] = ui.file;
		columns[COLUMN_PATH] = ui.path;
	}
	if(ui.updateMask & UpdateInfo::MASK_TIMELEFT) {
		timeLeft = ui.timeLeft;
		if (status == STATUS_RUNNING) {
			columns[COLUMN_TIMELEFT] = Text::toT(Util::formatSeconds(timeLeft));
		} else {
			columns[COLUMN_TIMELEFT] = Util::emptyStringT;
		}
	}
	if(ui.updateMask & UpdateInfo::MASK_IP) {
		columns[COLUMN_IP] = ui.IP;
	}
	if(ui.updateMask & UpdateInfo::MASK_CIPHER) {
		columns[COLUMN_CIPHER] = ui.cipher;
	}
}

void TransferView::on(ConnectionManagerListener::Added, ConnectionQueueItem* aCqi) throw() {
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());

	ui->setStatus(ItemInfo::STATUS_WAITING);
	ui->setStatusString(TSTRING(CONNECTING));
	speak(ADD_ITEM, ui);
}

void TransferView::on(ConnectionManagerListener::StatusChanged, ConnectionQueueItem* aCqi) throw() {
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());

	ui->setStatusString((aCqi->getState() == ConnectionQueueItem::CONNECTING) ? TSTRING(CONNECTING) : TSTRING(WAITING_TO_RETRY));

	speak(UPDATE_ITEM, ui);
}

void TransferView::on(ConnectionManagerListener::Removed, ConnectionQueueItem* aCqi) throw() {
	speak(REMOVE_ITEM, new UpdateInfo(aCqi->getUser(), aCqi->getDownload()));
}

void TransferView::on(ConnectionManagerListener::Failed, ConnectionQueueItem* aCqi, const string& aReason) throw() {
	UpdateInfo* ui = new UpdateInfo(aCqi->getUser(), aCqi->getDownload());
	if(aCqi->getUser()->isSet(User::OLD_CLIENT)) {
		ui->setStatusString(TSTRING(SOURCE_TOO_OLD));
	} else {
		ui->setStatusString(Text::toT(aReason));
	}
	speak(UPDATE_ITEM, ui);
}

void TransferView::on(DownloadManagerListener::Starting, Download* aDownload) throw() {
	UpdateInfo* ui = new UpdateInfo(aDownload->getUser(), true);
	ui->setStatus(ItemInfo::STATUS_RUNNING);
	ui->setPos(aDownload->getPos());
	ui->setActual(aDownload->getActual());
	ui->setStart(0);
	ui->setSize(aDownload->getSize());
	ui->setFile(Text::toT(aDownload->getPath()));
	ui->setStatusString(TSTRING(DOWNLOAD_STARTING));
	ui->setCipher(Text::toT(aDownload->getUserConnection().getCipherName()));
	tstring country = Text::toT(Util::getIpCountry(aDownload->getUserConnection().getRemoteIp()));
	tstring ip = Text::toT(aDownload->getUserConnection().getRemoteIp());
	if(country.empty()) {
		ui->setIP(ip);
	} else {
		ui->setIP(country + _T(" (") + ip + _T(")"));
	}
	if(aDownload->getType() == Transfer::TYPE_TREE) {
		ui->file = _T("TTH: ") + ui->file;
	}

	speak(UPDATE_ITEM, ui);
}

void TransferView::on(DownloadManagerListener::Tick, const DownloadList& dl) throw()  {
	for(DownloadList::const_iterator j = dl.begin(); j != dl.end(); ++j) {
		Download* d = *j;

		UpdateInfo* ui = new UpdateInfo(d->getUser(), true);
		ui->setActual(d->getActual());
		ui->setPos(d->getPos());
		ui->setTimeLeft(d->getSecondsLeft());
		ui->setSpeed(d->getAverageSpeed());

		tstring pos = Text::toT(Util::formatBytes(d->getPos()));
		double percent = (double)d->getPos()*100.0/(double)d->getSize();
		tstring elapsed = Text::toT(Util::formatSeconds((GET_TICK() - d->getStart())/1000)); 

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
		
		statusString += _T("Downloaded ") + pos + _T(" (") + StilUtils::QtoTstr(QString::number(percent, 'g', 3)+"%") + _T(") in ") + elapsed;
		ui->setStatusString(statusString);

		tasks.add(UPDATE_ITEM, ui);
	}

	speak();
}

void TransferView::on(DownloadManagerListener::Failed, Download* aDownload, const string& aReason) throw() {
	UpdateInfo* ui = new UpdateInfo(aDownload->getUser(), true, true);
	ui->setStatus(ItemInfo::STATUS_WAITING);
	ui->setPos(0);
	ui->setStatusString(Text::toT(aReason));
	ui->setSize(aDownload->getSize());
	ui->setFile(Text::toT(aDownload->getPath()));
	if(aDownload->getType() == Transfer::TYPE_TREE) {
		ui->file = _T("TTH: ") + ui->file;
	}

	speak(UPDATE_ITEM, ui);
}

void TransferView::on(UploadManagerListener::Starting, Upload* aUpload) throw() {
	UpdateInfo* ui = new UpdateInfo(aUpload->getUser(), false);

	ui->setStatus(ItemInfo::STATUS_RUNNING);
	ui->setPos(aUpload->getPos());
	ui->setActual(aUpload->getActual());
	ui->setStart(aUpload->getPos());
	ui->setSize(aUpload->getSize());
	ui->setFile(Text::toT(aUpload->getPath()));
	ui->setStatusString(TSTRING(UPLOAD_STARTING));
	ui->setCipher(Text::toT(aUpload->getUserConnection().getCipherName()));
	tstring country = Text::toT(Util::getIpCountry(aUpload->getUserConnection().getRemoteIp()));
	tstring ip = Text::toT(aUpload->getUserConnection().getRemoteIp());
	if(country.empty()) {
		ui->setIP(ip);
	} else {
		ui->setIP(country + _T(" (") + ip + _T(")"));
	}
	if(aUpload->getType() == Transfer::TYPE_TREE) {
		ui->file = _T("TTH: ") + ui->file;
	}

	speak(UPDATE_ITEM, ui);
}

void TransferView::on(UploadManagerListener::Tick, const UploadList& ul) throw() {
	AutoArray<TCHAR> buf(TSTRING(UPLOADED_BYTES).size() + 64);

	for(UploadList::const_iterator j = ul.begin(); j != ul.end(); ++j) {
		Upload* u = *j;

		UpdateInfo* ui = new UpdateInfo(u->getUser(), false);
		ui->setActual(u->getActual());
		ui->setPos(u->getPos());
		ui->setTimeLeft(u->getSecondsLeft());
		ui->setSpeed(u->getAverageSpeed());

		tstring pos = Text::toT(Util::formatBytes(u->getPos()));
		double percent = (double)u->getPos()*100.0/(double)u->getSize();
		tstring elapsed = Text::toT(Util::formatSeconds((GET_TICK() - u->getStart())/1000)); 

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
		
		statusString += _T("Uploaded ") + pos + _T(" (") + StilUtils::QtoTstr(QString::number(percent, 'g', 3) + "%") + _T(") in ") + elapsed; 
		ui->setStatusString(statusString);

		tasks.add(UPDATE_ITEM, ui);
	}

	speak();
}

void TransferView::onTransferComplete(Transfer* aTransfer, bool isUpload) {
	UpdateInfo* ui = new UpdateInfo(aTransfer->getUser(), !isUpload);

	ui->setStatus(ItemInfo::STATUS_WAITING);
	ui->setPos(0);
	ui->setStatusString(isUpload ? TSTRING(UPLOAD_FINISHED_IDLE) : TSTRING(DOWNLOAD_FINISHED_IDLE));

	speak(UPDATE_ITEM, ui);
}

void TransferView::ItemInfo::disconnect()
{
	ConnectionManager::getInstance()->disconnect(user, download);
}

bool TransferView::speak()
{
	emit sigSpeak();
	return true;
}
