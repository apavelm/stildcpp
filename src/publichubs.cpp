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


#include "publichubs.h"
#include "mainwindowimpl.h"

int PublicHubWindow::columnSizes[] = { 200, 290, 50, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

static const char*  columnNames[] = {
	N_("Name"),
	N_("Description"),
	N_("Users"),
	N_("Address"),
	N_("Country"),
	N_("Shared"),
	N_("Min Share"),
	N_("Min Slots"),
	N_("Max Hubs"),
	N_("Max Users"),
	N_("Reliability"),
	N_("Rating")
};


PublicHubWindow::HubInfo::HubInfo(const HubEntry* entry_) : entry(entry_)
{
	columns[COLUMN_NAME] = Text::toT(entry->getName());
	columns[COLUMN_DESCRIPTION] = Text::toT(entry->getDescription());
	columns[COLUMN_USERS] = Text::toT(Util::toString(entry->getUsers()));
	columns[COLUMN_SERVER] = Text::toT(entry->getServer());
	columns[COLUMN_COUNTRY] = Text::toT(entry->getCountry());
	columns[COLUMN_SHARED] = Text::toT(Util::formatBytes(entry->getShared()));
	columns[COLUMN_MINSHARE] = Text::toT(Util::formatBytes(entry->getMinShare()));
	columns[COLUMN_MINSLOTS] = Text::toT(Util::toString(entry->getMinSlots()));
	columns[COLUMN_MAXHUBS] = Text::toT(Util::toString(entry->getMaxHubs()));
	columns[COLUMN_MAXUSERS] = Text::toT(Util::toString(entry->getMaxUsers()));
	columns[COLUMN_RELIABILITY] = Text::toT(Util::toString(entry->getReliability()));
	columns[COLUMN_RATING] = Text::toT(entry->getRating());
}

int PublicHubWindow::HubInfo::compareItems(const HubInfo* a, const HubInfo* b, int col)
{
	switch(col)
	{
		case COLUMN_USERS: return compare(a->entry->getUsers(), b->entry->getUsers()); 
		case COLUMN_MINSLOTS: return compare(a->entry->getMinSlots(), b->entry->getMinSlots());
		case COLUMN_MAXHUBS: return compare(a->entry->getMaxHubs(), b->entry->getMaxHubs());
		case COLUMN_MAXUSERS: return compare(a->entry->getMaxUsers(), b->entry->getMaxUsers());
		case COLUMN_RELIABILITY: return compare(a->entry->getReliability(), b->entry->getReliability());
		case COLUMN_SHARED: return compare(a->entry->getShared(), b->entry->getShared());
		case COLUMN_MINSHARE: return compare(a->entry->getMinShare(), b->entry->getMinShare());
		case COLUMN_RATING: return compare(a->entry->getRating(), b->entry->getRating());
		default: return QString::compare(StilUtils::TstrtoQ(a->columns[col]), StilUtils::TstrtoQ(b->columns[col]),  Qt::CaseInsensitive);
	}
}

PublicHubWindow::PublicHubWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_PUBLIC_HUBS;
	
	datalist.clear();
	datalistitem.clear();
	hubs->clear();
	
	// labels
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	hubs->setHeaderLabels(columns);
	
	// seting sort
	hubs->sortItems(COLUMN_USERS, Qt::DescendingOrder);
	
	connect(hubs, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotOpen(QTreeWidgetItem *, int)) );
	connect(hubs, SIGNAL(sig_KeyPressEvent(QKeyEvent*)), this, SLOT(slotKeyPressed(QKeyEvent*)));
	connect(filter, SIGNAL(returnPressed()), this, SLOT(slotFilterReturn()));
	
	// width
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(PUBLICHUBSFRAME_WIDTHS))).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) hubs->setColumnWidth(i, columnSizes[i]);
	else 
		for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) hubs->setColumnWidth(i, columnSizes[i]);
				else hubs->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	QStringList vlist = APPSTRING(s_PUBHUBS_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == COLUMN_LAST)
		for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) hubs->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(PUBLICHUBSFRAME_ORDER))).split(",");
	if (olist.size() == COLUMN_LAST)
		for (int j= COLUMN_FIRST; j<COLUMN_LAST; j++) hubs->header()->swapSections(hubs->header()->visualIndex(olist[j].toInt()), j);
		
	// COLUMN (HEADER) MENUS
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	hubs->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(hubs->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	// CONTEXT MENU
	cnxtMenu = new QMenu();
	hubs->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(hubs, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	cnxtMenu->clear();
	QAction * t = cnxtMenu->addAction(/*QIcon(":/images/connect.png"),*/ StilUtils::TstrtoQ(T_("&Connect")) ,this ,SLOT(slotConnect()) );
	cnxtMenu->addAction(/*QIcon(":/images/add_to_fav.png"),*/ StilUtils::TstrtoQ(T_("Add To &Favorites")) ,this ,SLOT(slotAddtoFav()) );
	cnxtMenu->addAction(/*QIcon(":/images/copy2cliboard.png"),*/ StilUtils::TstrtoQ(T_("Copy &address to clipboard")) ,this ,SLOT(slotCopyHub()) );
	cnxtMenu->setDefaultAction(t);
	
	//populate the filter list with the column names
	for(int j=0; j<COLUMN_LAST; j++) 
		filterSel->addItem(StilUtils::TstrtoQ(T_(columnNames[j])));
	filterSel->addItem(StilUtils::TstrtoQ(T_("Any")));
	filterSel->setCurrentIndex(COLUMN_LAST);
	connect(filterSel, SIGNAL(currentIndexChanged(int)), this, SLOT(updateList(int)) );
	
	configure->setText(StilUtils::TstrtoQ(T_("&Configure")));
	connect(configure, SIGNAL(clicked()), this, SLOT(slotConf()) );

	refresh->setText(StilUtils::TstrtoQ(T_("&Refresh")));
	connect(refresh, SIGNAL(clicked()), this, SLOT(slotRefresh()) );
	connect(pubLists, SIGNAL(currentIndexChanged(int)), this, SLOT(slotListSelChanged(int)) );

	groupBox_2->setTitle(StilUtils::TstrtoQ(T_("Configured Public Hub Lists")));
	groupBox->setTitle(StilUtils::TstrtoQ(T_("F&ilter")));


	FavoriteManager::getInstance()->addListener(this);
	entries = FavoriteManager::getInstance()->getPublicHubs();

	// populate with values from the settings
	updateDropDown();
	updateList();
	
	connect(this, SIGNAL(sigSpeak(int, tstring )), this, SLOT(slotSpeak(int, tstring )), Qt::QueuedConnection);
	
	if(FavoriteManager::getInstance()->isDownloading())
		setStatus(STATUS_STATUS, T_("Downloading public hub list..."));
	else 
		if(entries.empty()) FavoriteManager::getInstance()->refresh();
}

void PublicHubWindow::updateDropDown()
{
	pubLists->clear();
	StringList lists(FavoriteManager::getInstance()->getHubLists());
	for(StringList::iterator idx = lists.begin(); idx != lists.end(); ++idx)
	{
		pubLists->addItem(StilUtils::TstrtoQ(Text::toT(*idx).c_str()));
	}
	pubLists->setCurrentIndex(FavoriteManager::getInstance()->getSelectedHubList());
}

void PublicHubWindow::setStatus(Status a, tstring t)
{
	switch (a)
	{
		case STATUS_STATUS: lbl_status->setText(StilUtils::TstrtoQ(t)); break;
		case STATUS_HUBS: lbl_hubs->setText(StilUtils::TstrtoQ(t)); break;
		case STATUS_USERS: lbl_users->setText(StilUtils::TstrtoQ(t)); break;
		case STATUS_LAST: break;
	}
}

PublicHubWindow::~PublicHubWindow()
{
	FavoriteManager::getInstance()->removeListener(this);
	datalist.clear();
	datalistitem.clear();
	
	// visibility
	QStringList vv;
	for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) 
		vv << QString::number(hubs->header()->isSectionHidden(i));
	SETAPPSTRING(s_PUBHUBS_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) 
		w << QString::number(hubs->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::PUBLICHUBSFRAME_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i= COLUMN_FIRST; i<COLUMN_LAST; i++) 
		ww << QString::number(hubs->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::PUBLICHUBSFRAME_ORDER, ww.join(",").toStdString());
	
	hubs->clear();
	
	delete cnxtMenu;
	delete columnMenu;
}

void PublicHubWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) hubs->hideColumn(-index-1);
	else hubs->showColumn(index);
}

void PublicHubWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = hubs->header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(hubs->header()->mapToGlobal(point));
}

void PublicHubWindow::showCnxtMenu(const QPoint& point)
{
	if (hubs->indexAt(point).isValid())
		cnxtMenu->exec(mapToGlobal(point));
}

void PublicHubWindow::insert(HubInfo * hi)
{
	QStringList l;
	for (int i = COLUMN_FIRST; i < COLUMN_LAST; i++)
		l << StilUtils::TstrtoQ(hi->getText(i));
	datalist << hi;
	QTreeWidgetItem * w = new QTreeWidgetItem(hubs, l);
	datalistitem << w;
}

void PublicHubWindow::updateList(int sel)
{
	hubs->clear();
	//HubInfo* h;
	//foreach(h, datalist) delete h;
	datalist.clear();
	//QTreeWidgetItem * w;
	//foreach(w, datalistitem) delete w;
	datalistitem.clear();
	users = 0;
	visibleHubs = 0;
	
	hubs->setUpdatesEnabled(false);
	
	double size = -1;
	FilterModes mode = NONE;

	bool doSizeCompare = parseFilter(mode, size);

	for(HubEntryList::const_iterator i = entries.begin(); i != entries.end(); ++i)
	{
		if(matchFilter(*i, sel, doSizeCompare, mode, size))
		{
			insert(new HubInfo(&(*i)));
			visibleHubs++;
			users += i->getUsers();
		}
	}

	hubs->setUpdatesEnabled(true);

	setStatus(STATUS_HUBS, str(TF_("Hubs: %1%") % visibleHubs));
	setStatus(STATUS_USERS, str(TF_("Users: %1%") % users));
}

void PublicHubWindow::slotSpeak(int type, tstring x)
{
	if((type == FINISHED) || (type == LOADED_FROM_CACHE))
	{
		entries = FavoriteManager::getInstance()->getPublicHubs();
		updateList();
		setStatus(STATUS_STATUS, ((type == LOADED_FROM_CACHE) ? T_("Hub list loaded from cache...") : str(TF_("Hub list downloaded... (%1%)") % (x))));
		return;
	}
	if(type == STARTING)
	{
		setStatus(STATUS_STATUS, str(TF_("Downloading public hub list... (%1%)") % (x)));
		return;
	}
	if(type == FAILED)
	{
		setStatus(STATUS_STATUS, str(TF_("Download failed: %1%") % (x)));
		return;
	}
}

bool PublicHubWindow::parseFilter(FilterModes& mode, double& size)
{
	string::size_type start = (string::size_type)string::npos;
	string::size_type end = (string::size_type)string::npos;
	int64_t multiplier = 1;

	if(filterString.compare(0, 2, ">=") == 0) {
		mode = GREATER_EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, "<=") == 0) {
		mode = LESS_EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, "==") == 0) {
		mode = EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, "!=") == 0) {
		mode = NOT_EQUAL;
		start = 2;
	} else if(filterString[0] == _T('<')) {
		mode = LESS;
		start = 1;
	} else if(filterString[0] == _T('>')) {
		mode = GREATER;
		start = 1;
	} else if(filterString[0] == _T('=')) {
		mode = EQUAL;
		start = 1;
	}

	if(start == string::npos)
		return false;
	if(filterString.length() <= start)
		return false;
	
	if((end = Util::findSubString(filterString, "YiB")) != tstring::npos) {
		multiplier = 1024LL * 1024LL * 1024LL * 1024LL * 1024LL;
	} else if((end = Util::findSubString(filterString, "TiB")) != tstring::npos) {
		multiplier = 1024LL * 1024LL * 1024LL * 1024LL;
	} else if((end = Util::findSubString(filterString, "GiB")) != tstring::npos) {
		multiplier = 1024*1024*1024;
	} else if((end = Util::findSubString(filterString, "MiB")) != tstring::npos) {
		multiplier = 1024*1024;
	} else if((end = Util::findSubString(filterString, "KiB")) != tstring::npos) {
		multiplier = 1024;
	} else if((end = Util::findSubString(filterString, "YB")) != tstring::npos) {
		multiplier = 1000LL * 1000LL * 1000LL * 1000LL * 1000LL;
	} else if((end = Util::findSubString(filterString, "TB")) != tstring::npos) {
		multiplier = 1000LL * 1000LL * 1000LL * 1000LL;
	} else if((end = Util::findSubString(filterString, "GB")) != tstring::npos) {
		multiplier = 1000*1000*1000;
	} else if((end = Util::findSubString(filterString, "MB")) != tstring::npos) {
		multiplier = 1000*1000;
	} else if((end = Util::findSubString(filterString, "kB")) != tstring::npos) {
		multiplier = 1000;
	} else if((end = Util::findSubString(filterString, "B")) != tstring::npos) {
		multiplier = 1;
	}

	if(end == string::npos) {
		end = filterString.length();
	}

	string tmpSize = filterString.substr(start, end-start);
	size = Util::toDouble(tmpSize) * multiplier;

	return true;
}

bool PublicHubWindow::matchFilter(const HubEntry& entry, const int& sel, bool doSizeCompare, const FilterModes& mode, const double& size)
{
	if(filterString.empty())
		return true;

	double entrySize = 0;
	string entryString = "";

	switch(sel) {
		case COLUMN_NAME: entryString = entry.getName(); doSizeCompare = false; break;
		case COLUMN_DESCRIPTION: entryString = entry.getDescription(); doSizeCompare = false; break;
		case COLUMN_USERS: entrySize = entry.getUsers(); break;
		case COLUMN_SERVER: entryString = entry.getServer(); doSizeCompare = false; break;
		case COLUMN_COUNTRY: entryString = entry.getCountry(); doSizeCompare = false; break;
		case COLUMN_SHARED: entrySize = (double)entry.getShared(); break;
		case COLUMN_MINSHARE: entrySize = (double)entry.getMinShare(); break;
		case COLUMN_MINSLOTS: entrySize = entry.getMinSlots(); break;
		case COLUMN_MAXHUBS: entrySize = entry.getMaxHubs(); break;
		case COLUMN_MAXUSERS: entrySize = entry.getMaxUsers(); break;
		case COLUMN_RELIABILITY: entrySize = entry.getReliability(); break;
		case COLUMN_RATING: entryString = entry.getRating(); doSizeCompare = false; break;
		default: break;
	}

	bool insert = false;
	if(doSizeCompare) {
		switch(mode) {
			case EQUAL: insert = (size == entrySize); break;
			case GREATER_EQUAL: insert = (size <= entrySize); break;
			case LESS_EQUAL: insert = (size >= entrySize); break;
			case GREATER: insert = (size < entrySize); break;
			case LESS: insert = (size > entrySize); break;
			case NOT_EQUAL: insert = (size != entrySize); break;
			case NONE: ; break;
		}
	} else {
		if(sel >= COLUMN_LAST) {
			if( Util::findSubString(entry.getName(), filterString) != string::npos ||
				Util::findSubString(entry.getDescription(), filterString) != string::npos ||
				Util::findSubString(entry.getServer(), filterString) != string::npos ||
				Util::findSubString(entry.getCountry(), filterString) != string::npos ||
				Util::findSubString(entry.getRating(), filterString) != string::npos ) {
					insert = true;
				}
		}
		if(Util::findSubString(entryString, filterString) != string::npos)
			insert = true;
	}

	return insert;
}

void PublicHubWindow::slotRefresh()
{
	setStatus(STATUS_STATUS, T_("Downloading public hub list..."));
	FavoriteManager::getInstance()->refresh(true);
	updateDropDown();
}

void PublicHubWindow::slotConf()
{
	new ConfPubHubDlg(this);
	updateDropDown();
}

void PublicHubWindow::slotListSelChanged(int x)
{
	FavoriteManager::getInstance()->setHubList(x);
	entries = FavoriteManager::getInstance()->getPublicHubs();
	updateList();
}

void PublicHubWindow::slotConnect()
{
	if(!checkNick()) return;
	if (!hubs->currentItem()) return;
	
	int idx = datalistitem.indexOf(hubs->currentItem());
	tstring t = Text::toT(datalist.at(idx)->entry->getServer());
	MainWindowImpl::getInstance()->OpenHub(t);
}

void PublicHubWindow::slotAddtoFav()
{
	if(!checkNick()) return;
	
	QList<QTreeWidgetItem *> lt = hubs->selectedItems();
	int idx;
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		idx = datalistitem.indexOf(lt.at(i));
		FavoriteManager::getInstance()->addFavorite(*datalist.at(idx)->entry);
	}
}

void PublicHubWindow::slotCopyHub()
{
	if (hubs->currentItem())
	{
		int idx = datalistitem.indexOf(hubs->currentItem());
		StilUtils::copy2Clipboard(Text::toT(datalist.at(idx)->entry->getServer()));
	}
}

bool PublicHubWindow::checkNick()
{
	if(SETTING(NICK).empty()) 
	{
		QMessageBox::critical(this, tr("StilDC++"), StilUtils::TstrtoQ(T_("Please enter a nickname in the settings dialog!")));
		return false;
	}
	return true;
}

void PublicHubWindow::slotOpen(QTreeWidgetItem * item, int)
{
	// On double-click
	if(!checkNick()) return;
	
	int idx = datalistitem.indexOf(item);
	tstring t = Text::toT(datalist.at(idx)->entry->getServer());
	MainWindowImpl::getInstance()->OpenHub(t);
}

void PublicHubWindow::slotKeyPressed(QKeyEvent* e)
{
	if ( (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) )
	{
		if (hubs->currentItem()) slotOpen(hubs->currentItem(), 0);
	}
}

void PublicHubWindow::slotFilterReturn()
{
	filterString = Text::fromT(StilUtils::QtoTstr(filter->text()));
	updateList();
}
