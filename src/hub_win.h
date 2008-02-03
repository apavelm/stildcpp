/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *   Copyright (C) 2007 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)
 *                                                                         *
 *	 Thanks to Jacek Sieka (arnetheduck on gmail point com) for original   *
 *   code from original client (dcplusplus)                                *
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
//#include <QEvent>

#include "userlist.h"
#include "highlighter.h"
#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"
#include "iconset.h"
#include "textutil.h"
#include "UserInfoBase.h"
#include "stilutils.h"
#include "useractmenu.h"
#include "mainwindowimpl.h"
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

using namespace dcpp;

class HoldRedraw
{
public:
	HoldRedraw(QWidget *w_, bool reallyHold = true) : w(w_)
	{
		if(reallyHold)
			w -> setUpdatesEnabled(false);	 
		else
			w = 0;
	}
	
	~HoldRedraw()
	{
		if(w) w -> setUpdatesEnabled(true);
	}
	
private:
	QWidget* w;
};

class HubWindow : public MdiChild, private Ui::mdiHUBwin
	,private dcpp::ClientListener 
	,private dcpp::FavoriteManagerListener
{
	Q_OBJECT
private:
	UserActionMenu * usrActionMenu;
	Highlighter *highlighter;
	
	dcpp::Client* client;
	
	//enum QEvent { HubEvent = 1001};
	
	enum { MAX_CLIENT_LINES = 5 };
	TStringList lastLinesList;
	tstring lastLines;
	
	enum {
		IMAGE_USER = 0, IMAGE_OP
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
	
	enum {
		COLUMN_FIRST,
		COLUMN_NICK = COLUMN_FIRST,
		COLUMN_SHARED,
		COLUMN_DESCRIPTION,
		COLUMN_TAG,
		COLUMN_CONNECTION,
		COLUMN_IP,
		COLUMN_EMAIL,
		COLUMN_CID,
		COLUMN_LAST
	};
	
	enum Tasks { UPDATE_USER_JOIN, UPDATE_USER, REMOVE_USER, ADD_CHAT_LINE,
		ADD_STATUS_LINE, ADD_SILENT_STATUS_LINE, SET_TAB_AND_WINDOW_TITLE, SET_TAB_TOOLTIP, 
		GET_PASSWORD, PRIVATE_MESSAGE, CONNECTED, DISCONNECTED, FOLLOW
	};
	
	struct UserTask : public Task {
		UserTask(const OnlineUser& ou);

		UserPtr user;
		Identity identity;
	};
	
	struct PMTask : public StringTask {
		PMTask(const OnlineUser& from_, const OnlineUser& to_, const OnlineUser& replyTo_, const string& m);
		
		UserPtr from;
		UserPtr to;
		UserPtr replyTo;

		bool hub;
		bool bot;
	};
	
	class UserInfo : public UserInfoBase, public FastAlloc<UserInfo> 
	{ 
		public:
			UserInfo(const UserTask& u) : UserInfoBase(u.user) {
				update(u.identity, -1);
			}

			const tstring& getText(int col) const {
				return columns[col];
			}
			int getImage() const;
	
			static int compareItems(const UserInfo* a, const UserInfo* b, int col);
			bool update(const Identity& identity, int sortCol);

			string getNick() const { return identity.getNick(); }
			bool isHidden() const { return identity.isHidden(); }

			tstring columns[COLUMN_LAST];
			GETSET(Identity, identity, Identity);
		};
	
	tstring getStatusUsers() const;
	tstring getStatusShared() const;
	void initSecond();
	
	bool matchFilter(const UserInfo& ui, int sel, bool doSizeCompare = false, FilterModes mode = NONE, int64_t size = 0);
	bool parseFilter(FilterModes& mode, int64_t& size);
	bool updateUser(const UserTask& u);
	void removeUser(const UserPtr& aUser);
	tstring filterString;
	string redirect;
	string url;
	typedef unordered_map<UserPtr, UserInfo*, User::Hash> UserMap;
	typedef UserMap::iterator UserMapIter;
	UserMap userMap;
	TaskQueue tasks;
	bool updateUsers;
	bool resort;
	bool showJoins;
	bool favShowJoins;
	bool timeStamps;
	bool waitingForPW;
	/*
	typedef TypedListView<HubWindow, UserInfo, false> WidgetUsers;
	typedef WidgetUsers* WidgetUsersPtr;
	WidgetUsersPtr users;
	*/
	void clearUserList();
	void clearTaskList();
	const tstring& getNick(const UserPtr& u);
	void updateUserList(UserInfo* ui = NULL);
	//void handleSpeaker();//Tasks s, const OnlineUser& u);
	void handleFollow();
	void addChat(const tstring& aLine);
	void addStatus(const tstring& aLine, bool inChat = true);
	
			
	struct CountAvailable {
		CountAvailable() : available(0) { }
		int64_t available;
		void operator()(UserInfo *ui) {
			available += ui->getIdentity().getBytesShared();
		}
		void operator()(UserMap::const_reference ui) {
			available += ui.second->getIdentity().getBytesShared();
		}
	};
	
	void setStatus(int s, const tstring& text);
	void updateStatus();
	//FIXME - delete, it's hack 
	void setDirty(SettingsManager::IntSetting setting) {}

	
	bool speak();// { return true; }
	void speak(Tasks s) { tasks.add(s, 0); speak(); }
	void speak(Tasks s, const string& msg) { tasks.add(s, new StringTask(msg)); speak(); }
	void speak(Tasks s, const OnlineUser& u) { tasks.add(s, new UserTask(u)); updateUsers = true; }
	void speak(const OnlineUser& from, const OnlineUser& to, const OnlineUser& replyTo, const string& line) { tasks.add(PRIVATE_MESSAGE, new PMTask(from, to, replyTo, line));  speak(); }
	
	//void customEvent(QEvent *event);
// FavoriteManagerListener
//	virtual void on(dcpp::FavoriteManagerListener::UserAdded, const FavoriteUser& /*aUser*/) throw();
//	virtual void on(dcpp::FavoriteManagerListener::UserRemoved, const FavoriteUser& /*aUser*/) throw();
//	void resortForFavsFirst(bool justDoIt = false);

// ClientListener
	virtual void on(Connecting, dcpp::Client*) throw();
	virtual void on(Connected, dcpp::Client*) throw();
	virtual void on(UserUpdated, Client*, const OnlineUser&) throw();
	virtual void on(UsersUpdated, Client*, const OnlineUserList&) throw();
	virtual void on(ClientListener::UserRemoved, Client*, const OnlineUser&) throw();
	virtual void on(Redirect, dcpp::Client*, const std::string&) throw();
	virtual void on(Failed, dcpp::Client*, const std::string&) throw();
	virtual void on(GetPassword, Client*) throw();
	virtual void on(HubUpdated, Client*) throw();
	virtual void on(Message, dcpp::Client*, const OnlineUser&, const string&, bool thirdPerson) throw();
	virtual void on(StatusMessage, Client*, const string&) throw();
	virtual void on(PrivateMessage, Client*, const OnlineUser&, const OnlineUser&, const OnlineUser&, const string&, bool thirdPerson) throw();
	virtual void on(NickTaken, Client*) throw();
	virtual void on(SearchFlood, Client*, const string&) throw();
	
	// Probably is hack, to remove in future
	QMap<int, QString> userIconsMap;
	HubUserList *UList;
	void initUserList();
	void initUserListIcons();
	void initIconset();
	// Create UserActionMenu
	void initUserActionMenu();
	void setHubEncoding();
	
public:
	HubWindow(QWidget *parent, const dcpp::tstring& url);
	virtual ~HubWindow();
	void setupeditor();
	void ReConnect() { client->reconnect(); setHubEncoding(); }
	bool isConnected() { return client->isConnected(); }
	
	enum Status 
	{
		STATUS_STATUS,
		STATUS_USERS,
		STATUS_SHARED,
		STATUS_SHOW_USERS,
		STATUS_LAST
	};

	void insertUser(UserInfo *ui);
	void eraseUser(UserInfo *ui);
	int  findUser(UserInfo *ui);
	void updateSingleUser(int pos, UserInfo *ui);
	void clearUser();
	//friend class HubUserList;
	//static void closeDisconnected();
	//static void resortUsers();
signals:
	void speakerSignal();
private slots:
	void sendFunc(const QString &);
	void addnicktext( const QString & );
	bool eachSecond();
	void handleSpeaker();
};

#endif // __HUB_WIN_H__
