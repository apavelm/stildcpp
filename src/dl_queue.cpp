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

#include "dl_queue.h"

QIcon DownLoadQueueWindow::folderIcon;
QIcon DownLoadQueueWindow::fileIcon;

int DownLoadQueueWindow::columnIndexes[] = { COLUMN_TARGET, COLUMN_STATUS, COLUMN_SIZE, COLUMN_DOWNLOADED, COLUMN_PRIORITY,
COLUMN_USERS, COLUMN_PATH, COLUMN_EXACT_SIZE, COLUMN_ERRORS, COLUMN_ADDED, COLUMN_TTH, COLUMN_TYPE };

int DownLoadQueueWindow::columnSizes[] = { 200, 300, 75, 110, 75, 200, 200, 75, 200, 100, 125, 75 };

static const char* columnNames[] = {
	N_("Filename"),
	N_("Status"),
	N_("Size"),
	N_("Downloaded"),
	N_("Priority"),
	N_("Users"),
	N_("Path"),
	N_("Exact size"),
	N_("Errors"),
	N_("Added"),
	N_("TTH Root"),
	N_("Type")
};

#define FILE_LIST_NAME _T("File Lists")

void DownLoadQueueWindow::QueueItemInfo::remove() { 
	QueueManager::getInstance()->remove(getTarget()); 
}

DownLoadQueueWindow::DownLoadQueueWindow(QWidget *parent) : MdiChild(parent),// test_count(0),
	dirty(true),
	usingDirMenu(false),
	queueSize(0),
	queueItems(0),
	fileLists(0)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_DL_QUEUE;
	
	connect(this, SIGNAL(speakerSignal()), this, SLOT(slotSpeaker()),Qt::QueuedConnection);

	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
						QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
						QIcon::Normal, QIcon::On);
	fileIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	initDirTree();
	initDirMenu();
	initContentTree();
	initQueueMenu();
	
	addQueueList(QueueManager::getInstance()->lockQueue());
	QueueManager::getInstance()->unlockQueue();
	QueueManager::getInstance()->addListener(this);

	//onSpeaker(std::tr1::bind(&DownLoadQueueWindow::handleSpeaker, this, _1, _2));
	
	updateStatus();
}

DownLoadQueueWindow::~DownLoadQueueWindow()
{
	QStandardItem* parentItem = getChild(dirModel->invisibleRootItem());

	while(parentItem != NULL)
	{
		clearTree(parentItem);
		parentItem = getNextSibling(parentItem);
	}

	for(DirectoryIter i = directories.begin(); i != directories.end(); ++i)
	{
		delete i->second;
	}
	directories.clear();

	clearContent();
	
//	SettingsManager::getInstance()->set(SettingsManager::QUEUEFRAME_ORDER, StilUtils::toString(files->getColumnOrder()));
//	SettingsManager::getInstance()->set(SettingsManager::QUEUEFRAME_WIDTHS, StilUtils::toString(files->getColumnWidths()));

	QueueManager::getInstance()->removeListener(this);

	delete fileListMenuAction;
	delete privateMenuAction;
	delete reAddSourceMenuAction;
	delete removeSourceMenuAction;
	delete removeUserFromQueueMenuAction;

//	delete columnMenu;
	delete privateMenu;
	delete reAddSourceMenu;
	delete removeSourceMenu;
	delete removeUserFromQueueMenu;
	delete fileListMenu;
	delete dirMenu;
	delete queueMenu;
}

void DownLoadQueueWindow::initDirTree()
{
	dirMap.clear();

	dirModel = new QStandardItemModel();

	dirView->setModel(dirModel);
	dirView->header()->hide();
	dirView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(dirView, SIGNAL(clicked(QModelIndex)), this, SLOT(updateFiles(QModelIndex)));
}

void DownLoadQueueWindow::initContentTree()
{
	contentMap.clear();
	
	QStringList columns;

	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);

	contentModel = new QStandardItemModel(0, columns.size());
	
	for (int i=0;i<columns.size();i++)
		contentModel->setHeaderData(i, Qt::Horizontal, columns.at(i));

	contentView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	contentView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	proxyContentModel = new dlqueueSortingModel();
	
	proxyContentModel->setDynamicSortFilter(true);
	proxyContentModel->setSourceModel(contentModel);

	contentView->setModel(proxyContentModel);

	//proxyModel -> setSortCaseSensitivity(Qt::CaseInsensitive);
	contentView->setSortingEnabled(true);
	contentView->sortByColumn(0, Qt::AscendingOrder);

	contentView->setRootIsDecorated(false);
	contentView->setAlternatingRowColors(true);
}


void DownLoadQueueWindow::slotSpeaker()
{
	TaskQueue::List t;

	tasks.get(t);

	for(TaskQueue::Iter ti = t.begin(); ti != t.end(); ++ti) {
		if(ti->first == ADD_ITEM) {
			boost::scoped_ptr<QueueItemInfoTask> iit(static_cast<QueueItemInfoTask*>(ti->second));
			
			dcassert(findFile(iit->ii) == -1);
			addQueueItem(iit->ii, false);
			updateStatus();
		} else if(ti->first == REMOVE_ITEM) {
			boost::scoped_ptr<StringTask> target(static_cast<StringTask*>(ti->second));
			QueueItemInfo* ii = getItemInfo(target->str);
			if(!ii) {
				dcassert(ii);
				continue;
			}

			if(isCurDir(ii->getPath()) ) {
				dcassert(findFile(ii) != -1);
				deleteFile(ii);
			}

			if(!ii->isSet(QueueItem::FLAG_USER_LIST)) {
				queueSize-=ii->getSize();
				dcassert(queueSize >= 0);
			}
			queueItems--;
			dcassert(queueItems >= 0);

			pair<DirectoryIter, DirectoryIter> i = directories.equal_range(ii->getPath());
			DirectoryIter j;
			for(j = i.first; j != i.second; ++j) {
				if(j->second == ii)
					break;
			}
			dcassert(j != i.second);
			directories.erase(j);
			if(directories.count(ii->getPath()) == 0) {
				removeDirectory(ii->getPath(), ii->isSet(QueueItem::FLAG_USER_LIST));
				if(isCurDir(ii->getPath()))
					curDir.clear();
			}

			delete ii;
			updateStatus();
			//setDirty(SettingsManager::BOLD_QUEUE);
			dirty = true;
		} else if(ti->first == UPDATE_ITEM) {
			boost::scoped_ptr<UpdateTask> ui(reinterpret_cast<UpdateTask*>(ti->second));
            QueueItemInfo* ii = getItemInfo(ui->target);

			ii->setPriority(ui->priority);
			ii->setRunning(ui->running);
			ii->setDownloadedBytes(ui->downloadedBytes);
			ii->setSources(ui->sources);
			ii->setBadSources(ui->badSources);

			ii->updateMask |= QueueItemInfo::MASK_PRIORITY | QueueItemInfo::MASK_USERS | QueueItemInfo::MASK_ERRORS | QueueItemInfo::MASK_STATUS | QueueItemInfo::MASK_DOWNLOADED;

			if(isCurDir(ii->getPath())) {
				dcassert(findFile(ii) != -1);
				ii->update();
				updateFile(ii);
			}
		}
	}
}

/*
bool DownLoadQueueWindow::handleKeyDownDirs(int c) {
	if(c == VK_DELETE) {
		removeSelectedDir();
	}
	return false;
}

bool DownLoadQueueWindow::handleKeyDownFiles(int c) {
	if(c == VK_DELETE) {
		removeSelected();
	} else if(c == VK_ADD){
		// Increase Item priority
		changePriority(true);
	} else if(c == VK_SUBTRACT){
		// Decrease item priority
		changePriority(false);
	}
	return false;
}
*/
void DownLoadQueueWindow::addQueueList(const QueueItem::StringMap& li) {
//	HoldRedraw hold(files);
//	HoldRedraw hold2(dirs);

	for(QueueItem::StringMap::const_iterator j = li.begin(); j != li.end(); ++j) {
		QueueItem* aQI = j->second;
		QueueItemInfo* ii = new QueueItemInfo(*aQI);
		addQueueItem(ii, true);
	}

	// qDebug() << "TODO: resort";
	//files->resort();
}

DownLoadQueueWindow::QueueItemInfo* DownLoadQueueWindow::getItemInfo(const string& target) {
	string path = Util::getFilePath(target);
	DirectoryPair items = directories.equal_range(path);
	for(DirectoryIter i = items.first; i != items.second; ++i) {
		if(i->second->getTarget() == target) {
			return i->second;
		}
	}
	return 0;
}

bool DownLoadQueueWindow::isCurDir(const std::string& aDir) const {
	 return Util::stricmp(curDir, aDir) == 0;
}

void DownLoadQueueWindow::updateStatus()
{
	int64_t total = 0;
	int cnt = contentView->selectionModel()->selectedRows().size();

	if(cnt < 2)
	{
		cnt = contentMap.size();
	
		for (QMap<QStandardItem*, QueueItemInfo*>::const_iterator iter = contentMap.constBegin(); 
			 iter != contentMap.constEnd(); ++iter)
		{
			QueueItemInfo* ii = iter.value();
			total += (ii->getSize() > 0) ? ii->getSize() : 0;
		}
	}
	else 
	{
		const QModelIndexList indexes = contentView->selectionModel()->selectedRows();
	
		for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
		{
			QStandardItem *item = contentModel->itemFromIndex(proxyContentModel->mapToSource(*index));

			QueueItemInfo* ii = contentMap.value(item);
			total += (ii->getSize() > 0) ? ii->getSize() : 0;
		}
	}

	setStatus(STATUS_PARTIAL_COUNT, str(TF_("Items: %1%") % cnt));
	setStatus(STATUS_PARTIAL_BYTES, str(TF_("Size: %1%") % Text::toT(Util::formatBytes(total))));
	
	if(dirty)
	{
		setStatus(STATUS_TOTAL_COUNT, str(TF_("Files: %1%") % queueItems));
		setStatus(STATUS_TOTAL_BYTES, str(TF_("Size: %1%") % Text::toT(Util::formatBytes(queueSize))));
		dirty = false;
	}
}

void DownLoadQueueWindow::addQueueItem(QueueItemInfo* ii, bool noSort) {
	if(!ii->isSet(QueueItem::FLAG_USER_LIST)) {
		queueSize+=ii->getSize();
	}
	queueItems++;
	dirty = true;

	const string& dir = ii->getPath();

	bool updateDir = (directories.find(dir) == directories.end());
	directories.insert(make_pair(dir, ii));

	if(updateDir) {
		addDirectory(dir, ii->isSet(QueueItem::FLAG_USER_LIST));
	}

	if(isCurDir(dir))
	{
		ii->update();/*
		if(noSort) {
			files->insert(files->size(), ii);
		} else {
			files->insert(ii);
		}
*/
		insertFile(ii);
	}
}

void DownLoadQueueWindow::updateFiles(QModelIndex index) {
//	HoldRedraw hold(files);

	clearContent();
	pair<DirectoryIter, DirectoryIter> i;

	i = directories.equal_range(getSelectedDir(dirModel->itemFromIndex(index)));

	for(DirectoryIter j = i.first; j != i.second; ++j) {
		QueueItemInfo* ii = j->second;
		ii->update();
		//TODO: wtf first parameter?
		//files->insert(files->size(), ii);
		insertFile(ii);
	}
	
	resortFile();

	curDir = getSelectedDir(dirModel->itemFromIndex(index));

	updateStatus();
}

void DownLoadQueueWindow::on(QueueManagerListener::Added, QueueItem* aQI) throw() {
	QueueItemInfo* ii = new QueueItemInfo(*aQI);

	speak(ADD_ITEM,	new QueueItemInfoTask(ii));
}

void DownLoadQueueWindow::on(QueueManagerListener::Removed, QueueItem* aQI) throw() {
	speak(REMOVE_ITEM, aQI->getTarget());
}

void DownLoadQueueWindow::on(QueueManagerListener::Moved, QueueItem* aQI, const string& oldTarget) throw() {
	speak(REMOVE_ITEM, oldTarget);
	speak(ADD_ITEM,	new QueueItemInfoTask(new QueueItemInfo(*aQI)));
}

void DownLoadQueueWindow::on(QueueManagerListener::SourcesUpdated, QueueItem* aQI) throw() {
	speak(UPDATE_ITEM, new UpdateTask(*aQI));
}

void DownLoadQueueWindow::QueueItemInfo::update() {
	if(display != NULL) {
		int colMask = updateMask;
		updateMask = 0;

		if(colMask & MASK_TARGET) {
			display->columns[COLUMN_TARGET] = Text::toT(Util::getFileName(getTarget()));
		}
		int online = 0;
		if(colMask & MASK_USERS || colMask & MASK_STATUS) {
			tstring tmp;

			for(QueueItem::SourceIter j = getSources().begin(); j != getSources().end(); ++j) {
				if(tmp.size() > 0)
					tmp += _T(", ");

				if(j->getUser()->isOnline())
					online++;

				tmp += StilUtils::getNicks(j->getUser());
			}
			display->columns[COLUMN_USERS] = tmp.empty() ? T_("No users") : tmp;
		}
		if(colMask & MASK_STATUS) {
			if(!getRunning()) {
				if(online > 0) {
					if(getSources().size() == 1) {
						display->columns[COLUMN_STATUS] = T_("Waiting (User online)");
					} else {
						display->columns[COLUMN_STATUS] = str(TF_("Waiting (%1% of %2% users online)") % online % getSources().size());
					}
				} else {
					if(getSources().size() == 0) {
						display->columns[COLUMN_STATUS] = T_("No users to download from");
					} else if(getSources().size() == 1) {
						display->columns[COLUMN_STATUS] = T_("User offline");
					} else {
						display->columns[COLUMN_STATUS] = str(TF_("All %1% users offline") % getSources().size());
					}
				}
			} else {
				display->columns[COLUMN_STATUS] = T_("Running...");
			}
		}
		if(colMask & MASK_SIZE) {
			display->columns[COLUMN_SIZE] = (getSize() == -1) ? T_("Unknown") : Text::toT(Util::formatBytes(getSize()));
			display->columns[COLUMN_EXACT_SIZE] = (getSize() == -1) ? T_("Unknown") : Text::toT(Util::formatExactSize(getSize()));
		}
		if(colMask & MASK_DOWNLOADED) {
			if(getSize() > 0)
				display->columns[COLUMN_DOWNLOADED] = Text::toT(Util::formatBytes(getDownloadedBytes()) + " (" + Util::toString((double)getDownloadedBytes()*100.0/(double)getSize()) + "%)");
			else
				display->columns[COLUMN_DOWNLOADED].clear();
		}
		if(colMask & MASK_PRIORITY) {
			switch(getPriority()) {
		case QueueItem::PAUSED: display->columns[COLUMN_PRIORITY] = T_("Paused"); break;
		case QueueItem::LOWEST: display->columns[COLUMN_PRIORITY] = T_("Lowest"); break;
		case QueueItem::LOW: display->columns[COLUMN_PRIORITY] = T_("Low"); break;
		case QueueItem::NORMAL: display->columns[COLUMN_PRIORITY] = T_("Normal"); break;
		case QueueItem::HIGH: display->columns[COLUMN_PRIORITY] = T_("High"); break;
		case QueueItem::HIGHEST: display->columns[COLUMN_PRIORITY] = T_("Highest"); break;
		default: dcasserta(0); break;
			}
		}

		if(colMask & MASK_PATH) {
			display->columns[COLUMN_PATH] = Text::toT(getPath());
		}

		if(colMask & MASK_ERRORS) {
			tstring tmp;
			for(QueueItem::SourceIter j = getBadSources().begin(); j != getBadSources().end(); ++j) {
				if(!j->isSet(QueueItem::Source::FLAG_REMOVED)) {
					if(tmp.size() > 0)
						tmp += _T(", ");
					tmp += StilUtils::getNicks(j->getUser());
					tmp += _T(" (");
					if(j->isSet(QueueItem::Source::FLAG_FILE_NOT_AVAILABLE)) {
						tmp += T_("File not available");
					} else if(j->isSet(QueueItem::Source::FLAG_PASSIVE)) {
						tmp += T_("Passive user");
					} else if(j->isSet(QueueItem::Source::FLAG_CRC_FAILED)) {
						tmp += T_("CRC32 inconsistency (SFV-Check)");
					} else if(j->isSet(QueueItem::Source::FLAG_BAD_TREE)) {
						tmp += T_("Full tree does not match TTH root");
					} else if(j->isSet(QueueItem::Source::FLAG_SLOW_SOURCE)) {
						tmp += T_("Source too slow");
					} else if(j->isSet(QueueItem::Source::FLAG_NO_TTHF)) {
						tmp += T_("Remote client does not fully support TTH - cannot download");
					}
					tmp += ')';
				}
			}
			display->columns[COLUMN_ERRORS] = tmp.empty() ? T_("No errors") : tmp;
		}

		if(colMask & MASK_ADDED) {
			display->columns[COLUMN_ADDED] = Text::toT(Util::formatTime("%Y-%m-%d %H:%M", getAdded()));
		}
		if(colMask & MASK_TTH) {
			display->columns[COLUMN_TTH] = Text::toT(getTTH().toBase32());
		}
		if(colMask & MASK_TYPE) {
			display->columns[COLUMN_TYPE] = Text::toT(Util::getFileExt(getTarget()));
			if(display->columns[COLUMN_TYPE].size() > 0 && display->columns[COLUMN_TYPE][0] == '.')
				display->columns[COLUMN_TYPE].erase(0, 1);
		}
	}
}

DownLoadQueueWindow::DirItemInfo::DirItemInfo(const string& dir_) : dir(dir_), text(dir_.empty() ? 
																					Util::emptyStringT : Text::toT(dir_.substr(0, dir_.length()-1)))
{	
}

QStandardItem* DownLoadQueueWindow::addDirectory(const string& dir, bool isFileList /* = false */, QStandardItem* startAt /* = NULL */)
{
//	test_count++;

	// qDebug() << "addDirectory(): dir"  << StilUtils::TstrtoQ(Text::toT(dir));

	if(isFileList)
	{
		// We assume we haven't added it yet, and that all filelists go to the same
		// directory...
		dcassert(fileLists == NULL);
		fileLists = insertDir(NULL, new DirItemInfo(dir, FILE_LIST_NAME));
		return fileLists;
	}

	// More complicated, we have to find the last available tree item and then see...
	string::size_type i = 0;
	string::size_type j;

	QStandardItem* next = NULL;
	QStandardItem* parent = NULL;

	if(startAt == NULL)
	{
#ifdef Q_OS_WIN
		// First find the correct drive letter
		dcassert(dir.at(1) == ':');
		dcassert(dir.at(2) == '\\');
#else
		dcassert(dir.at(0) == '/');
#endif
		next = getChild(dirModel->invisibleRootItem());

		while(next != NULL)
		{
//			qDebug() << "next:" << StilUtils::TstrtoQ(Text::toT(getDir(next)));

			if(next != fileLists)
			{
#ifdef Q_OS_WIN
				if(Util::strnicmp(getDir(next), dir, 3) == 0)
#else
				if(Util::strnicmp(getDir(next), dir, getDir(next).find("/", 1)) == 0)
#endif
					break;
			}
			
			next = getNextSibling(next);
		}

		if(next == NULL) {
			// First addition, set commonStart to the dir minus the last part...
#ifdef Q_OS_WIN
			i = dir.rfind('\\', dir.length()-2);
#else
			i = dir.rfind('/', dir.length()-2);
#endif
			if(i != string::npos) {
				next = insertDir(NULL, new DirItemInfo(dir.substr(0, i+1)));
			} else {
				dcassert(dir.length() == 3);
				next = insertDir(NULL, new DirItemInfo(dir, Text::toT(dir)));
			}
		}

		// Ok, next now points to x:\... find how much is common

		DirItemInfo* rootInfo = dirMap.value(next);
		const string& rootStr = rootInfo->getDir();
		// qDebug() << "rootStr:"  << StilUtils::TstrtoQ(Text::toT(rootStr));

		i = 0;

		for(;;) {
#ifdef Q_OS_WIN
			j = dir.find('\\', i);
#else
			j = dir.find('/', i);
#endif
			if(j == string::npos)
				break;
//			qDebug() << "dir.c_str() +" << i << StilUtils::TstrtoQ(Text::toT(dir.substr(i))) << endl
//					 << "rootStr.c_str() +" << i << StilUtils::TstrtoQ(Text::toT(rootStr.substr(i)));
			if(Util::strnicmp(dir.c_str() + i, rootStr.c_str() + i, j - i + 1) != 0)
				break;
			i = j + 1;
		}

		if(i < rootStr.length()) {
			QStandardItem* oldRoot = next;

			// Create a new root
			QStandardItem* newRoot = insertDir(NULL, new DirItemInfo(rootStr.substr(0, i)));

			parent = addDirectory(rootStr, false, newRoot);

			next = getChild(oldRoot);

			while(next != NULL) {
				moveNode(next, parent);
				next = getChild(oldRoot);
			}
			delete rootInfo;
			eraseDir(oldRoot);
			parent = newRoot;
		} else {
			// Use this root as parent
			parent = next;
			//if (parent) qDebug() << "parent2:" << parent->text();
			next = getChild(parent);
			//if (next)  qDebug() << "next2:" << next->text();
		}
	} else {
		parent = startAt;
		next = getChild(parent);
		i = getDir(parent).length();
		dcassert(Util::strnicmp(getDir(parent), dir, i) == 0);
	}

	QStandardItem* firstParent = parent;

	while( i < dir.length() ) {
		while(next != NULL) {
			if(next != fileLists) {
				const string& n = getDir(next);
				if(Util::strnicmp(n.c_str()+i, dir.c_str()+i, n.length()-i) == 0) {
					// Found a part, we assume it's the best one we can find...
					i = n.length();

					parent = next;
					next = getChild(next);
					break;
				}
			}
			next = getNextSibling(next);
		}

		if(next == NULL) {
			// We didn't find it, add...
#ifdef Q_OS_WIN
			j = dir.find('\\', i);
#else
			j = dir.find('/', i);
#endif	
			dcassert(j != string::npos);
			parent = insertDir(parent, new DirItemInfo(dir.substr(0, j+1), Text::toT(dir.substr(i, j-i))));
			i = j + 1;
		}
	}

	if(firstParent != NULL)
		dirView->expand(firstParent->index());
	return parent;
}

void DownLoadQueueWindow::removeDirectory(const string& dir, bool isFileList  /* = false */) {
	// qDebug() << "removeDirectory()" << StilUtils::TstrtoQ(Text::toT(dir));
	
	// First, find the last name available
	string::size_type i = 0;

	QStandardItem* next = getChild(dirModel->invisibleRootItem());
	QStandardItem* parent = NULL;

	if(isFileList) {
		dcassert(fileLists != NULL);
		delete dirMap.value(fileLists);
		eraseDir(fileLists);
		fileLists = NULL;
		return;
	} else {
		while(i < dir.length()) {
			while(next != NULL) {
				if(next != fileLists) {
					const string& n = getDir(next);
					if(Util::strnicmp(n.c_str()+i, dir.c_str()+i, n.length()-i) == 0) {
						// Match!
						parent = next;
						next = getChild(next);
						i = n.length();
						break;
					}
				}
				next = getNextSibling(next);
			}
			if(next == NULL)
				break;
		}
	}

	next = parent;

	while((getChild(next) == NULL) && (directories.find(getDir(next)) == directories.end())) {
		delete dirMap.value(next);
		parent = next->parent();

		eraseDir(next);
		if(parent == NULL)
			break;
		next = parent;
		}
}

void DownLoadQueueWindow::moveDir(QStandardItem* parentItem, const string& target)
{
	QStandardItem* next = getChild(parentItem);

	while(next != NULL) {
		// must add path separator since getLastDir only give us the name
		moveDir(next, target + Util::getLastDir(getDir(next)) + PATH_SEPARATOR);
		next = getNextSibling(next);
	}
	const string& dir = getDir(parentItem);

	DirectoryPair p = directories.equal_range(dir);

	for(DirectoryIter i = p.first; i != p.second; ++i)
	{
		QueueItemInfo* ii = i->second;
		QueueManager::getInstance()->move(ii->getTarget(), target + Util::getFileName(ii->getTarget()));
	}
}

void DownLoadQueueWindow::removeDir(QStandardItem* parentItem)
{
	if(parentItem == NULL)
		return;

	QStandardItem* child = getChild(parentItem);

	while(child)
	{
		removeDir(child);
		child = getNextSibling(child);
	}

	const string& name = getDir(parentItem);

	DirectoryPair dp = directories.equal_range(name);

	for(DirectoryIter i = dp.first; i != dp.second; ++i)
	{
		QueueManager::getInstance()->remove(i->second->getTarget());
	}
}

/*
void DownLoadQueueWindow::changePriority(bool inc){
	std::vector<size_t> selected = files->getSelected();
	for(std::vector<size_t>::iterator i = selected.begin(); i != selected.end(); ++i) {
		QueueItemInfo* ii = files->getData(*i);
		QueueItem::Priority p = ii->getPriority();

		if ((inc && p == QueueItem::HIGHEST) || (!inc && p == QueueItem::PAUSED)){
			// Trying to go higher than HIGHEST or lower than PAUSED
			// so do nothing
			continue;
		}

		switch(p){
			case QueueItem::HIGHEST: p = inc ? QueueItem::HIGHEST : QueueItem::HIGH; break;
			case QueueItem::HIGH:    p = inc ? QueueItem::HIGHEST : QueueItem::NORMAL; break;
			case QueueItem::NORMAL:  p = inc ? QueueItem::HIGH    : QueueItem::LOW; break;
			case QueueItem::LOW:     p = inc ? QueueItem::NORMAL  : QueueItem::LOWEST; break;
			case QueueItem::LOWEST:  p = inc ? QueueItem::LOW     : QueueItem::PAUSED; break;
			case QueueItem::PAUSED:  p = inc ? QueueItem::LOWEST : QueueItem::PAUSED; break;
		}

		QueueManager::getInstance()->setPriority(ii->getTarget(), p);
	}
}
*/

void DownLoadQueueWindow::setDirPriority(QStandardItem* parentItem, const QueueItem::Priority& p)
{
	if(parentItem == NULL)
		return;
	
	QStandardItem* child = getChild(parentItem);

	while(child)
	{
		setDirPriority(child, p);
		child = getNextSibling(child);
	}

	const string& name = getDir(parentItem);

	DirectoryPair dp = directories.equal_range(name);

	for(DirectoryIter i = dp.first; i != dp.second; ++i)
	{
		QueueManager::getInstance()->setPriority(i->second->getTarget(), p);
	}
}


void DownLoadQueueWindow::clearTree(QStandardItem* parentItem)
{
	QStandardItem* next = getChild(parentItem);

	while(next != NULL)
	{
		clearTree(next);
		next = getNextSibling(next);
	}

	delete dirMap.value(parentItem);
}

// Put it here to avoid a copy for each recursion...

void DownLoadQueueWindow::moveNode(QStandardItem* item, QStandardItem* parent)
{
	QStandardItem* ht = item;
	parent->appendRow(item);
	QStandardItem* next = getChild(item);

	while(next != NULL) 
	{
		moveNode(next, ht);
		next = getChild(item);
	}

	eraseDir(item);
}

const string& DownLoadQueueWindow::getSelectedDir(QStandardItem* item) {
	DirItemInfo* info = dirMap.value(item);
	return info == NULL ? Util::emptyString : info->getDir();
}

const string& DownLoadQueueWindow::getDir(QStandardItem* item)
{
	//TODO: reimplement this with Qt-style (through getData() model method)
	DirItemInfo* info = dirMap.value(item);
	return info == NULL ? Util::emptyString : info->getDir();
}

void DownLoadQueueWindow::setStatus(int s, const tstring& text)
{
	switch(s)
	{
		case STATUS_STATUS:
			dlqueueStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_PARTIAL_COUNT:
			partCountStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_PARTIAL_BYTES:
			partBytesStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_TOTAL_COUNT:
			totalCountStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_TOTAL_BYTES:
			totalBytesStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		default:
			dcdebug("Set status for %d with text: %s\n", s, Text::fromT(text).c_str());
	}
}

void DownLoadQueueWindow::speak()
{
	emit speakerSignal();
}

void DownLoadQueueWindow::insertFile(QueueItemInfo* qii)
{
	QStandardItem *parentItem = contentModel->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStringList::const_iterator constIterator;

	QStandardItem *item;

    for (int i = 0; i<COLUMN_LAST;i++)
    {
    	item = new QStandardItem(StilUtils::TstrtoQ(qii->getText(i)));
    	
    	if (i == 0)
		{
			item->setIcon(*(qii->getImage()));
			contentMap.insert(item, qii);
		}

    	itemList << item;
    }

    parentItem->appendRow(itemList);
}

int DownLoadQueueWindow::findFile(QueueItemInfo* qii)
{
	int i = 0;

	for (QMap<QStandardItem*, QueueItemInfo*>::const_iterator iter = contentMap.constBegin(); 
		 iter != contentMap.constEnd(); ++iter, ++i)
	{
		if (iter.value() == qii) return i;
	}

	return -1;
}

void DownLoadQueueWindow::deleteFile(QueueItemInfo* qii)
{
	QStandardItem *item = contentMap.key(qii);
	
	Q_ASSERT(contentMap.remove(item) > 0);

	QStandardItem *parentItem = contentModel->invisibleRootItem();
	
	parentItem->removeRow(item->row());
}

void DownLoadQueueWindow::updateFile(QueueItemInfo* qii)
{
	int pos = findFile(qii);
	
	Q_ASSERT(pos != -1);

	QStandardItem *item = contentMap.key(qii);

	contentMap.remove(item);

	contentModel->setData(contentModel->index(item->row(), 0), *(qii->getImage()), Qt::DecorationRole);
	
	for (int i = 0; i<COLUMN_LAST;i++)
		contentModel->setData(contentModel->index(item->row(), i), StilUtils::TstrtoQ(qii->getText(i)), Qt::DisplayRole);

	contentMap.insert(item, qii);

	//this commented code has bug (feature :)): selection cleared during update of item

/*
	QStandardItem *parentItem = contentModel->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStandardItem *item;

    for (int i = 0; i<COLUMN_LAST;i++)
    {
    	item = new QStandardItem(StilUtils::TstrtoQ(qii->getText(i)));
    	
    	if (i == 0)
		{
			item->setIcon(*(qii->getImage()));
			contentMap.insert(item, qii);
		}

    	itemList << item;
    }

	parentItem->takeRow(pos);
    parentItem->insertRow(pos, itemList);
*/
}

void DownLoadQueueWindow::clearContent()
{
	contentModel->removeRows(0, contentModel->rowCount());
	contentMap.clear();
}


void DownLoadQueueWindow::resortFile()
{
	proxyContentModel->clear();
}

QStandardItem* DownLoadQueueWindow::insertDir(QStandardItem* parentItem, DirItemInfo* dii)
{
	/* qDebug() << "insertDir():" << endl << "dir:" << StilUtils::TstrtoQ(Text::toT(dii->getDir()))
		 << "text:" << StilUtils::TstrtoQ(dii->getText());
	parentItem == NULL ? qDebug() << "No parent" : qDebug() << "parent:" << parentItem->text();
	*/
	QStandardItem *item = new QStandardItem(folderIcon, StilUtils::TstrtoQ(dii->getText()));

	if (parentItem == NULL)
	{
		QStandardItem *pItem = dirModel->invisibleRootItem();

		pItem->appendRow(item);
	}
	else
		parentItem->appendRow(item);

	dirMap.insert(item, dii);

//	qDebug() << "insertDir():" << "row:" <<  item->row() << "column:" << item->column();
/*	
	if (test_count > 1)
	{
		QStandardItem* par = getNextSibling(getChild(dirModel->invisibleRootItem()));

		if (par)
		{
			qDebug() << "TEST_P" << par->text();
			QStandardItem* it = getChild(par);

			if (it)
			{
				qDebug() << "TEST" << it->text();

				QStandardItem* it2 = getNextSibling(it);

				if (it2) 
					qDebug() << "TEST2" << it2->text();
				else
					qDebug() << "TEST2_NULL";
			}
			else
				qDebug() << "TEST_NULL";
		}
		else
			qDebug() << "TEST_P_NULL";
	}
*/
	return item;
}

void DownLoadQueueWindow::eraseDir(QStandardItem* item)
{
	dirModel->removeRow(item->row(), item->index().parent());
}

QIcon* DownLoadQueueWindow::QueueItemInfo::getImage() const
{
	return &fileIcon;
}

QStandardItem* DownLoadQueueWindow::getNextSibling(const QStandardItem* item) const
{
/*
	qDebug() << "Sibling for" << item->text();

	QModelIndex index = dirModel->sibling(item->row() + 1, item->column(), item->index());

	if (index.isValid())
	{
			qDebug() << "Next sibling:" << dirModel->itemFromIndex(index)->text();
			return dirModel->itemFromIndex(index);
	}
	else
	{
			qDebug() << "No sibling";
			return NULL;
	}
*/

		QModelIndex index = dirModel->sibling(item->row() + 1, item->column(), item->index());

		return (index.isValid() ? dirModel->itemFromIndex(index) : NULL);
}

QStandardItem* DownLoadQueueWindow::getChild(const QStandardItem* item) const
{
//	qDebug() << "getChild(): row:" <<  item->row() << "column:" << item->column() << "text:" << item->text();
//	qDebug() << "getChild(): hasChildren:" << item->hasChildren();
	
//	QStandardItem* temp = item->child(item->row() == -1 ? 0 : item->row(), item->column() == -1 ? 0 : item->column());
	QStandardItem* temp = item->child(0, 0);
//	if (!temp) qDebug() << "No child";
	return temp;
}

void DownLoadQueueWindow::initDirMenu()
{
	dirMenu = new QMenu(dirView);
    dirView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dirView, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(showDirMenu(const QPoint&)));

	QMenu *priorityMenu = new QMenu(tr("Set priority"), dirMenu);
	connect(priorityMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotChangeProirity(QAction *)));

	QAction *action;
	QStringList priorityList;
	priorityList << tr("Paused") << tr("Lowest") << tr("Low") << tr("Normal") << tr("High") << tr("Highest");
	
	for (int i=0; i<priorityList.size(); ++i)
	{
		action = priorityMenu->addAction(priorityList.at(i));
		action->setData(i);
	}
	
	dirMenu->addMenu(priorityMenu);

	dirMenu->addAction(tr("Move/Rename"), this, SLOT(actionMoveRenameDir()));

	dirMenu->addSeparator();

	dirMenu->addAction(tr("Remove"), this, SLOT(actionRemove()));
}

void DownLoadQueueWindow::showDirMenu(const QPoint& point)
{
    if (dirView->indexAt(point).isValid())
    {
        foreach (QAction *action, fantomActions)
            action->setVisible(dirView->selectionModel()->selectedRows().size() <= 1 ? true : false);
		
		usingDirMenu = true;

		dirMenu->exec(dirView->mapToGlobal(point));
    }
}

void DownLoadQueueWindow::actionMoveRenameDir()
{
	const QModelIndex index = dirView->selectionModel()->selectedRows().first();

	QStandardItem *item = dirModel->itemFromIndex(index);

	dcassert(!curDir.empty());
	tstring name = Text::toT(curDir);

	QString dir = QFileDialog::getExistingDirectory(this, tr("Move/rename Directory"), StilUtils::TstrtoQ(name),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (dir != StilUtils::TstrtoQ(name) && !dir.isEmpty())
	{
		moveDir(item, Text::fromT(StilUtils::QtoTstr(dir)));
	}
}

void DownLoadQueueWindow::initQueueMenu()
{
    queueMenu = new QMenu(contentView);
    contentView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(contentView, SIGNAL(customContextMenuRequested(const QPoint&)),
                this, SLOT(showQueueMenu(const QPoint&)));

    //connect(treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
    //      this, SLOT(updateActions(const QItemSelection &, const QItemSelection &)));


    fantomActions.append(queueMenu->addAction(tr("Search for alternates"), this, SLOT(actionSearchAlt())));
    fantomActions.append(queueMenu->addAction(tr("Lookup TTH at Bitzi.com"), this, SLOT(actionBitzi())));
    fantomActions.append(queueMenu->addAction(tr("Copy magnet link to clipboard"), this, SLOT(actionCopyMagnetToClipboard())));

    queueMenu->addAction(tr("Move/Rename"), this, SLOT(actionMoveRename()));

	QMenu *priorityMenu = new QMenu(tr("Set priority"), queueMenu);
	connect(priorityMenu, SIGNAL(triggered(QAction*)), this, SLOT(slotChangeProirity(QAction *)));

	QAction *action;
	QStringList priorityList;
	priorityList << tr("Paused") << tr("Lowest") << tr("Low") << tr("Normal") << tr("High") << tr("Highest");
	
	for (int i=0; i<priorityList.size(); ++i)
	{
//		action = priorityMenu->addAction(priorityList.at(i), this, SLOT(slotChangeProirity()));
		action = priorityMenu->addAction(priorityList.at(i));
		action->setData(i);
	}
	
	queueMenu->addMenu(priorityMenu);
	
//    QAction *priorityAction = queueMenu->addAction(tr("Set priority"), this, SLOT(actionSetPriority()));

	fileListMenu = new QMenu(tr("Get file list"), queueMenu);
	connect(fileListMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionGetFilelist(QAction *)));
	fileListMenuAction = queueMenu->addMenu(fileListMenu);
	fantomActions.append(fileListMenuAction);

	privateMenu = new QMenu(tr("Send private message"), queueMenu);
	connect(privateMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionSendPM(QAction *)));
	privateMenuAction = queueMenu->addMenu(privateMenu);
	fantomActions.append(privateMenuAction);

    queueMenu->addSeparator();

	reAddSourceMenu = new QMenu(tr("Re-add source"), queueMenu);
	connect(reAddSourceMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionReaddSource(QAction *)));
	reAddSourceMenuAction = queueMenu->addMenu(reAddSourceMenu);
	fantomActions.append(reAddSourceMenuAction);

	removeSourceMenu = new QMenu(tr("Remove source"), queueMenu);
	connect(removeSourceMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionRemoveSource(QAction *)));
	removeSourceMenuAction = queueMenu->addMenu(removeSourceMenu);
	fantomActions.append(removeSourceMenuAction);
	
	removeUserFromQueueMenu = new QMenu(tr("Remove user from queue"), queueMenu);
	connect(removeUserFromQueueMenu, SIGNAL(triggered(QAction*)), this, SLOT(actionRemoveUserFromQueue(QAction *)));
	removeUserFromQueueMenuAction = queueMenu->addMenu(removeUserFromQueueMenu);
	fantomActions.append(removeUserFromQueueMenuAction);

    queueMenu->addAction(tr("Remove"), this, SLOT(actionRemove()));
}

void DownLoadQueueWindow::showQueueMenu(const QPoint &point)
{
    if (contentView->indexAt(point).isValid())
    {   
        foreach (QAction *action, fantomActions)
            action->setVisible(contentView->selectionModel()->selectedRows().size() <= 1 ? true : false);
		
		usingDirMenu = false;

		const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
		QStandardItem *item = contentModel->itemFromIndex(index);
		QueueItemInfo* qii = contentMap.value(item);

		fileListMenuAction->setEnabled(makeNestedMenu(fileListMenu, qii->getSources(), false));
		privateMenuAction->setEnabled(makeNestedMenu(privateMenu, qii->getSources(), false));
		reAddSourceMenuAction->setEnabled(makeNestedMenu(reAddSourceMenu, qii->getBadSources(), true));
		removeSourceMenuAction->setEnabled(makeNestedMenu(removeSourceMenu, qii->getSources(), true));
		removeUserFromQueueMenuAction->setEnabled(makeNestedMenu(removeUserFromQueueMenu, qii->getSources(), true));

        queueMenu->exec(contentView->mapToGlobal(point));
    }
}

void DownLoadQueueWindow::actionSearchAlt()
{
	const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = contentModel->itemFromIndex(index);

	const tstring tth = contentMap.value(item)->getDisplay()->columns[COLUMN_TTH];

	if (tth != Util::emptyStringT)
		MainWindowImpl::getInstance()->OpenSearch(tth, 0, SearchManager::SIZE_DONTCARE, SearchManager::TYPE_TTH);
}

void DownLoadQueueWindow::actionBitzi()
{
	const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = contentModel->itemFromIndex(index);

	const tstring tth = contentMap.value(item)->getDisplay()->columns[COLUMN_TTH];

	if (tth != Util::emptyStringT)
	{
		QUrl url = QUrl("http://bitzi.com/lookup/tree:tiger:" + StilUtils::TstrtoQ(tth));
		QDesktopServices::openUrl(url);
	}
}

void DownLoadQueueWindow::actionCopyMagnetToClipboard()
{
	const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = contentModel->itemFromIndex(index);

	const tstring tth = contentMap.value(item)->getDisplay()->columns[COLUMN_TTH];
	const tstring fileName = contentMap.value(item)->getDisplay()->columns[COLUMN_TARGET];

	if(!fileName.empty())
	{
		QString magnet = StilUtils::TstrtoQ(_T("magnet:?xt=urn:tree:tiger:") + tth + _T("&dn=") +Text::toT(Util::encodeURI(Text::fromT(fileName))));
		StilUtils::copy2Clipboard(magnet);
	}
}

void DownLoadQueueWindow::actionMoveRename()
{
	const QModelIndexList indexes = contentView->selectionModel()->selectedRows();
	
	if (indexes.size() == 1)
	{
		QueueItemInfo* ii = contentMap.value(contentModel->itemFromIndex(proxyContentModel->mapToSource(indexes.first())));

		tstring target = Text::toT(ii->getTarget());
		tstring ext = Util::getFileExt(target);

		tstring ext2;
		if (!ext.empty())
		{
			ext = ext.substr(1);
//			ext2 = _T("(*.") + ext + _T(");;All files (*.*)");
			ext2 = _T("(*.") + ext + _T(")");
		}
		else
			ext2 += _T("All files (*.*)");
		
		QString fileName = QFileDialog::getSaveFileName(this, tr("Move/rename File"), 
														StilUtils::TstrtoQ(target), StilUtils::TstrtoQ(ext2));
		
		if (fileName != StilUtils::TstrtoQ(target) && !fileName.isEmpty())
			QueueManager::getInstance()->move(ii->getTarget(), Text::fromT(StilUtils::QtoTstr(fileName)));
	}
	else
	{
		tstring name = Text::toT(curDir);

		QString dir = QFileDialog::getExistingDirectory(this, tr("Move/rename Files"), StilUtils::TstrtoQ(name),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		if (dir != StilUtils::TstrtoQ(name) && !dir.isEmpty())
		{
			for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
			{
				QStandardItem *item = contentModel->itemFromIndex(proxyContentModel->mapToSource(*index));
			
				QueueItemInfo* ii = contentMap.value(item);

				QueueManager::getInstance()->move(ii->getTarget(), 
												  Text::fromT(StilUtils::QtoTstr(dir)) + Util::getFileName(ii->getTarget()));
			}
		}
	}
}

void DownLoadQueueWindow::actionSendPM(QAction *action)
{
	MainWindowImpl::getInstance()->OpenPM(action->data().value<UserPtr>());
}

void DownLoadQueueWindow::actionGetFilelist(QAction *action)
{
	const UserPtr user = action->data().value<UserPtr>();
	
	try
	{
		QueueManager::getInstance()->addList(user, QueueItem::FLAG_CLIENT_VIEW);
	} catch(const Exception&) {}
}

void DownLoadQueueWindow::actionReaddSource(QAction *action)
{
	const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = contentModel->itemFromIndex(index);

	QueueItemInfo* ii = contentMap.value(item);

	const UserPtr user = action->data().value<UserPtr>();

	if(!user)
	{
		for(QueueItem::SourceIter s = ii->getBadSources().begin(); s != ii->getBadSources().end(); ++s)
		{
			QueueManager::getInstance()->readd(ii->getTarget(), s->getUser());
		}
	} 
	else
	{
		try
		{
			QueueManager::getInstance()->readd(ii->getTarget(), user);
		}
		catch(const Exception& e)
		{
			setStatus(STATUS_STATUS, Text::toT(e.getError()));
		}
	}
}

void DownLoadQueueWindow::actionRemoveSource(QAction *action)
{
	const QModelIndex index = proxyContentModel->mapToSource(contentView->selectionModel()->selectedRows().first());
	
	QStandardItem *item = contentModel->itemFromIndex(index);

	QueueItemInfo* ii = contentMap.value(item);

	const UserPtr user = action->data().value<UserPtr>();

	if(!user)
	{
		for(QueueItem::SourceIter si = ii->getSources().begin(); si != ii->getSources().end(); ++si)
		{
			QueueManager::getInstance()->removeSource(ii->getTarget(), si->getUser(), QueueItem::Source::FLAG_REMOVED);
		}
	} 
	else 
	{
		QueueManager::getInstance()->removeSource(ii->getTarget(), user, QueueItem::Source::FLAG_REMOVED);
	}
}

void DownLoadQueueWindow::actionRemoveUserFromQueue(QAction *action)
{
	QueueManager::getInstance()->removeSource(action->data().value<UserPtr>(), QueueItem::Source::FLAG_REMOVED);
}

void DownLoadQueueWindow::actionRemove()
{
	if(!BOOLSETTING(CONFIRM_ITEM_REMOVAL) || QMessageBox::question(this, tr("StilDC++"),tr("Really remove?"), 
																  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		if (!usingDirMenu)
		{
			const QModelIndexList indexes = contentView->selectionModel()->selectedRows();

			for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
			{
				QStandardItem *item = contentModel->itemFromIndex(proxyContentModel->mapToSource(*index));
				contentMap.value(item)->remove();
			}
		}
		else
		{
			const QModelIndex index = dirView->selectionModel()->selectedRows().first();

			QStandardItem *item = dirModel->itemFromIndex(index);

			removeDir(item);
		}
	}
}

void DownLoadQueueWindow::slotChangeProirity(QAction *action)
{
//	qDebug() << "Select priority" << action->data().toInt();
	
	QueueItem::Priority p;

	switch(action->data().toInt())
	{
		case 0: p = QueueItem::PAUSED; break;
		case 1: p = QueueItem::LOWEST; break;
	case 2: p = QueueItem::LOW; break;
		case 3: p = QueueItem::NORMAL; break;
		case 4: p = QueueItem::HIGH; break;
		case 5: p = QueueItem::HIGHEST; break;
		default: p = QueueItem::DEFAULT; break;
	}

	if(usingDirMenu)
	{
		const QModelIndex index = dirView->selectionModel()->selectedRows().first();
	
		QStandardItem *item = dirModel->itemFromIndex(index);

		setDirPriority(item, p);
	}
	else
	{
		const QModelIndexList indexes = contentView->selectionModel()->selectedRows();
	
		for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
		{
			QStandardItem *item = contentModel->itemFromIndex(proxyContentModel->mapToSource(*index));
			
			QueueManager::getInstance()->setPriority(contentMap.value(item)->getTarget(), p);
		}
	}
}

bool DownLoadQueueWindow::makeNestedMenu(QMenu *parentMenu, 
										 const QueueItem::SourceList& sources, bool offlineNotImportant)
{
	unsigned int count = 0;
	
	QAction *action;

	parentMenu->clear();

	if (parentMenu == reAddSourceMenu || parentMenu == removeSourceMenu)
	{
		action = parentMenu->addAction(tr("All"));
		action->setData(NULL);
		parentMenu->addSeparator();
	}
	
	for(QueueItem::SourceConstIter s_iter = sources.begin(); s_iter != sources.end(); ++s_iter)
	{
		const QueueItem::Source& src = *s_iter;

		if(src.getUser()->isOnline() || offlineNotImportant)
		{
			tstring nick = StilUtils::getNicks(src.getUser());
			action = parentMenu->addAction(StilUtils::TstrtoQ(nick));
			action->setData(qVariantFromValue((src.getUser())));
			++count;
		}
	}
	
	return (count > 0);
}
