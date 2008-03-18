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

#ifndef __PM_WIN_H__
#define __PM_WIN_H__

#include <QFile>

#include "highlighter.h"
#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"
#include "iconset.h"
#include "textutil.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Client.h"
#include "client/ClientListener.h"
#include "client/ClientManager.h"
#include "client/LogManager.h"
#include "client/UploadManager.h"
#include "client/FavoriteManager.h"
#include "client/User.h"
#include "client/StringTokenizer.h"
//

#include "ui_PMWindow.h"

using namespace std;
using namespace dcpp;

class PMWindow : public MdiChild, private Ui::mdiPMwin, private ClientManagerListener
{
	Q_OBJECT
private:
	Highlighter *highlighter;
	UserPtr replyTo;
	
	void readLog();
	void updateTitle();
	void speak() { emit sigSpeak(); }
	// ClientManagerListener
	virtual void on(ClientManagerListener::UserUpdated, const OnlineUser& aUser) throw();
	virtual void on(ClientManagerListener::UserConnected, const UserPtr& aUser) throw();
	virtual void on(ClientManagerListener::UserDisconnected, const UserPtr& aUser) throw();
	
public:
	PMWindow(QWidget *parent, const UserPtr& replyTo);
	~PMWindow();
	void setupeditor();
	bool isOnline();
	UserPtr getUser() const { return replyTo; }
	
	void sendMessage(const tstring& msg, bool thirdPerson = false) { ClientManager::getInstance()->privateMessage(replyTo, Text::fromT(msg), thirdPerson); }
	void addChat(const tstring& aLine);
	void addStatus(const tstring& aLine, bool inChat = true);
	
signals:
	void sigSpeak();
private slots:
	void send_pm_msg(const QString &);
	void slotSpeak();
};

// PRIVATE MESSAGE WINDOW

#endif // __PM_WIN_H__
