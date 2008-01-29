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

#include <QClipboard>
#include <QtGui>
#include <QtCore>


using namespace dcpp;
using namespace std;


class TransferView : public QTreeWidget 
	,private dcpp::DownloadManagerListener
	,private dcpp::UploadManagerListener
	,private dcpp::ConnectionManagerListener
	//,private dcpp::FavoriteManagerListener
{
	Q_OBJECT
public:
	TransferView(QWidget *parent = 0);
	~TransferView();
	void preClose();
private:
	QMenu * cnxtMenu;
	QMenu * columnMenu;
	bool speak();
	
	enum {
		ADD_ITEM,
		REMOVE_ITEM,
		UPDATE_ITEM,
	};

	enum {
		COLUMN_FIRST,
		COLUMN_USER = COLUMN_FIRST,
		COLUMN_STATUS,
		COLUMN_SPEED,
		COLUMN_CHUNK,
		COLUMN_TRANSFERED,
		COLUMN_QUEUED,
		COLUMN_CIPHER,
		COLUMN_IP,
		COLUMN_LAST
	};

	struct UpdateInfo;
	class ItemInfo : public UserInfoBase {
	public:
		enum Status {
			STATUS_RUNNING,		///< Transfering
			STATUS_WAITING		///< Idle
		};

		ItemInfo(const UserPtr& u, bool aDownload);

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
		
		tstring columns[COLUMN_LAST];
		void update(const UpdateInfo& ui);

		void disconnect();

		double getRatio() { return (transfered > 0) ? (double)actual / (double)transfered : 1.0; }

		const tstring& getText(int col) const {
			return columns[col];
		}

		static int compareItems(ItemInfo* a, ItemInfo* b, int col);
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

		bool operator==(const ItemInfo& ii) { return download == ii.download && user == ii.user; }

		UpdateInfo(const UserPtr& aUser, bool isDownload, bool isTransferFailed = false) : updateMask(0), user(aUser), download(isDownload), transferFailed(isTransferFailed) { }

		uint32_t updateMask;

		UserPtr user;
		bool download;
		bool transferFailed;
		
		void setStatus(ItemInfo::Status aStatus) { status = aStatus; updateMask |= MASK_STATUS; }
		ItemInfo::Status status;
		void setTransfered(int64_t aTransfered, int64_t aActual) {
			transfered = aTransfered; actual = aActual; updateMask |= MASK_TRANSFERED; 
		}
		int64_t actual;
		int64_t transfered;
		void setSpeed(int64_t aSpeed) { speed = aSpeed; updateMask |= MASK_SPEED; }
		int64_t speed;
		void setStatusString(const tstring& aStatusString) { statusString = aStatusString; updateMask |= MASK_STATUS_STRING; }
		tstring statusString;
		void setChunk(int64_t aChunk) { chunk = aChunk; updateMask |= MASK_CHUNK; }
		int64_t chunk;
		
		void setIP(const tstring& aIp) { ip = aIp; updateMask |= MASK_IP; }
		tstring ip;
		void setCipher(const tstring& aCipher) { cipher = aCipher; updateMask |= MASK_CIPHER; }
		tstring cipher;
	};

	static int columnIndexes[];
	static int columnSizes[];

	TaskQueue tasks;
	StringMap ucLineParams;

	// workarounds
	void IIinsert(ItemInfo*);
	void IIupdate(int);
	void IIerase(int);
	
	QList<ItemInfo*> datalist;
	QList<QModelIndex> datalistitem;
	//
	
	void makeContextMenu();


	void speak(int type, UpdateInfo* ui) { tasks.add(type, ui); speak(); }
	ItemInfo * getItemInfoFromItem(QTreeWidgetItem *);


// Listeners
	virtual void on(ConnectionManagerListener::Added, ConnectionQueueItem* aCqi) throw();
	virtual void on(ConnectionManagerListener::Failed, ConnectionQueueItem* aCqi, const string& aReason) throw();
	virtual void on(ConnectionManagerListener::Removed, ConnectionQueueItem* aCqi) throw();
	virtual void on(ConnectionManagerListener::StatusChanged, ConnectionQueueItem* aCqi) throw();

	virtual void on(DownloadManagerListener::Complete, Download* aDownload) throw() { onTransferComplete(aDownload, false);}
	virtual void on(DownloadManagerListener::Failed, Download* aDownload, const string& aReason) throw();
	virtual void on(DownloadManagerListener::Starting, Download* aDownload) throw();
	virtual void on(DownloadManagerListener::Tick, const DownloadList& aDownload) throw();

	virtual void on(UploadManagerListener::Starting, Upload* aUpload) throw();
	virtual void on(UploadManagerListener::Tick, const UploadList& aUpload) throw();
	virtual void on(UploadManagerListener::Complete, Upload* aUpload) throw() { onTransferComplete(aUpload, true); }

	void onTransferComplete(Transfer* aTransfer, bool isUpload);
// FavoriteManager
	//void on(FavoriteManagerListener::UserAdded, const FavoriteUser& /*aUser*/) throw(); 
	//void on(FavoriteManagerListener::UserRemoved, const FavoriteUser& /*aUser*/) throw();
	
	void starting(UpdateInfo* ui, Transfer* t);
	
protected:
	void keyPressEvent(QKeyEvent *);
private slots:
	void slotSpeak();
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void showCnxtMenu(const QPoint &point);
	// handlers
	void handleForce();
	void handleAddToFav();
	void handleGetFL();
	void handleSearchAlternates();
	void handleCopyNick();
	void handleRemove();
	void runUserCommand(const UserCommand& uc);
	void handleDblClicked(const QModelIndex &);
signals:
	void sigSpeak();
};

#endif
