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

#include "SearchWin.h"
#include "mainwindowimpl.h"

int SearchWindow::columnSizes[] = { 200, 100, 50, 80, 100, 40, 100, 150, 80, 100, 125, 125 };

TStringList SearchWindow::lastSearches;

static QIcon folderIcon;
static QIcon fileIcon;

SearchWindow::SearchWindow(QWidget *parent, const tstring& initialString_, int64_t initialSize_, SearchManager::SizeModes initialMode_, SearchManager::TypeModes initialType_) : MdiChild(parent),
isHash(false),
onlyFree(BOOLSETTING(SEARCH_ONLY_FREE_SLOTS)),
initialString(initialString_),
initialSize(initialSize_),
initialMode(initialMode_),
initialType(initialType_)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_SEARCH;
	idText  = StilUtils::TstrtoQ(initialString);

	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
						QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
						QIcon::Normal, QIcon::On);
	fileIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

//	connect(searchButton, SIGNAL(pressed()), this, SLOT(SearchBtn()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchBtn()));
//	connect(purgeButton, SIGNAL(pressed()), this, SLOT(purgeClicked()));
	connect(purgeButton, SIGNAL(clicked()), this, SLOT(purgeClicked()));
	connect(hideZeroSlots, SIGNAL(stateChanged (int)), this, SLOT(zeroStatusChanged(int)));
	
	connect(this, SIGNAL(speakerSignal(unsigned int, long)), this, SLOT(handleSpeaker(unsigned int, long)),Qt::QueuedConnection);
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));

	for(TStringIter i = lastSearches.begin(); i != lastSearches.end(); ++i)
		searchCombo->insertItem(0, StilUtils::TstrtoQ(*i));
	
	sizeTypeCombo->setCurrentIndex((initialSize == 0) ? 2 : 0);
	hideZeroSlots->setCheckState(onlyFree ? Qt::Checked : Qt::Unchecked);
	
	//hubs = new HubList();
	initHubs();
	initSearchList();
	readSettings();
	initSearchMenu();
	initColumnMenu();
	
	ClientManager* clientMgr = ClientManager::getInstance();
	clientMgr->lock();
	clientMgr->addListener(this);
	Client::List& clients = clientMgr->getClients();
	for(Client::List::iterator it = clients.begin(); it != clients.end(); ++it) {
		Client* client = *it;
		if(!client->isConnected())
			continue;

		onHubAdded(new HubInfo(Text::toT(client->getHubUrl()), Text::toT(client->getHubName()), client->getMyIdentity().isOp()));
	}
	clientMgr->unlock();

	SearchManager::getInstance()->addListener(this);

	if(!initialString.empty())
	{
		lastSearches.push_back(initialString);
		searchCombo->insertItem(0, StilUtils::TstrtoQ(initialString));
		searchCombo->setCurrentIndex(0);
		sizeModeCombo->setCurrentIndex(initialMode);
		sizeLineEdit->setText(StilUtils::TstrtoQ(Text::toT(Util::toString(initialSize))));
		typeCombo->setCurrentIndex(initialType);
		runSearch();
	} 
	else
	{
		sizeModeCombo->setCurrentIndex(1);
		typeCombo->setCurrentIndex(SETTING(LAST_SEARCH_TYPE));
	}
//	searchBox->setFocus();
}

void SearchWindow::zeroStatusChanged(int state)
{
	if (state == Qt::Checked)
		onlyFree = true;
	else
		if (state ==  Qt::Unchecked)
			onlyFree = false;
}

//update hulist, if op checkbox changed

void SearchWindow::hubStateChanged(QListWidgetItem* currentItem)
{
	if (hubListWidget->row(currentItem) == 0)
	{
		for(int i = 1; i<hubListWidget->count(); i++)
		{
			QListWidgetItem* item = hubListWidget->item(i);
			HubInfo* client = hubMap.key(item);

			if (!client->op)
				item->setCheckState(Qt::Unchecked);
		}
	}
}


SearchWindow::~SearchWindow()
{
	saveSettings();
	
	delete proxyModel;
	delete model;
	delete timer;
	
	SearchManager::getInstance()->removeListener(this);
	ClientManager::getInstance()->removeListener(this);

//	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_ORDER, WinUtil::toString(results->getColumnOrder()));
//	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_WIDTHS, WinUtil::toString(results->getColumnWidths()));

}

void SearchWindow::initSecond()
{
/*	if (timer != NULL)
	{
		disconnect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
		delete timer;
	}
*/
	timer->start(1000);
}

bool SearchWindow::eachSecond()
{
	int32_t waitFor = SearchManager::getInstance()->timeToSearch();

	if(waitFor > 0) {
		QString msg = QString(tr("Searching too soon, next search in %1 second(s)")).arg(QString::number(waitFor));
		setStatus(STATUS_STATUS, msg);
		setText(QString(tr("Search - %1")).arg(msg));
		return true;
	} 
	
	setStatus(STATUS_STATUS, tr("Ready to search..."));
	setText(tr("Search - Ready to search..."));
	
	timer->stop();
	
	return false;
}

void SearchWindow::initHubs()
{
	hubMap.clear();
	QListWidgetItem *opsItem = new QListWidgetItem(StilUtils::TstrtoQ(T_("Only where I'm op")), hubListWidget);
	opsItem->setCheckState(Qt::Unchecked);
	connect(hubListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(hubStateChanged(QListWidgetItem*)));
}

void SearchWindow::initSearchList()
{
	searchMap.clear();
	
	QStringList columns;
	columns << tr("File") << tr("User") << tr("Type") << tr("Size") << tr("Path")
			<< tr("Slots") << tr("Connection") << tr("Hub") << tr("Exact size") << tr("IP")
			<< tr("TTH Root") << tr("CID") << tr("Hidden size");

	int totalColumns =  columns.size();
	
	model = new QStandardItemModel(0, totalColumns);
	
	for (int i=0;i<totalColumns;i++)
		model->setHeaderData(i, Qt::Horizontal, columns.at(i));

	searchView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	proxyModel = new searchSortingModel();
	
	proxyModel->setDynamicSortFilter(true);
	proxyModel->setSourceModel(model);

	searchView->setModel(proxyModel);

	//proxyModel -> setSortCaseSensitivity(Qt::CaseInsensitive);
	searchView->setSortingEnabled(true);
	searchView->sortByColumn(0, Qt::AscendingOrder);

	searchView->setRootIsDecorated(false);
	searchView->setAlternatingRowColors(true);

	//hide size of item (without units of measurement)
	searchView->hideColumn(totalColumns - 1);
}


void SearchWindow::insertSearchResult(SearchInfo* si)
{
	QStandardItem *parentItem = model->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStringList::const_iterator constIterator;

	QStandardItem *item;
    
	//also fill hidden column
    for (int i = 0; i<COLUMN_LAST+1;i++)
    {
    	item = new QStandardItem(StilUtils::TstrtoQ(si->getText(i)));
    	
    	if (i == 0)
		{
			item->setIcon(*(si->getImage()));
			searchMap.insert(item, si);
		}

    	itemList << item;
    }

    parentItem->appendRow(itemList);

}

void SearchWindow::removeSearchResult(QStandardItem* item)
{
	QStandardItem *parentItem = model->invisibleRootItem();
	
	Q_ASSERT(searchMap.remove(item) > 0);

	parentItem->removeRow(item->row());
	
}

void SearchWindow::clearSearchResult()
{
	model->removeRows(0, model->rowCount());
	searchMap.clear();
}

void SearchWindow::runSearch()
{
	StringList clients;

	// Change Default Settings If Changed
	if (onlyFree != BOOLSETTING(SEARCH_ONLY_FREE_SLOTS))
		SettingsManager::getInstance()->set(SettingsManager::SEARCH_ONLY_FREE_SLOTS, onlyFree);
	if (!initialType && typeCombo->currentIndex() != SETTING(LAST_SEARCH_TYPE))
		SettingsManager::getInstance()->set(SettingsManager::LAST_SEARCH_TYPE, typeCombo->currentIndex());

	tstring s = StilUtils::QtoTstr(searchCombo->currentText());

	if(s.empty())
		return;
	/*
	int n = hubListWidget->count();
	for(int i = 0; i < n; i++) {
		if(hubs->isChecked(i)) {
			clients.push_back(Text::fromT(hubs->getData(i)->url));
		}
	}
	*/

	QMap<HubInfo*, QListWidgetItem*>::const_iterator i = hubMap.constBegin();
	
	while (i != hubMap.constEnd())
	{
		if (i.value()->checkState() == Qt::Checked)
			clients.push_back(Text::fromT(i.key()->url));

		++i;
	}

	if(clients.empty())
		return;

	tstring tsize = StilUtils::QtoTstr(sizeLineEdit->text());
	
	double lsize = Util::toDouble(Text::fromT(tsize));
	switch(sizeTypeCombo->currentIndex()) {
	case 1:
		lsize*=1024.0; break;
	case 2:
		lsize*=1024.0*1024.0; break;
	case 3:
		lsize*=1024.0*1024.0*1024.0; break;
	}

	int64_t llsize = (int64_t)lsize;

	clearSearchResult();

	{
		Lock l(cs);
		currentSearch = StringTokenizer<tstring>(s, ' ').getTokens();
		s.clear();
		//strip out terms beginning with -
		for(TStringList::iterator si = currentSearch.begin(); si != currentSearch.end(); )
		{
			if(si->empty()) 
			{
				si = currentSearch.erase(si);
				continue;
			}
			if ((*si)[0] != _T('-')) 
				s += *si + _T(' ');	
			++si;
		}

		s = s.substr(0, max(s.size(), static_cast<tstring::size_type>(1)) - 1);
		token = Util::toString(Util::rand());
	}


	SearchManager::SizeModes searchMode((SearchManager::SizeModes)sizeModeCombo->currentIndex());
	if(llsize == 0)
		searchMode = SearchManager::SIZE_DONTCARE;

	int ftype = typeCombo->currentIndex();

	// Add new searches to the last-search dropdown list
	if(std::find(lastSearches.begin(), lastSearches.end(), s) == lastSearches.end())
	{
		int i = max(SETTING(SEARCH_HISTORY)-1, 0);

		if(searchCombo->count() > i)
			searchCombo->removeItem(i);
		searchCombo->insertItem(0, StilUtils::TstrtoQ(s));

		while(lastSearches.size() > (TStringList::size_type)i) {
			lastSearches.erase(lastSearches.begin());
		}
		lastSearches.push_back(s);
	}

	setStatus(STATUS_STATUS, T_("Search for ") + s + _T("..."));
	setStatus(STATUS_COUNT, Util::emptyStringT);
	setStatus(STATUS_FILTERED, Util::emptyStringT);
	droppedResults = 0;
	isHash = (ftype == SearchManager::TYPE_TTH);

	setText(T_( "Search") + _T(" - ") + s);

	if(SearchManager::getInstance()->okToSearch())
	{
		SearchManager::getInstance()->search(clients, Text::fromT(s), llsize, (SearchManager::TypeModes)ftype, searchMode, token);
/*		qDebug() << "llsize:" << llsize << endl << "ftype:" << ftype << endl << "mode:" << searchMode << endl << "token:" << StilUtils::TstrtoQ(Text::toT(token));*/
		if(BOOLSETTING(CLEAR_SEARCH)) // Only clear if the search was sent
			searchCombo->addItem(StilUtils::TstrtoQ(Util::emptyStringT));
	}
	else 
	{
		int32_t waitFor = SearchManager::getInstance()->timeToSearch();
		QString msg = QString(StilUtils::TstrtoQ(T_("Searching too soon, next search in %1 seconds"))).arg(waitFor);

		setStatus(STATUS_STATUS, msg);
		setStatus(STATUS_COUNT, Util::emptyStringT);
		setStatus(STATUS_FILTERED, Util::emptyStringT);

		setText(T_( "Search") + _T(" - ") + StilUtils::QtoTstr(msg));
		// Start the countdown timer
		initSecond();
	}
}

void SearchWindow::onHubAdded(HubInfo* info)
{
	//TODO: check for memory leaks
	QListWidgetItem *item = new QListWidgetItem(StilUtils::TstrtoQ(info->getText()), hubListWidget);

	if (item->checkState() == Qt::Checked)
	{
		if (!info->op)
			item->setCheckState(Qt::Unchecked);
	}
	else
		item->setCheckState(Qt::Checked);

	hubMap.insert(info, item);
}

void SearchWindow::setStatus(int s, const tstring& text)
{
	switch(s)
	{
		case STATUS_STATUS:
			searchStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_COUNT:
			countStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_FILTERED:
			filteredStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		default:
			dcdebug("Set status for %d with text: %s\n", s, Text::fromT(text).c_str());
	}
}

void SearchWindow::setStatus(int s, const QString& text)
{
	switch(s)
	{
		case STATUS_STATUS:
			searchStatusLabel -> setText(text);
			break;
		case STATUS_COUNT:
			countStatusLabel -> setText(text);
			break;
		case STATUS_FILTERED:
			filteredStatusLabel -> setText(text);
			break;
		default:
			dcdebug("Set status for %d with text: %s\n", s, Text::fromT((StilUtils::QtoTstr(text))).c_str());
	}
}

void SearchWindow::setText(const tstring& text)
{
	setTabText(StilUtils::TstrtoQ(text));
}

void SearchWindow::setText(const QString& text)
{
	setTabText(text);
}

void SearchWindow::onHubChanged(HubInfo* info)
{
	QMap<HubInfo*, QListWidgetItem*>::const_iterator i = hubMap.constBegin();
	
	while (i != hubMap.constEnd())
	{
		if (i.key()->url == info->url)
			break;

		++i;
	}

	if (i == hubMap.constEnd()) return;
	
	QListWidgetItem *item = i.value();
	
	item->setText(StilUtils::TstrtoQ(info->getText()));

	if  (item->checkState() == Qt::Checked)
		item->setCheckState(info->op ? Qt::Checked : Qt::Unchecked);
	
	//hubs = new HubList();
	initHubs();
	
}

void SearchWindow::onHubRemoved(HubInfo* info)
{
	QMap<HubInfo*, QListWidgetItem*>::const_iterator i = hubMap.constBegin();
	
	while (i != hubMap.constEnd())
	{
		if (i.key()->url == info->url)
			break;

		++i;
	}

	if (i == hubMap.constEnd()) return;
	
	QListWidgetItem *item = i.value();
	hubListWidget->removeItemWidget(item);
	
	hubMap.remove(i.key());
}

void SearchWindow::SearchBtn()
{
	searchButton->setDefault(true);
/*	if (searchCombo->currentIndex()>=0)
	{
		idText  = searchCombo->itemText(searchCombo->currentIndex());	
		setTabText(tr("Search for: ")+idText);*/
		runSearch();
		//SEARCH
//		}
}

void SearchWindow::purgeClicked()
{
	purgeButton->setDefault(true);
	searchCombo->clear();
	lastSearches.clear();
}

void SearchWindow::on(SearchManagerListener::SR, SearchResult* aResult) throw() {
	// Check that this is really a relevant search result...
	{
		Lock l(cs);

		if(currentSearch.empty())
		//if(searchCombo->currentIndex()<0)
		{
			return;
		}
		
		if(!aResult->getToken().empty() && token != aResult->getToken()) {
			droppedResults++;
			speak(SPEAK_FILTER_RESULT);
			return;
		}

		if(isHash) {
			if(aResult->getType() != SearchResult::TYPE_FILE || TTHValue(Text::fromT(currentSearch[0])) != aResult->getTTH()) {
				droppedResults++;
				speak(SPEAK_FILTER_RESULT);
				return;
			}
		} else {
			// match all here
			for(TStringIter j = currentSearch.begin(); j != currentSearch.end(); ++j) {
				if((*j->begin() != _T('-') && Util::findSubString(aResult->getFile(), Text::fromT(*j)) == -1) ||
					(*j->begin() == _T('-') && j->size() != 1 && Util::findSubString(aResult->getFile(), Text::fromT(j->substr(1))) != -1)
					)
				{
					droppedResults++;
					speak(SPEAK_FILTER_RESULT);
					return;
				}
			}
		}
	}

	// Reject results without free slots
	if((onlyFree && aResult->getFreeSlots() < 1))
	{
		droppedResults++;
		speak(SPEAK_FILTER_RESULT);
		return;
	}

	SearchInfo* i = new SearchInfo(aResult);
	speak(SPEAK_ADD_RESULT, reinterpret_cast<unsigned int>(i));
}

int SearchWindow::handleSpeaker(unsigned int wParam, long lParam)
{
	switch(wParam) 
	{
	case SPEAK_ADD_RESULT:
		{
			SearchInfo* si = reinterpret_cast<SearchInfo*>(lParam);
			SearchResult* sr = si->sr;

			// Check previous search results for dupes
			int i, j;
			QMap< QStandardItem*, SearchInfo*>::const_iterator iter;
			
			for(iter = searchMap.constBegin(), i = 0, j = model->rowCount(); i < j; ++i, ++iter) 
			{
				SearchInfo* si2 = iter.value();
				SearchResult* sr2 = si2->sr;
				if((sr->getUser()->getCID() == sr2->getUser()->getCID()) && (sr->getFile() == sr2->getFile()))
				{
					delete si;
					return 0;
				}
			}

			insertSearchResult(si);
			setStatus(STATUS_COUNT, QString(tr("%1 item(s)")).arg(model->rowCount()));
//			setDirty(SettingsManager::BOLD_SEARCH);
		}
		break;
	case SPEAK_FILTER_RESULT:
		setStatus(STATUS_FILTERED, QString(tr("%1 filtered")).arg(droppedResults));
		break;
	case SPEAK_HUB_ADDED:
		onHubAdded(reinterpret_cast<HubInfo*>(lParam));
		break;
	case SPEAK_HUB_CHANGED:
		onHubChanged(reinterpret_cast<HubInfo*>(lParam));
		break;
	case SPEAK_HUB_REMOVED:
 		onHubRemoved(reinterpret_cast<HubInfo*>(lParam));
		break;
 	}
	return 0;
}

void SearchWindow::SearchInfo::update()
{
	if(sr->getType() == SearchResult::TYPE_FILE)
	{//PORT_ME
		string file_sep = StilUtils::linuxSeparator(sr->getFile());

		if(file_sep.rfind(_T('/')) == tstring::npos)
      /////////////
		{
			columns[COLUMN_FILENAME] = Text::toT(file_sep);
		} 
		else 
		{
			columns[COLUMN_FILENAME] = Text::toT(Util::getFileName(file_sep));
			columns[COLUMN_PATH] = Text::toT(Util::getFilePath(file_sep));
		}

		columns[COLUMN_TYPE] = Text::toT(Util::getFileExt(Text::fromT(columns[COLUMN_FILENAME])));

		if(!columns[COLUMN_TYPE].empty() && columns[COLUMN_TYPE][0] == _T('.'))
			columns[COLUMN_TYPE].erase(0, 1);

		columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(sr->getSize()));
		columns[COLUMN_EXACT_SIZE] = Text::toT(Util::formatExactSize(sr->getSize()));
		//hidden column
		columns[COLUMN_LAST] = Text::toT(Util::toString(sr->getSize()));
} 
	else 
	{//PORT_ME
		columns[COLUMN_FILENAME] = Text::toT(StilUtils::linuxSeparator(sr->getFileName()));
		columns[COLUMN_PATH] = Text::toT(StilUtils::linuxSeparator(sr->getFile()));
     //////////
		columns[COLUMN_TYPE] = StilUtils::QtoTstr(tr("Directory"));
		if(sr->getSize() > 0)
		{
			columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(sr->getSize()));
			columns[COLUMN_EXACT_SIZE] = Text::toT(Util::formatExactSize(sr->getSize()));
			//hidden column
			columns[COLUMN_LAST] = Text::toT(Util::toString(sr->getSize()));
		}
	}
	columns[COLUMN_NICK] = StilUtils::getNicks(sr->getUser());
	columns[COLUMN_CONNECTION] = Text::toT(ClientManager::getInstance()->getConnection(sr->getUser()->getCID()));
	columns[COLUMN_HUB] = Text::toT(sr->getHubName());
	columns[COLUMN_SLOTS] = Text::toT(sr->getSlotString());
	columns[COLUMN_IP] = Text::toT(sr->getIP());
	if (!columns[COLUMN_IP].empty())
	{
		// Only attempt to grab a country mapping if we actually have an IP address
		tstring tmpCountry = Text::toT(Util::getIpCountry(sr->getIP()));
		if(!tmpCountry.empty())
			columns[COLUMN_IP] = tmpCountry + _T(" (") + columns[COLUMN_IP] + _T(")");
	}

	if(sr->getType() == SearchResult::TYPE_FILE)
	{
        columns[COLUMN_TTH] = Text::toT(sr->getTTH().toBase32());
	}
	columns[COLUMN_CID] = Text::toT(sr->getUser()->getCID().toBase32());

}

QIcon* SearchWindow::SearchInfo::getImage() const
{
	return sr->getType() == SearchResult::TYPE_FILE ? &fileIcon : &folderIcon;
}

void SearchWindow::SearchInfo::view()
{
	try {
		if(sr->getType() == SearchResult::TYPE_FILE)
		{
			QueueManager::getInstance()->add(Util::getTempPath() + sr->getFileName(), sr->getSize(), 
											 sr->getTTH(), sr->getUser(), QueueItem::FLAG_CLIENT_VIEW | QueueItem::FLAG_TEXT);
		}
	} catch(const Exception&) {}
}

void SearchWindow::SearchInfo::Download(const tstring& tgt) 
{
	try {
		if(sr->getType() == SearchResult::TYPE_FILE)
		{
			string target = Text::fromT(tgt + columns[COLUMN_FILENAME]);
			QueueManager::getInstance()->add(target, sr->getSize(),
											 sr->getTTH(), sr->getUser());
/*
			if(WinUtil::isShift())
				QueueManager::getInstance()->setPriority(target, QueueItem::HIGHEST);
*/
		} 
		else 
		{
			/*
			QueueManager::getInstance()->addDirectory(si->sr->getFile(), si->sr->getUser(), Text::fromT(tgt),
				WinUtil::isShift() ? QueueItem::HIGHEST : QueueItem::DEFAULT);
			*/
			QueueManager::getInstance()->addDirectory(sr->getFile(), sr->getUser(), 
													  Text::fromT(tgt), QueueItem::DEFAULT);
		}
	} catch(const Exception&) {
	}
}

void SearchWindow::SearchInfo::DownloadWhole(const tstring& tgt)
{
	try {
		//QueueItem::Priority prio = WinUtil::isShift() ? QueueItem::HIGHEST : QueueItem::DEFAULT;
		QueueItem::Priority prio = QueueItem::DEFAULT;
		if(sr->getType() == SearchResult::TYPE_FILE) 
		{
			QueueManager::getInstance()->addDirectory(StilUtils::windowsSeparator(Text::fromT(columns[COLUMN_PATH])), 
				sr->getUser(), Text::fromT(tgt), prio);
		} 
		else
		{
			QueueManager::getInstance()->addDirectory(sr->getFile(), sr->getUser(), Text::fromT(tgt), prio);
		}
	} catch(const Exception&) {}
}

void SearchWindow::speak(unsigned int wParam, long lParam)
{
	emit speakerSignal(wParam, lParam);
}

void SearchWindow::speak(Speakers s, Client* aClient)
{
	HubInfo* hubInfo = new HubInfo(Text::toT(aClient->getHubUrl()), Text::toT(aClient->getHubName()), aClient->getMyIdentity().isOp());
	speak(s, reinterpret_cast<unsigned int>(hubInfo));
}

void SearchWindow::initColumnMenu()
{
	columnMenu = new QMenu(searchView->header());

	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));

	searchView->header()->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(searchView->header(), SIGNAL(customContextMenuRequested(const QPoint&)), 
				this, SLOT(showColumnMenu(const QPoint&)));
}

void SearchWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0)
		searchView->hideColumn(-index-1);
	else
		searchView->showColumn(index);
}

void SearchWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columnMenu);
		action->setText(proxyModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
		
		bool isHidden = searchView->header()->isSectionHidden(i);

		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
			
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(searchView->header()->mapToGlobal(point));	
}

void SearchWindow::initSearchMenu()
{
	userMenu = new QMenu(searchView);
	searchView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(searchView, SIGNAL(customContextMenuRequested(const QPoint&)), 
				this, SLOT(showSearchMenu(const QPoint&)));

	//connect(treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	//		this, SLOT(updateActions(const QItemSelection &, const QItemSelection &)));

	userMenu->setDefaultAction(userMenu->addAction(tr("Download"), this, SLOT(actionDownload())));

	userMenu->addAction(tr("Download to..."), this, SLOT(actionDownloadTo()));
	userMenu->addAction(tr("Download whole directory"), this, SLOT(actionDownloadWholeDir()));
	userMenu->addAction(tr("Download whole directory to..."), this, SLOT(actionDownloadWholeDirTo()));
	userMenu->addAction(tr("View as text"), this, SLOT(actionViewAsText()));

	userMenu->addSeparator();

	fantomActions.append(userMenu->addAction(tr("Search for alternates"), this, SLOT(actionSearchAlt())));
	fantomActions.append(userMenu->addAction(tr("Lookup TTH at Bitzi.com"), this, SLOT(actionBitzi())));
	fantomActions.append(userMenu->addAction(tr("Copy magnet link to clipboard"), this, SLOT(actionCopyMagnetToClipboard())));

	userMenu->addSeparator();

	userMenu->addAction(tr("Get file list"), this, SLOT(actionGetFilelist()));
	userMenu->addAction(tr("Match queue"), this, SLOT(actionMatchQueue()));
	userMenu->addAction(tr("Send private message"), this, SLOT(actionSendPM()));
	userMenu->addAction(tr("Add to favorites"), this, SLOT(actionAddToFavorites()));
	userMenu->addAction(tr("Grand extra slot"), this, SLOT(actionGrandExtraSlot()));

	userMenu->addSeparator();

	userMenu->addAction(tr("Remove user from queue"), this, SLOT(actionRemoveUserFromQueue()));

	userMenu->addSeparator();

	userMenu->addAction(tr("Remove"), this, SLOT(actionRemove()));
}

void SearchWindow::showSearchMenu(const QPoint &point)
{
	if (searchView->indexAt(point).isValid())
	{
		foreach (QAction *action, fantomActions)
			action->setVisible(searchView->selectionModel()->selectedRows().size() <= 1 ? true : false);

		userMenu->exec(searchView->mapToGlobal(point));
	}
}

void SearchWindow::actionDownload()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		searchMap.value(item)->Download(Text::toT(SETTING(DOWNLOAD_DIRECTORY)));
	}
}

void SearchWindow::actionDownloadTo()
{
}

void SearchWindow::actionDownloadWholeDir()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));
		
		searchMap.value(item)->DownloadWhole(Text::toT(SETTING(DOWNLOAD_DIRECTORY)));
	}
}

void SearchWindow::actionDownloadWholeDirTo()
{
}

void SearchWindow::actionViewAsText()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));
		
		searchMap.value(item)->view();                                              ;
	}
}

void SearchWindow::actionSearchAlt()
{
	const QModelIndex index = proxyModel->mapToSource(searchView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = model->itemFromIndex(index);

	const tstring tth = searchMap.value(item)->columns[COLUMN_TTH];

	if (tth != Util::emptyStringT)
		MainWindowImpl::getInstance()->OpenSearch(tth, 0, SearchManager::SIZE_DONTCARE, SearchManager::TYPE_TTH);
}

void SearchWindow::actionBitzi()
{
	const QModelIndex index = proxyModel->mapToSource(searchView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = model->itemFromIndex(index);

	const tstring tth = searchMap.value(item)->columns[COLUMN_TTH];

	if (tth != Util::emptyStringT)
	{
		QUrl url = QUrl("http://bitzi.com/lookup/tree:tiger:" + StilUtils::TstrtoQ(tth));
		QDesktopServices::openUrl(url);
	}
}

void SearchWindow::actionCopyMagnetToClipboard()
{
	const QModelIndex index = proxyModel->mapToSource(searchView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = model->itemFromIndex(index);

	const tstring tth = searchMap.value(item)->columns[COLUMN_TTH];
	const tstring fileName = searchMap.value(item)->columns[COLUMN_FILENAME];

	if(!fileName.empty())
	{
		QString magnet = StilUtils::TstrtoQ(_T("magnet:?xt=urn:tree:tiger:") + tth + _T("&dn=") +Text::toT(Util::encodeURI(Text::fromT(fileName))));
		StilUtils::copy2Clipboard(magnet);
	}
}

void SearchWindow::actionGetFilelist()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		try {
			QueueManager::getInstance()->addList(searchMap.value(item)->getUser(), QueueItem::FLAG_CLIENT_VIEW);
		} catch(const Exception& e) {
			LogManager::getInstance()->message(e.getError());
		}
	}
}

void SearchWindow::actionMatchQueue()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));
		
		try {
			QueueManager::getInstance()->addList(searchMap.value(item)->getUser(), QueueItem::FLAG_MATCH_QUEUE);
		} catch(const Exception& e) {
			LogManager::getInstance()->message(e.getError());
		}
	}
}

void SearchWindow::actionSendPM()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		MainWindowImpl::getInstance()->OpenPM(searchMap.value(item)->getUser());
	}
}

void SearchWindow::actionAddToFavorites()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		FavoriteManager::getInstance()->addFavoriteUser(searchMap.value(item)->getUser());
	}
}

void SearchWindow::actionGrandExtraSlot()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		UploadManager::getInstance()->reserveSlot(searchMap.value(item)->getUser());
	}
}

void SearchWindow::actionRemoveUserFromQueue()
{
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QStandardItem *item = model->itemFromIndex(proxyModel->mapToSource(*index));

		QueueManager::getInstance()->removeSource(searchMap.value(item)->getUser(), QueueItem::Source::FLAG_REMOVED);
	}
}

void SearchWindow::actionRemove()
{
	QList<QStandardItem *> items;
	
	const QModelIndexList indexes = searchView->selectionModel()->selectedRows();

	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
		items.append(model->itemFromIndex(proxyModel->mapToSource(*index)));
		
	for (QList<QStandardItem *>::const_iterator item = items.constBegin(); item != items.constEnd(); ++item)
		removeSearchResult(*item);
}

void SearchWindow::saveSettings()
{
	QStringList visibilityList, widthList, orderList;

	for (int i=0; i<COLUMN_LAST; i++)
	{
		visibilityList << QString::number(searchView->header()->isSectionHidden(i));
		widthList << QString::number(searchView->columnWidth(i));
		orderList << QString::number(searchView->header()->visualIndex(i));
	}
	
	SETAPPSTRING(s_SEARCHWINDOW_COLUMN_VISIBILITY, visibilityList.join(","));
	
	QString width = widthList.join(",");
	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_WIDTHS, width.toStdString());

	QString order = orderList.join(",");
	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_ORDER, order.toStdString());
/*
		// CONNECTIONS
	// visibility
	QStringList vv;
	for (int i=0; i<COLUMN_LAST; i++) 
		vv << QString::number(searchView->header()->isSectionHidden(i));
	SETAPPSTRING(s_SEARCHWINDOW_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) 
		w << QString::number(searchView->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i=0; i<COLUMN_LAST; i++) 
		ww << QString::number(searchView->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::SEARCHFRAME_ORDER, ww.join(",").toStdString());
	
	qDebug() << vv.join(",") << endl << w.join(",") << endl << ww.join(",");
	
	//AppSettings::AppSettingsMgr::getInstance()->save();
	//SettingsManager::getInstance()->save();
	*/
}

void SearchWindow::readSettings()
{
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(SEARCHFRAME_WIDTHS))).split(",");

	if (clist.size() != COLUMN_LAST)
	{
		for (int i=0; i<COLUMN_LAST; i++)
			searchView->setColumnWidth(i, columnSizes[i]);
	}
	else 
	{
		for (int i=0; i<COLUMN_LAST; i++)
		{
			if ( clist.at(i).isEmpty() || (clist.at(i).toInt() <= 0))
				searchView->setColumnWidth(i, columnSizes[i]);
			else 
				searchView->setColumnWidth(i, clist.at(i).toInt());
		}
	}
	
	// SETTING COLUMNS VISIBILITY
	QStringList vlist = APPSTRING(s_SEARCHWINDOW_COLUMN_VISIBILITY).split(",");

	if (vlist.size() == COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) 
			searchView->header()->setSectionHidden(i, vlist.at(i).toInt());
			
	// COLUMNS ORDER SET
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(SEARCHFRAME_ORDER))).split(",");

	if (olist.size() == COLUMN_LAST)
		for (int j=0; j<COLUMN_LAST; j++) 
			searchView->header()->swapSections(searchView->header()->visualIndex(olist.at(j).toInt()), j);
}

searchSortingModel::searchSortingModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool searchSortingModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
	QString leftString = sourceModel()->data(left).toString();
	QString rightString = sourceModel()->data(right).toString();
	
	int col = left.column();
	
	int64_t leftSize = getSize(left);
	int64_t rightSize = getSize(right);
	
	if (col == SearchWindow::COLUMN_SIZE || col == SearchWindow::COLUMN_EXACT_SIZE)
		return leftSize < rightSize;
	
	return (QString::compare(leftString, rightString, Qt::CaseInsensitive) < 0);
}

int64_t searchSortingModel::getSize(const QModelIndex &index) const
{
	// read data from hidden column
	QString sizeString = sourceModel()->data(index.sibling(index.row(), SearchWindow::COLUMN_LAST)).toString();	
	return sizeString.toULongLong();
}
