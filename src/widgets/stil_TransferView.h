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

#ifndef STIL_TRANSFER_VIEW_H
#define STIL_TRANSFER_VIEW_H

#include "../client/stdinc.h"
#include "../client/DCPlusPlus.h"
#include "../client/Text.h"
#include "../client/DownloadManagerListener.h"
#include "../client/UploadManagerListener.h"
#include "../client/ConnectionManagerListener.h"
#include "../client/FavoriteManagerListener.h"
#include "../client/TaskQueue.h"
#include "../client/forward.h"
#include "../client/Util.h"
#include "../client/Download.h"
#include "../client/Upload.h"

#include "../client/ResourceManager.h"
#include "../client/SettingsManager.h"
#include "../client/ConnectionManager.h"
#include "../client/DownloadManager.h"
#include "../client/UploadManager.h"
#include "../client/QueueManager.h"
#include "../client/ClientManager.h"

#include "../UserInfoBase.h"
#include "../stilutils.h"
#include "../config.h"
#include "stil_qdltreewidget.h"

#include "ui_transview.h"

#include <boost/scoped_ptr.hpp>

#include <QtGui>
#include <QtCore>


using namespace dcpp;
using namespace std;


class TransferView : public QWidget, private Ui::tr_view
	,private DownloadManagerListener
	,private UploadManagerListener
	,private ConnectionManagerListener
	,private QueueManagerListener
{
	Q_OBJECT
public:
	TransferView(QWidget *parent = 0);
	~TransferView();
	void preClose();
private:
	QMenu * cnxtMenu;
	QMenu * columnMenu1;
	QMenu * columnMenu2;
	
	class ConnectionInfo;
	class DownloadInfo;
	QList<ConnectionInfo*> datalist1;
	QList<DownloadInfo*> datalist2;
	QList<QTreeWidgetItem*> datalistitem1, datalistitem2;
	
	inline DownloadInfo* downloads_getData(int x) const { return datalist2[x]; }
	inline ConnectionInfo* connections_getData(int x) const { return datalist1[x]; }
	inline int connections_size() const { return datalist1.size(); }
	inline int downloads_size() const { return datalist2.size(); }
	void connections_erase(int x) { delete datalistitem1[x]; datalistitem1.removeAt(x); datalist1.removeAt(x); }
	void downloads_erase(int x) { delete datalistitem2[x]; datalistitem2.removeAt(x); datalist2.removeAt(x); }
	void connections_update(int x);
	void downloads_update(int x);
	void connections_insert(ConnectionInfo* ci);
	void downloads_insert(DownloadInfo* di);
	
	enum {
		DOWNLOAD_COLUMN_FIRST,
		DOWNLOAD_COLUMN_FILE = DOWNLOAD_COLUMN_FIRST,
		DOWNLOAD_COLUMN_PATH,
		DOWNLOAD_COLUMN_STATUS,
		DOWNLOAD_COLUMN_TIMELEFT,
		DOWNLOAD_COLUMN_SPEED,
		DOWNLOAD_COLUMN_DONE,
		DOWNLOAD_COLUMN_SIZE,
		DOWNLOAD_COLUMN_LAST
	};
	
	enum {
		DOWNLOADS_ADD_USER,
		DOWNLOADS_TICK,
		DOWNLOADS_REMOVE_USER,
		DOWNLOADS_REMOVED,
		CONNECTIONS_ADD,
		CONNECTIONS_REMOVE,
		CONNECTIONS_UPDATE
	};

	enum {
		CONNECTION_COLUMN_FIRST,
		CONNECTION_COLUMN_USER = CONNECTION_COLUMN_FIRST,
		CONNECTION_COLUMN_STATUS,
		CONNECTION_COLUMN_SPEED,
		CONNECTION_COLUMN_CHUNK,
		CONNECTION_COLUMN_TRANSFERED,
		CONNECTION_COLUMN_QUEUED,
		CONNECTION_COLUMN_CIPHER,
		CONNECTION_COLUMN_IP,
		CONNECTION_COLUMN_LAST
	};

	struct UpdateInfo;
	
	class ConnectionInfo : public UserInfoBase {
	public:
		enum Status {
			STATUS_RUNNING,	///< Transfering
			STATUS_WAITING		///< Idle
		};

		ConnectionInfo(const UserPtr& u, bool aDownload);

		bool download;
		bool transferFailed;
		
		Status status;
		
		int64_t actual;
		int64_t lastActual;
		int64_t transfered;
		int64_t lastTransfered;
		int64_t queued;
		int64_t speed;
		int64_t chunk;
		int64_t chunkPos;
		
		tstring columns[CONNECTION_COLUMN_LAST];
		void update(const UpdateInfo& ui);

		void disconnect();

		double getRatio() { return (transfered > 0) ? (double)actual / (double)transfered : 1.0; }

		const tstring& getText(int col) const {
			return columns[col];
		}

		static int compareItems(ConnectionInfo* a, ConnectionInfo* b, int col);
	};

	struct UpdateInfo : public Task {
		enum {
			MASK_STATUS = 1 << 0,
			MASK_STATUS_STRING = 1 << 1,
			MASK_SPEED = 1 << 2,
			MASK_TRANSFERED = 1 << 3,
			MASK_IP = 1 << 4,
			MASK_CIPHER = 1 << 5,
			MASK_CHUNK = 1 << 6
		};

		bool operator==(const ConnectionInfo& ii) { return download == ii.download && user == ii.user; }

		UpdateInfo(const UserPtr& aUser, bool isDownload, bool isTransferFailed = false) : updateMask(0), user(aUser), download(isDownload), transferFailed(isTransferFailed) { }

		uint32_t updateMask;

		UserPtr user;
		bool download;
		bool transferFailed;
		
		void setStatus(ConnectionInfo::Status aStatus) { status = aStatus; updateMask |= MASK_STATUS; }
		ConnectionInfo::Status status;
		void setTransfered(int64_t aTransfered, int64_t aActual) {
			transfered = aTransfered; actual = aActual; updateMask |= MASK_TRANSFERED; 
		}
		int64_t actual;
		int64_t transfered;
		void setSpeed(int64_t aSpeed) { speed = aSpeed; updateMask |= MASK_SPEED; }
		int64_t speed;
		void setStatusString(const tstring& aStatusString) { statusString = aStatusString; updateMask |= MASK_STATUS_STRING; }
		tstring statusString;
		void setChunk(int64_t aChunkPos, int64_t aChunk) { chunkPos = aChunkPos; chunk = aChunk; updateMask |= MASK_CHUNK; }
		int64_t chunkPos;
		int64_t chunk;
		
		void setIP(const tstring& aIp) { ip = aIp; updateMask |= MASK_IP; }
		tstring ip;
		void setCipher(const tstring& aCipher) { cipher = aCipher; updateMask |= MASK_CIPHER; }
		tstring cipher;
	};

	struct TickInfo : public Task {
		TickInfo(const string& path_) : path(path_), done(0), bps(0) { }
		
		string path;
		int64_t done;
		double bps;
	};

	static int connectionSizes[CONNECTION_COLUMN_LAST];
	
	class DownloadInfo {
	public:
		DownloadInfo(const string& filename, int64_t size, const TTHValue& tth);
		
		const tstring& getText(int col) const {
			return columns[col];
		}

		static int compareItems(DownloadInfo* a, DownloadInfo* b, int col) {
			switch(col) {
			case DOWNLOAD_COLUMN_STATUS: return compare(a->users, b->users);
			case DOWNLOAD_COLUMN_TIMELEFT: return compare(a->timeleft(), b->timeleft());
			case DOWNLOAD_COLUMN_SPEED: return compare(a->bps, b->bps);
			case DOWNLOAD_COLUMN_SIZE: return compare(a->size, b->size);
			case DOWNLOAD_COLUMN_DONE: return compare(a->done, b->done);
			default: return QString::compare(StilUtils::TstrtoQ(a->columns[col]), StilUtils::TstrtoQ(b->columns[col]),  Qt::CaseInsensitive) ;
			}
		}
		
		void update();
		void update(const TickInfo& ti);
		
		int64_t timeleft() { return bps == 0 ? 0 : (size - done) / bps; }
		string path;
		int64_t done;
		int64_t size;
		double bps;
		int users;
		TTHValue tth;
		
		tstring columns[DOWNLOAD_COLUMN_LAST];
	};
	

	static int downloadSizes[DOWNLOAD_COLUMN_LAST];

	TaskQueue tasks;
	StringMap ucLineParams;

	int find(const string& path);
	
	void makeContextMenu1();
	void makeContextMenu2();
	
	void speak() { emit sigSpeak(); }
	void speak(int type, UpdateInfo* ui) { tasks.add(type, ui); speak(); }
	void speak(int type, TickInfo* ti) { tasks.add(type, ti); speak(); }

// Listeners
	virtual void on(ConnectionManagerListener::Added, ConnectionQueueItem* aCqi) throw();
	virtual void on(ConnectionManagerListener::Failed, ConnectionQueueItem* aCqi, const string& aReason) throw();
	virtual void on(ConnectionManagerListener::Removed, ConnectionQueueItem* aCqi) throw();
	virtual void on(ConnectionManagerListener::StatusChanged, ConnectionQueueItem* aCqi) throw();

	virtual void on(DownloadManagerListener::Requesting, Download* aDownload) throw();
	virtual void on(DownloadManagerListener::Complete, Download* aDownload) throw();
	virtual void on(DownloadManagerListener::Failed, Download* aDownload, const string& aReason) throw();
	virtual void on(DownloadManagerListener::Starting, Download* aDownload) throw();
	virtual void on(DownloadManagerListener::Tick, const DownloadList& aDownload) throw();

	virtual void on(UploadManagerListener::Starting, Upload* aUpload) throw();
	virtual void on(UploadManagerListener::Tick, const UploadList& aUpload) throw();
	virtual void on(UploadManagerListener::Complete, Upload* aUpload) throw();

	virtual void on(QueueManagerListener::Removed, QueueItem*) throw();

	void onTransferComplete(Transfer* aTransfer, bool idDownload);
	void onTransferTick(Transfer* aTransfer, bool isDownload);
	void starting(UpdateInfo* ui, Transfer* t);
private slots:
	void slotSpeak();
	void chooseColumn1(QAction *action);
	void showColumnMenu1(const QPoint &point);
	void chooseColumn2(QAction *action);
	void showColumnMenu2(const QPoint &point);
	void showCnxtMenu1(const QPoint &point);
	void showCnxtMenu2(const QPoint &point);
	// handlers
	void handleForce();
	void handleAddToFav();
	void handleGetFL();
	void handleSearchAlternates();
	void handleBitzi();
	void handleMatch();
	void handleSendPM();
	void handleBrowse();
	void handleConHub();
	void handleGrantSlot();
	void handleRemoveUserQueue();
	void handleCopyNick();
	void handleCopyMagnet();
	void handleRemove();
	void runUserCommand(const UserCommand& uc);
	void handleDblClicked(const QModelIndex &);
	//
signals:
	void sigSpeak();
};

#endif
