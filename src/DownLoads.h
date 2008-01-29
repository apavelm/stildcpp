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


#ifndef __MDI_DOWNLOADS_H__
#define __MDI_DOWNLOADS_H__

#include "config.h"
#include "stilutils.h"
#include "mdi_c.h"
#include "widgets/stil_qdltreewidget.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Text.h"
#include "client/DownloadManagerListener.h"
#include "client/UploadManagerListener.h"
#include "client/ConnectionManagerListener.h"
#include "client/FavoriteManagerListener.h"
#include "client/TaskQueue.h"
#include "client/forward.h"
#include "client/Util.h"
#include "client/Download.h"
#include "client/Upload.h"
#include "client/ResourceManager.h"
#include "client/SettingsManager.h"
#include "client/ConnectionManager.h"
#include "client/DownloadManager.h"
#include "client/UploadManager.h"
#include "client/QueueManager.h"
#include "client/ClientManager.h"
//

#include <QtGui>
#include <QtCore>

#include "ui_DownLoads.h"

using namespace std;
using namespace dcpp;

class DownLoadsWindow : public MdiChild, private Ui::mdiDownLoads,
	public dcpp::DownloadManagerListener, 
	public dcpp::QueueManagerListener
{
	Q_OBJECT
public:
	enum Status {
		STATUS_STATUS,
		STATUS_LAST
	};

	DownLoadsWindow(QWidget *parent);
	~DownLoadsWindow();
private:

	enum {
		COLUMN_FIRST,
		COLUMN_FILE = COLUMN_FIRST,
		COLUMN_PATH,
		COLUMN_STATUS,
		COLUMN_TIMELEFT,
		COLUMN_SPEED,
		COLUMN_DONE,
		COLUMN_SIZE,
		COLUMN_LAST
	};
	
	enum {
		SPEAKER_DISCONNECTED,
		SPEAKER_REMOVED,
		SPEAKER_TICK
	};
	
	struct TickInfo {
		TickInfo(const string& path_) : path(path_), done(0), bps(0), users(0) { }
		
		string path;
		int64_t done;
		double bps;
		int users;
	};

	class DownloadInfo {
	public:
		DownloadInfo(const string& filename, int64_t size, const TTHValue& tth);
		
		const tstring& getText(int col) const {
			return columns[col];
		}

		static int compareItems(DownloadInfo* a, DownloadInfo* b, int col) {
			switch(col) {
			case COLUMN_STATUS: return compare(a->users, b->users);
			case COLUMN_TIMELEFT: return compare(a->timeleft(), b->timeleft());
			case COLUMN_SPEED: return compare(a->bps, b->bps);
			case COLUMN_SIZE: return compare(a->size, b->size);
			case COLUMN_DONE: return compare(a->done, b->done);
			default: return QString::compare(StilUtils::TstrtoQ(a->columns[col]), StilUtils::TstrtoQ(b->columns[col]), Qt::CaseInsensitive);
			}
		}
		
		void _update();
		void _update(const TickInfo& ti);
		
		int64_t timeleft() { return bps == 0 ? 0 : (size - done) / bps; }
		string path;
		int64_t done;
		int64_t size;
		double bps;
		int users;
		TTHValue tth;
		
		tstring columns[COLUMN_LAST];
	};
	
	static int columnSizes[COLUMN_LAST];
	static int columnIndexes[COLUMN_LAST];
	
	bool startup;
	
	QList<DownloadInfo *> datalist;
	QList<QModelIndex> datalistitem;
	const QString convTick2QString(TickInfo *);
	TickInfo convQString2Tick(const QString &);
	
	int find(const string& path);

	//bool handleContextMenu(SmartWin::ScreenCoordinate pt);
	//LRESULT handleSpeaker(WPARAM wParam, LPARAM lParam);
	
	virtual void on(DownloadManagerListener::Tick, const DownloadList&) throw();
	virtual void on(DownloadManagerListener::Complete, Download*) throw();
	virtual void on(DownloadManagerListener::Failed, Download*, const string&) throw();

	virtual void on(QueueManagerListener::Removed, QueueItem*) throw();
private slots:
	void slotSpeak(int , const QString &);
signals:
	void sigSpeak(int , const QString &);
};

#endif // __MDI_DOWNLOADS_H__
