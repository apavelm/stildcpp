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

#ifndef __WAITUSR_H__
#define __WAITUSR_H__

#include "mdi_c.h"
#include "config.h"
#include "defs.h"
#include "stilutils.h"

#include "ui_waitingusers.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/UploadManagerListener.h"
#include "client/Client.h"
#include "client/QueueManager.h"
#include "client/FavoriteManager.h"
#include "client/UploadManager.h"

#include <QtGui>
#include <QtCore>

using namespace std;
using namespace dcpp;

class WaitingUsersWindow : public MdiChild, private Ui::mdi_wait_users, public UploadManagerListener 
{
	Q_OBJECT
public:
	WaitingUsersWindow(QWidget *parent);
	~WaitingUsersWindow();
private:
	enum
	{
		SPEAK_ADD_FILE,
		SPEAK_REMOVE_USER
	};

	QMenu * cnxtMenu;
	QList<UserPtr> datalist;
	QList<QString> datalist2;
	QList<QTreeWidgetItem*> datalistitem;
	
	// Communication with manager
	//void updateSearch(int index, bool doDelete = true);

	// UploadManagerListener
	virtual void on(UploadManagerListener::WaitingRemoveUser, const UserPtr&) throw();
	virtual void on(UploadManagerListener::WaitingAddFile, const UserPtr&, const string&) throw();
protected:
	void onRemoveUser(const UserPtr&);
	void onAddFile(const UserPtr&, const QString&);
	
	void makeContextMenu();
	UserPtr getSelectedUser();
	
	void speak(int a, UserPtr b, QString c) { emit sigSpeak(a, b, c); }
private slots:
	void slotSpeak(int, UserPtr, QString);
	void showCnxtMenu(const QPoint& point);
	
	void onGetList();
	void onCopyFilename();
	void onRemove();
	void onPrivateMessage();
	void onGrantSlot();
	void onAddToFavorites();
	void onKeyPress(QKeyEvent *);
signals:
	void sigSpeak(int, UserPtr, QString);
};

#endif
