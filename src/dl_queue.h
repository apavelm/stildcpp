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

#ifndef __DL_QUEUE_H__
#define __DL_QUEUE_H__

#include "mainwindowimpl.h"
#include "mdi_c.h"
#include "stilutils.h"

#include "ui_dlqueue.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"

#include "client/TaskQueue.h"
#include "client/FastAlloc.h"
#include "client/QueueManagerListener.h"
#include "client/QueueManager.h"
#include "client/QueueItem.h"
#include <boost/scoped_ptr.hpp>

#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>

class dlqueueSortingModel;

class DownLoadQueueWindow : public MdiChild, private Ui::mdi_DLqueue, private QueueManagerListener
{
	Q_OBJECT

public:
	DownLoadQueueWindow(QWidget *parent);

	~DownLoadQueueWindow();

	enum Status {
		STATUS_SHOW_TREE,
		STATUS_STATUS,
		STATUS_PARTIAL_COUNT,
		STATUS_PARTIAL_BYTES,
		STATUS_TOTAL_COUNT,
		STATUS_TOTAL_BYTES,
		STATUS_LAST
	};

private:
	enum {
		COLUMN_FIRST,
		COLUMN_TARGET = COLUMN_FIRST,
		COLUMN_STATUS,
		COLUMN_SIZE,
		COLUMN_DOWNLOADED,
		COLUMN_PRIORITY,
		COLUMN_USERS,
		COLUMN_PATH,
		COLUMN_EXACT_SIZE,
		COLUMN_ERRORS,
		COLUMN_ADDED,
		COLUMN_TTH,
		COLUMN_TYPE,
		COLUMN_LAST
	};
	enum Tasks {
		ADD_ITEM,
		REMOVE_ITEM,
		UPDATE_ITEM
	};
	
	class DirItemInfo : public FastAlloc<DirItemInfo> {
	public:
		DirItemInfo(const string& dir);
		DirItemInfo(const string& dir_, const tstring& text_) : dir(dir_), text(text_) { }
		const tstring& getText() const { return text; }
		int getImage();
		int getSelectedImage();
		const string& getDir() const { return dir; }
	private:
		string dir;
		tstring text;
	};

	class QueueItemInfo;
	friend class QueueItemInfo;

	class QueueItemInfo : public Flags, public FastAlloc<QueueItemInfo> {
	public:

		struct Display : public FastAlloc<Display> {
			tstring columns[COLUMN_LAST];
		};

		enum {
			MASK_TARGET = 1 << COLUMN_TARGET,
			MASK_STATUS = 1 << COLUMN_STATUS,
			MASK_SIZE = 1 << COLUMN_SIZE,
			MASK_DOWNLOADED = 1 << COLUMN_DOWNLOADED,
			MASK_PRIORITY = 1 << COLUMN_PRIORITY,
			MASK_USERS = 1 << COLUMN_USERS,
			MASK_PATH = 1 << COLUMN_PATH,
			MASK_ERRORS = 1 << COLUMN_ERRORS,
			MASK_ADDED = 1 << COLUMN_ADDED,
			MASK_TTH = 1 << COLUMN_TTH,
			MASK_TYPE = 1 << COLUMN_TYPE
		};

		QueueItemInfo(const QueueItem& aQI) : Flags(aQI), target(aQI.getTarget()),
			path(Util::getFilePath(aQI.getTarget())),
			size(aQI.getSize()), downloadedBytes(aQI.getDownloadedBytes()),
			added(aQI.getAdded()), priority(aQI.getPriority()), running(aQI.isRunning()), tth(aQI.getTTH()), 
			sources(aQI.getSources()), badSources(aQI.getBadSources()), updateMask((uint32_t)-1), display(0)
		{
		}

		~QueueItemInfo() { delete display; }

		void update();

		void remove();

		// TypedListView functions
		const tstring& getText(int col) {
			return getDisplay()->columns[col];
		}

		QIcon* getImage() const;
		/*
		int getImage() const {
			WinUtil::getIconIndex(Text::toT(getTarget()));
		}
		*/
		static int compareItems(QueueItemInfo* a, QueueItemInfo* b, int col) {
			switch(col) {
				case COLUMN_SIZE: case COLUMN_EXACT_SIZE: return compare(a->getSize(), b->getSize());
				case COLUMN_PRIORITY: return compare((int)a->getPriority(), (int)b->getPriority());
				case COLUMN_DOWNLOADED: return compare(a->getDownloadedBytes(), b->getDownloadedBytes());
				case COLUMN_ADDED: return compare(a->getAdded(), b->getAdded());
			default: return (QString::compare(StilUtils::TstrtoQ(a->getDisplay()->columns[col]), 
																 StilUtils::TstrtoQ(b->getDisplay()->columns[col]), Qt::CaseInsensitive) < 0);
					//lstrcmpi(a->getDisplay()->columns[col].c_str(), b->getDisplay()->columns[col].c_str());
			}
		}

		QueueItem::SourceList& getSources() { return sources; }
		QueueItem::SourceList& getBadSources() { return badSources; }

		Display* getDisplay() {
			if(!display) {
				display = new Display;
				update();
			}
			return display;
		}

		bool isSource(const UserPtr& u) {
			return std::find(sources.begin(), sources.end(), u) != sources.end();
		}
		bool isBadSource(const UserPtr& u) {
			return std::find(badSources.begin(), badSources.end(), u) != badSources.end();
		}

		GETSET(string, target, Target);
		GETSET(string, path, Path);
		GETSET(int64_t, size, Size);
		GETSET(int64_t, downloadedBytes, DownloadedBytes);
		GETSET(time_t, added, Added);
		GETSET(QueueItem::Priority, priority, Priority);
		GETSET(bool, running, Running);
		GETSET(TTHValue, tth, TTH);
		GETSET(QueueItem::SourceList, sources, Sources);
		GETSET(QueueItem::SourceList, badSources, BadSources);
		uint32_t updateMask;

	private:

		Display* display;

		QueueItemInfo(const QueueItemInfo&);
		QueueItemInfo& operator=(const QueueItemInfo&);
	};

	struct QueueItemInfoTask : public FastAlloc<QueueItemInfoTask>, public Task {
		QueueItemInfoTask(QueueItemInfo* ii_) : ii(ii_) { }
		QueueItemInfo* ii;
	};

	struct UpdateTask : public FastAlloc<UpdateTask>, public Task {
		UpdateTask(const QueueItem& source) : target(source.getTarget()), priority(source.getPriority()),
			running(source.isRunning()), downloadedBytes(source.getDownloadedBytes()), sources(source.getSources()), badSources(source.getBadSources()) 
		{
		}

		string target;
		QueueItem::Priority priority;
		bool running;
		int64_t downloadedBytes;

		QueueItem::SourceList sources;
		QueueItem::SourceList badSources;
	};

	TaskQueue tasks;

	typedef unordered_multimap<string, QueueItemInfo*, noCaseStringHash, noCaseStringEq> DirectoryMap;
	typedef DirectoryMap::iterator DirectoryIter;
	typedef pair<DirectoryIter, DirectoryIter> DirectoryPair;
	DirectoryMap directories;

	std::string curDir;

	bool dirty;
	bool usingDirMenu;
	
	int64_t queueSize;
	int queueItems;

	QStandardItem* fileLists;

	static int columnIndexes[COLUMN_LAST];
	static int columnSizes[COLUMN_LAST];

	void updateStatus();

	void addQueueItem(QueueItemInfo* qi, bool noSort);
	void addQueueList(const QueueItem::StringMap& l);

	QStandardItem* addDirectory(const string& dir, bool isFileList = false, QStandardItem* startAt = NULL);
	void removeDirectories(QStandardItem* ht);
	void removeDirectory(const string& dir, bool isFileList = false);

	bool isCurDir(const string& aDir) const;

	QueueItemInfo* getItemInfo(const string& target);

	void clearTree(QStandardItem* parentItem);

	void moveSelectedDir();
	void moveDir(QStandardItem* parentItem, const string& target);

	void moveNode(QStandardItem* item, QStandardItem* parent);

	void removeSelected();
	void removeSelectedDir();

	const string& getSelectedDir(QStandardItem* item);
	const string& getDir(QStandardItem* item);

	void removeDir(QStandardItem* parentItem);
	void setDirPriority(QStandardItem* parentItem, const QueueItem::Priority& p);
	void changePriority(bool inc);
	
	void handleShowTreeClicked();

	void handleMove();
	void handleRemove();
	void handlePriority(unsigned id);
	void handlePM(const UserPtr& user);
	void handleRemoveSource(const UserPtr& user);
	void handleRemoveSources(const UserPtr& user);
	void handleBrowseList(const UserPtr& user);
	void handleReadd(const UserPtr& user);
	bool handleKeyDownFiles(int c);
	bool handleKeyDownDirs(int c);

	void speak();

	void speak(Tasks s, Task* t) { tasks.add(s, t); speak(); }
	void speak(Tasks s, const string& msg) { tasks.add(s, new StringTask(msg)); speak(); }

	virtual void on(QueueManagerListener::Added, QueueItem* aQI) throw();
	virtual void on(QueueManagerListener::Moved, QueueItem* aQI, const string& oldTarget) throw();
	virtual void on(QueueManagerListener::Removed, QueueItem* aQI) throw();
	virtual void on(QueueManagerListener::SourcesUpdated, QueueItem* aQI) throw();
	virtual void on(QueueManagerListener::StatusUpdated, QueueItem* aQI) throw() { on(QueueManagerListener::SourcesUpdated(), aQI); }

	void initContentTree();
	void initDirTree();
	void initQueueMenu();
	void initDirMenu();
	bool makeNestedMenu(QMenu *parentMenu, const QueueItem::SourceList& sources, bool offlineNotImportant);

	QMenu *queueMenu;
	QMenu *columnMenu;
	QMenu *fileListMenu;
	QMenu *privateMenu;
	QMenu *reAddSourceMenu;
	QMenu *removeSourceMenu;
	QMenu *removeUserFromQueueMenu;
	QMenu *dirMenu;

	QAction *fileListMenuAction;
	QAction *privateMenuAction;
	QAction *reAddSourceMenuAction;
	QAction *removeSourceMenuAction;
	QAction *removeUserFromQueueMenuAction;
	

	QList<QAction*> fantomActions;

	void setStatus(int s, const tstring& text);
	
	void insertFile(QueueItemInfo* qii);
	void deleteFile(QueueItemInfo* qii);
	void updateFile(QueueItemInfo* qii);
	int findFile(QueueItemInfo* qii);
	void clearContent();
	void resortFile();

	QStandardItem* insertDir(QStandardItem* parentItem, DirItemInfo* dii);
	void eraseDir(QStandardItem* item);

	QStandardItem* getNextSibling(const QStandardItem* item) const;
	QStandardItem* getChild(const QStandardItem* item) const;
	
	//QStandardItem *parentDirItem;

	QStandardItemModel *contentModel;
	QStandardItemModel *dirModel;
	dlqueueSortingModel *proxyContentModel;

	// Use QMap for replacing TypedListView of original client
	QMap<QStandardItem*, DirItemInfo*> dirMap;
	QMap<QStandardItem*, QueueItemInfo*> contentMap;
	
	static QIcon folderIcon;
	static QIcon fileIcon;

//	int test_count;

signals:
	void speakerSignal();

private slots:
	void slotSpeaker();
	void updateFiles(QModelIndex index);
	
	void actionSearchAlt();
	void actionBitzi();
	void actionCopyMagnetToClipboard();
	void actionMoveRename();
	void actionMoveRenameDir();
	void actionSendPM(QAction *);
	void actionGetFilelist(QAction *);
	void actionReaddSource(QAction *);
	void actionRemoveSource(QAction *);
	void actionRemoveUserFromQueue(QAction *);
	void actionRemove();

	void showQueueMenu(const QPoint &point);
	void showDirMenu(const QPoint &point);

	void slotChangeProirity(QAction *action);
};


class dlqueueSortingModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
	dlqueueSortingModel(QObject *parent = 0): QSortFilterProxyModel(parent){};
	
protected:
//	bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;

private:
//	int64_t getSize(const QModelIndex &index) const;
};

#endif // __DL_QUEUE_H__
