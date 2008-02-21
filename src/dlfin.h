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

#ifndef __DLFIN_H__
#define __DLFIN_H__

#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"

#include "ui_dl_finished.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/File.h"
#include "client/FinishedManager.h"
#include "client/TimerManager.h"

#include <QtGui>
#include <QtCore>

using namespace std;
using namespace dcpp;

class DownloadFinishedWindow : public MdiChild, private Ui::mdi_DLfinished, private FinishedManagerListener
{
	Q_OBJECT
public:
		enum Status {
		STATUS_STATUS,
		STATUS_COUNT,
		STATUS_BYTES,
		STATUS_SPEED,
		STATUS_LAST
	};
	
	DownloadFinishedWindow(QWidget *parent);
	~DownloadFinishedWindow();
private:
	QMenu * columnMenu;
	QMenu * cnxtMenu;
	class ItemInfo;
	QList<ItemInfo*> datalist;
	QList<QTreeWidgetItem*> datalistitem;
	
	enum {
		SPEAK_ADD_LINE,
		SPEAK_REMOVE,
		SPEAK_REMOVE_ALL
	};

	enum {
		COLUMN_FIRST,
		COLUMN_FILE = COLUMN_FIRST,
		COLUMN_DONE,
		COLUMN_PATH,
		COLUMN_NICK,
		COLUMN_HUB,
		COLUMN_SIZE,
		COLUMN_SPEED,
		COLUMN_CRC32,
		COLUMN_LAST
	};

	static int columnSizes[COLUMN_LAST];
	static const char* columnNames[COLUMN_LAST];
	
	int64_t totalBytes, totalTime;
	
	class ItemInfo : public FastAlloc<ItemInfo> {
	public:
		ItemInfo(FinishedItemPtr entry_) : entry(entry_) {
			columns[COLUMN_FILE] = Text::toT(Util::getFileName(entry->getTarget()));
			columns[COLUMN_DONE] = Text::toT(Util::formatTime("%Y-%m-%d %H:%M:%S", entry->getTime()));
			columns[COLUMN_PATH] = Text::toT(Util::getFilePath(entry->getTarget()));
			columns[COLUMN_NICK] = Text::toT(entry->getUser());
			columns[COLUMN_HUB] = Text::toT(entry->getHub());
			columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(entry->getSize()));
			columns[COLUMN_SPEED] = Text::toT(Util::formatBytes(entry->getAvgSpeed()) + "/s");
			columns[COLUMN_CRC32] = entry->getCrc32Checked() ? T_("Yes") : T_("No");
		}

		FinishedItemPtr entry;

		const tstring& getText(int col) const {
			return columns[col];
		}
		QIcon getImage() const {
			return QIcon();
		}
		
		static int compareItems(ItemInfo* a, ItemInfo* b, int col) {
			switch(col) {
				case COLUMN_SIZE: return compare(a->entry->getSize(), b->entry->getSize());
				case COLUMN_SPEED: return compare(a->entry->getAvgSpeed(), b->entry->getAvgSpeed());
				default: return QString::compare(StilUtils::TstrtoQ(a->columns[col]), StilUtils::TstrtoQ(b->columns[col]),  Qt::CaseInsensitive) ;
			}

		}

		void openFile() 
		{
			//WinUtil::openFile(Text::toT(entry->getTarget()));
		}

		void openFolder() 
		{
			QUrl url = QUrl(StilUtils::TstrtoQ(Text::toT(entry->getTarget()))).toLocalFile();
			QDesktopServices::openUrl(url);
		}

	private:
		tstring columns[COLUMN_LAST];
	};
	
	void addEntry(FinishedItemPtr entry);
	void updateStatus();
	void updateList(const FinishedItemList& fl);
	void clearList();
	
	void speak(int a, FinishedItemPtr b = 0) { emit sigSpeak(a, b); }
	
	// Listeners
	virtual void on(Added, bool upload, FinishedItemPtr entry) throw();
	virtual void on(Removed, bool upload, FinishedItemPtr entry) throw();
	virtual void on(RemovedAll, bool upload) throw();

private slots:
	void slotSpeak(int, FinishedItemPtr);
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void showCnxtMenu(const QPoint& point);
	void makeContextMenu();
	
	void slotViewAsText();
	void slotOpen();
	void slotOpenFolder();
	void slotRemove();
	void slotRemoveAll();
signals:
	void sigSpeak(int, FinishedItemPtr);
};

#endif // __DLFIN_H__
