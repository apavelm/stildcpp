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

#ifndef __FAVHUBLSTWIN_H__
#define __FAVHUBLSTWIN_H__

#include "mdi_c.h"
#include "stilutils.h"

#include "ui_favhublist.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/forward.h"
#include "client/FavoriteManagerListener.h"
#include "client/FavoriteManager.h"
//

#include <QtDebug>
#include <QtGui>
#include <QList>

using namespace std;
using namespace dcpp;


class FavoriteHubListWindow : public MdiChild
					, private Ui::mdiFAVHUBwin
					, private dcpp::FavoriteManagerListener
{
	Q_OBJECT
private:
	enum
	{
		COLUMN_FIRST,
		COLUMN_NAME = COLUMN_FIRST,
		COLUMN_DESCRIPTION,
		COLUMN_NICK,
		COLUMN_PASSWORD,
		COLUMN_SERVER,
		COLUMN_USERDESCRIPTION,
		COLUMN_LAST
	};
	
	bool startup;
	
	QList<FavoriteHubEntryPtr> datalist;
	QList<QModelIndex> datalistitem;
	void addEntry(const dcpp::FavoriteHubEntryPtr entry);
	
	// Listner
	virtual void on(FavoriteAdded, const FavoriteHubEntryPtr e) throw();
	virtual void on(FavoriteRemoved, const FavoriteHubEntryPtr e) throw();
public:
	FavoriteHubListWindow(QWidget *parent);
	~FavoriteHubListWindow();
private slots:
	void slot_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void listkeyPressEvent(QKeyEvent *e);
	void slot_autoconn(int);
	void slot_Add();
	void slot_Connect();
	void slot_Remove();
	void slotStore_1();
	void slotStore_2();
	void slotStore_3();
	void slotStore_4();
	void slotStore_5();
	void slotStore_6();
	void slotKey_1(QKeyEvent *);
	void slotKey_2(QKeyEvent *);
	void slotKey_4(QKeyEvent *);
	void slotKey_5(QKeyEvent *);
	void slotKey_6(QKeyEvent *);
};

#endif // __FAVHUBLSTWIN_H__
