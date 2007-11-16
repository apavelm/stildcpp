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

#ifndef __HUB_WIN_H__
#define __HUB_WIN_H__

#include <QFile>
#include <QUrl>
#include <QtDebug>

#include "highlighter.h"
#include "mdi_c.h"
#include "config.h"
#include "widgets/iconset.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Client.h"
#include "client/forward.h"
#include "client/ClientListener.h"
#include "client/ClientManager.h"
#include "client/TaskQueue.h"
#include "client/User.h"
#include "client/FavoriteManagerListener.h"
#include "client/FavoriteManager.h"
//

#include "ui_HUBWindow.h"

class HubWindow : public MdiChild, private Ui::mdiHUBwin
	,private dcpp::ClientListener 
	,private dcpp::FavoriteManagerListener
{
	Q_OBJECT
private:
	Highlighter *highlighter;
	
	dcpp::Client* client;
	
// FavoriteManagerListener
//	virtual void on(dcpp::FavoriteManagerListener::UserAdded, const FavoriteUser& /*aUser*/) throw();
//	virtual void on(dcpp::FavoriteManagerListener::UserRemoved, const FavoriteUser& /*aUser*/) throw();
//	void resortForFavsFirst(bool justDoIt = false);

// ClientListener
	//virtual void on(Connecting, dcpp::Client*) throw();
	//virtual void on(Connected, dcpp::Client*) throw();
//	virtual void on(UserUpdated, Client*, const OnlineUser&) throw();
//	virtual void on(UsersUpdated, Client*, const OnlineUserList&) throw();
//	virtual void on(ClientListener::UserRemoved, Client*, const OnlineUser&) throw();
	//virtual void on(Redirect, dcpp::Client*, const std::string&) throw();
	//virtual void on(Failed, dcpp::Client*, const std::string&) throw();
//	virtual void on(GetPassword, Client*) throw();
//	virtual void on(HubUpdated, Client*) throw();
//	virtual void on(Message, dcpp::Client*, const OnlineUser&, const string&) throw();
//	virtual void on(StatusMessage, Client*, const string&) throw();
//	virtual void on(PrivateMessage, Client*, const OnlineUser&, const OnlineUser&, const OnlineUser&, const string&) throw();
//	virtual void on(NickTaken, Client*) throw();
//	virtual void on(SearchFlood, Client*, const string&) throw();
public:
	HubWindow(QWidget *parent, const dcpp::tstring& url);
	~HubWindow();
	void setupeditor();
	void ReConnect() { client->reconnect(); }
	bool isConnected() { return client->isConnected(); }
	/*
	enum Status 
	{
		STATUS_STATUS,
		STATUS_USERS,
		STATUS_SHARED,
		STATUS_SHOW_USERS,
		STATUS_LAST
	};
	*/

	//static void closeDisconnected();
	//static void resortUsers();
private slots:
	void sendFunc();
};

#endif // __HUB_WIN_H__
