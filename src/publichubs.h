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

#ifndef __PUBHUB_H__
#define __PUBHUB_H__

#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"

#include "publichub_dlg.h"
#include "ui_pubhub.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/HubEntry.h"
#include "client/FavoriteManagerListener.h"
#include "client/FavoriteManager.h"
#include "client/version.h"

#include <QtGui>
#include <QtCore>
#include <QHeaderView>

class PublicHubWindow : public MdiChild, private Ui::mdi_PublicHub 
	,public FavoriteManagerListener
{
	Q_OBJECT
public:
	PublicHubWindow(QWidget *parent);
	~PublicHubWindow();
	
	enum Status {
		STATUS_STATUS,
		STATUS_HUBS,
		STATUS_USERS,
		STATUS_LAST
	};

private:

	enum {
		COLUMN_FIRST,
		COLUMN_NAME = COLUMN_FIRST,
		COLUMN_DESCRIPTION,
		COLUMN_USERS,
		COLUMN_SERVER,
		COLUMN_COUNTRY,
		COLUMN_SHARED,
		COLUMN_MINSHARE,
		COLUMN_MINSLOTS,
		COLUMN_MAXHUBS,
		COLUMN_MAXUSERS,
		COLUMN_RELIABILITY,
		COLUMN_RATING,
		COLUMN_LAST
	};

	enum {
		FINISHED,
		LOADED_FROM_CACHE,
		STARTING,
		FAILED
	};

	enum FilterModes{
		NONE,
		EQUAL,
		GREATER_EQUAL,
		LESS_EQUAL,
		GREATER,
		LESS,
		NOT_EQUAL
	};
	
	class HubInfo {
	public:
		HubInfo(const HubEntry* entry_);
		
		static int compareItems(const HubInfo* a, const HubInfo* b, int col);
		const tstring& getText(int column) const { return columns[column]; }
		const HubEntry* entry;
		tstring columns[COLUMN_LAST];
	};
	
	int visibleHubs;
	int users;
	
	string filterString;

	HubEntryList entries;
	
	static int columnSizes[];

	void updateDropDown();
	void setStatus(Status, tstring);

	bool parseFilter(FilterModes& mode, double& size);
	bool matchFilter(const HubEntry& entry, const int& sel, bool doSizeCompare, const FilterModes& mode, const double& size);

	virtual void on(DownloadStarting, const string& l) throw() { speak(STARTING, l); }
	virtual void on(DownloadFailed, const string& l) throw() { speak(FAILED, l); }
	virtual void on(DownloadFinished, const string& l) throw() { speak(FINISHED, l); }
	virtual void on(LoadedFromCache, const string& l) throw() { speak(LOADED_FROM_CACHE, l); }

	void speak(int x, const string& l) { emit sigSpeak(x, Text::toT(l)); }
	
	bool checkNick();
	void insert(HubInfo *);
	void updateList() { updateList(filterSel->currentIndex()); }
	
	QMenu * cnxtMenu;
	QMenu * columnMenu;
	QList<HubInfo*> datalist;
	QList<QTreeWidgetItem*> datalistitem;
	
signals:
	void sigSpeak(int, tstring);
	
private slots:
	void slotSpeak(int, tstring);
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void showCnxtMenu(const QPoint& point);
	void makeContextMenu();
	
	void updateList(int);
	void slotRefresh();
	void slotConf();
	void slotListSelChanged(int);
	
	void slotConnect();
	void slotAddtoFav();
	void slotCopyHub();
	void slotOpen(QTreeWidgetItem *, int);
	void slotKeyPressed(QKeyEvent*);
	void slotFilterReturn();

};

#endif // __FAVHUBLSTWIN_H__
