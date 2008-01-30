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

#include "SearchWin.h"

TStringList SearchWindow::lastSearches;

SearchWindow::SearchWindow(QWidget *parent, const QString &what, const tstring& initialString_, int64_t initialSize_, SearchManager::SizeModes initialMode_, SearchManager::TypeModes initialType_) : MdiChild(parent),
isHash(false),
onlyFree(BOOLSETTING(SEARCH_ONLY_FREE_SLOTS)),
initialString(initialString_),
initialSize(initialSize_),
initialMode(initialMode_),
initialType(initialType_)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_SEARCH;
	idText  = what;
	setTabText(tr("Search for: ")+what);

	connect(searchButton, SIGNAL(pressed()), this, SLOT(SearchBtn()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchBtn()));
	connect(purgeButton, SIGNAL(pressed()), this, SLOT(purgeClicked()));
	connect(purgeButton, SIGNAL(clicked()), this, SLOT(purgeClicked()));
	connect(hideZeroSlots, SIGNAL(stateChanged (int)), this, SLOT(zeroStatusChanged(int)));
	
	connect(this, SIGNAL(speakerSignal(unsigned int, long)), this, SLOT(handleSpeaker(unsigned int, long)),Qt::QueuedConnection);

	for(TStringIter i = lastSearches.begin(); i != lastSearches.end(); ++i)
		searchCombo->insertItem(0, StilUtils::TstrtoQ(*i));
	
	sizeTypeCombo->setCurrentIndex((initialSize == 0) ? 2 : 0);
	hideZeroSlots->setCheckState(onlyFree ? Qt::Checked : Qt::Unchecked);
	
	//hubs = new HubList();
	initHubs();
	initSearchList();

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
	delete model;
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
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
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
			<< tr("TTH Root") << tr("CID");

	int totalColumns =  columns.size();
	
	model = new QStandardItemModel(0, totalColumns);
	
	for (int i=0;i<totalColumns;i++)
		model->setHeaderData(i, Qt::Horizontal, columns.at(i));

	searchView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	searchView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	searchView->setModel(model);
	//proxyModel -> setSortCaseSensitivity(Qt::CaseInsensitive);
	searchView->setSortingEnabled(true);
	searchView->sortByColumn(0, Qt::AscendingOrder);

	searchView->setRootIsDecorated(false);
	searchView->setAlternatingRowColors(true);
	
}


void SearchWindow::insertSearchResult(SearchInfo* si)
{
	QStandardItem *parentItem = model->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStringList::const_iterator constIterator;

	QStandardItem *item;
    
    for (int i = 0; i<COLUMN_LAST;i++)
    {
    	item = new QStandardItem(StilUtils::TstrtoQ(si->getText(i)));
    	
    	if (i == 0)
		{
//			item->setIcon(*icon);
			searchMap.insert(si, item);
		}

    	itemList << item;
    }

    parentItem->appendRow(itemList);

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
	switch(sizeModeCombo->currentIndex()) {
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


	SearchManager::SizeModes mode((SearchManager::SizeModes)sizeTypeCombo->currentIndex());
	if(llsize == 0)
		mode = SearchManager::SIZE_DONTCARE;

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

	setStatus(STATUS_STATUS, T_("Search for") + s + _T("..."));
	setStatus(STATUS_COUNT, Util::emptyStringT);
	setStatus(STATUS_FILTERED, Util::emptyStringT);
	droppedResults = 0;
	isHash = (ftype == SearchManager::TYPE_TTH);

	setText(T_( "Search") + _T(" - ") + s);

	if(SearchManager::getInstance()->okToSearch()) {
		SearchManager::getInstance()->search(clients, Text::fromT(s), llsize,
			(SearchManager::TypeModes)ftype, mode, token);
		if(BOOLSETTING(CLEAR_SEARCH)) // Only clear if the search was sent
			searchCombo->addItem(StilUtils::TstrtoQ(Util::emptyStringT));
	} else {
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
			QMap<SearchInfo*, QStandardItem*>::const_iterator iter;
			
			for(iter = searchMap.constBegin(), i = 0, j = model->rowCount(); i < j; ++i, ++iter) 
			{
				SearchInfo* si2 = iter.key();
				SearchResult* sr2 = si2->sr;
				if((sr->getUser()->getCID() == sr2->getUser()->getCID()) && (sr->getFile() == sr2->getFile())) {
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

void SearchWindow::SearchInfo::update() {
	if(sr->getType() == SearchResult::TYPE_FILE) {
		if(sr->getFile().rfind(_T('\\')) == tstring::npos) {
			columns[COLUMN_FILENAME] = Text::toT(sr->getFile());
		} else {
			columns[COLUMN_FILENAME] = Text::toT(Util::getFileName(sr->getFile()));
			columns[COLUMN_PATH] = Text::toT(Util::getFilePath(sr->getFile()));
		}

		columns[COLUMN_TYPE] = Text::toT(Util::getFileExt(Text::fromT(columns[COLUMN_FILENAME])));
		if(!columns[COLUMN_TYPE].empty() && columns[COLUMN_TYPE][0] == _T('.'))
			columns[COLUMN_TYPE].erase(0, 1);
		columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(sr->getSize()));
		columns[COLUMN_EXACT_SIZE] = Text::toT(Util::formatExactSize(sr->getSize()));
	} else {
		columns[COLUMN_FILENAME] = Text::toT(sr->getFileName());
		columns[COLUMN_PATH] = Text::toT(sr->getFile());
		columns[COLUMN_TYPE] = StilUtils::QtoTstr(tr("Directory"));
		if(sr->getSize() > 0) {
			columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(sr->getSize()));
			columns[COLUMN_EXACT_SIZE] = Text::toT(Util::formatExactSize(sr->getSize()));
		}
	}
	columns[COLUMN_NICK] = StilUtils::getNicks(sr->getUser());
	columns[COLUMN_CONNECTION] = Text::toT(ClientManager::getInstance()->getConnection(sr->getUser()->getCID()));
	columns[COLUMN_HUB] = Text::toT(sr->getHubName());
	columns[COLUMN_SLOTS] = Text::toT(sr->getSlotString());
	columns[COLUMN_IP] = Text::toT(sr->getIP());
	if (!columns[COLUMN_IP].empty()) {
		// Only attempt to grab a country mapping if we actually have an IP address
		tstring tmpCountry = Text::toT(Util::getIpCountry(sr->getIP()));
		if(!tmpCountry.empty())
			columns[COLUMN_IP] = tmpCountry + _T(" (") + columns[COLUMN_IP] + _T(")");
	}
	if(sr->getType() == SearchResult::TYPE_FILE) {
        columns[COLUMN_TTH] = Text::toT(sr->getTTH().toBase32());
	}
	columns[COLUMN_CID] = Text::toT(sr->getUser()->getCID().toBase32());

}

void SearchWindow::speak(unsigned int wParam, long lParam)
{
	emit speakerSignal(wParam, lParam);
}
