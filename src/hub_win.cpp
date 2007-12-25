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

#include "hub_win.h"

using namespace std;
using namespace dcpp;

static ResourceManager::Strings columnNames[] = { ResourceManager::NICK, ResourceManager::SHARED,
ResourceManager::DESCRIPTION, ResourceManager::TAG, ResourceManager::CONNECTION, ResourceManager::IP_BARE, ResourceManager::EMAIL, ResourceManager::CID };

HubWindow::~HubWindow()
{
	FavoriteManager::getInstance()->removeListener(this);
	client->removeListener(this);
	client->disconnect(true);
	ClientManager::getInstance()->putClient(client);
	clearUserList();
	clearTaskList();
}

HubWindow::HubWindow(QWidget *parent, const dcpp::tstring& url) : MdiChild(parent), updateUsers(false),
	timeStamps(BOOLSETTING(TIME_STAMPS)),
	showJoins(BOOLSETTING(SHOW_JOINS)),
	favShowJoins(BOOLSETTING(FAV_SHOW_JOINS))
{	
	setupUi(this);
	type = 1;
	idText = StilUtils::TstrtoQ(url);

	// UserList in LeftSide
	if (APPSETTING(i_HUBLEFTSIDE)) 
		{
			QWidget *w1 = splitter->widget(0);
			splitter->addWidget(w1);
		}
		
	initUserListIcons();
	initUserList();
	initIconset();
	connect(this, SIGNAL(speakerSignal()), this, SLOT(handleSpeaker()),Qt::QueuedConnection);
	initSecond();
	
	setWindowTitle(tr("Hub: ")+StilUtils::TstrtoQ(url));
	editor->clear();
	setupeditor();

	client = ClientManager::getInstance()->getClient(dcpp::Text::fromT(url));
	client->addListener(this);
	
	if (APPSTRING(s_DEFCHARSET) != "System")
		client->setEncoding(APPSTRING(s_DEFCHARSET).toStdString());
	
	client->connect();
	
	connect(lineEdit, SIGNAL( SendText(const QString &) ), this, SLOT(sendFunc(const QString &)) );
	
	FavoriteManager::getInstance()->addListener(this);
	
	editor->setHubAddress(idText);
	usrActionMenu = new UserActionMenu;
	connect(usrActionMenu, SIGNAL(sig_sendPublicMessage(const QString &)), this, SLOT(addnicktext(const QString &)) );
	editor->setUserMenu(usrActionMenu);
}

void HubWindow::addnicktext( const QString & t)
{
	QTextCursor cursor(lineEdit->document());
	int p = cursor.position();
	cursor.movePosition(QTextCursor::EndOfLine);
	cursor.beginEditBlock();
	cursor.insertText(t+": ");
	cursor.endEditBlock();
	cursor.setPosition(p);
}

void HubWindow::sendFunc(const QString &txt)
{
	if ( (client) && (!txt.isEmpty()) )
		client->hubMessage(Text::fromT(StilUtils::QtoTstr(txt)));
}

void HubWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(8);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void HubWindow::initSecond()
{
	//createTimer(std::tr1::bind(&HubFrame::eachSecond, this), 1000);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
	timer->start(1000);
}

bool HubWindow::eachSecond()
{

	if(updateUsers) {
		updateUsers = false;
		speak();
	}
	
	updateStatus();
	return true;
}

// Probably is hack, to remove in future
void HubWindow::initUserListIcons()
{
	userIconsMap[0]=":/images/status/green_16.png";
	userIconsMap[1]=":/images/status/yellow-green_16.png";
	userIconsMap[2]=":/images/status/green-star_16.png";
	userIconsMap[3]=":/images/status/yellow-green-star_16.png";
	userIconsMap[4]=":/images/status/red_16.png";
	userIconsMap[5]=":/images/status/yellow-red_16.png";
	userIconsMap[6]=":/images/status/red-star_16.png";
	userIconsMap[7]=":/images/status/yellow-red-star_16.png";
}

void HubWindow::initUserList()
{
	QStringList columns;
	foreach(tstring name, ResourceManager::getInstance()->getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
		
		
	userListView->setRootIsDecorated(false);
	userListView->setAlternatingRowColors(true);
	userListView->setSortingEnabled(true);
	
	criteriaSortBox -> addItem("Any");
	criteriaSortBox -> addItems(columns);
	criteriaSortBox -> setCurrentIndex(0);
	
	columns << "IsOp" << "Total Shared";
	UList = new HubUserList(userListView, filterLine, criteriaSortBox, columns);
	//userlist -> setHeaderLabels(columns);
}

void HubWindow::initIconset()
{
	Iconset *ic = new Iconset();

	ic->load("/home/irq/stildcpp/images/emotions/default.icp");
	ic->addToFactory();
}

tstring HubWindow::getStatusShared() const {
	
	int64_t available;
	/*if (users->getSelectedCount() > 1) {
		available = users->forEachSelectedT(CountAvailable()).available;
	} else {*/
		available = std::for_each(userMap.begin(), userMap.end(), CountAvailable()).available;
	//}
	return Text::toT(Util::formatBytes(available));
}

tstring HubWindow::getStatusUsers() const {
	
	size_t userCount = 0;
	for(UserMap::const_iterator i = userMap.begin(); i != userMap.end(); ++i){
		UserInfo* ui = i->second;
		if(!ui->isHidden())
			userCount++;
	}

	tstring textForUsers;
/*	if (users->getSelectedCount() > 1)
		textForUsers += Text::toT(Util::toString(users->getSelectedCount()) + "/");
	if (showUsers->getChecked() && users->size() < userCount)
		textForUsers += Text::toT(Util::toString(users->size()) + "/");
*/
	return textForUsers + Text::toT(Util::toString(userCount) + " " + STRING(HUB_USERS));
}

void HubWindow::updateStatus() {
	setStatus(STATUS_USERS, getStatusUsers());
	setStatus(STATUS_SHARED, getStatusShared());
}

void HubWindow::updateUserList(UserInfo* ui)
{
	int64_t size = -1;
	FilterModes mode = NONE;

	//int sel = filterType->getSelectedIndex();
	int sel = 8;

	bool doSizeCompare = parseFilter(mode, size);// && sel == COLUMN_SHARED;

	//single update?
	//avoid refreshing the whole list and just update the current item
	//instead
	if(ui != NULL) {
		if(ui->isHidden()) {
			return;
		}
		if(filterString.empty()) {
			/* ported
			if(users->find(ui) == -1) {
				users->insert(ui);
			*/
			if (findUser(ui) == -1)
				insertUser(ui);
		} else {
			if(matchFilter(*ui, sel, doSizeCompare, mode, size)) {
				/* ported
				//if(users->find(ui) == -1) {
					//users->insert(ui);
				*/
				if (findUser(ui) == -1)
					insertUser(ui);
			} else {
				//erase checks to see that the item exists in the list
				//unnecessary to do it twice.
				//users->erase(ui); //ported
				eraseUser(ui);
			}
		}
	} else {
		HoldRedraw hold(userListView);
		clearUser();
		//users->clear(); // -ported

		if(filterString.empty()) 
		{
			for(UserMapIter i = userMap.begin(); i != userMap.end(); ++i)
			{
				UserInfo* ui = i->second;
				if(!ui->isHidden()) //qDebug() << ui->getIdentity()->getNick();
				 //dcdebug("Update users: %s", ui ->  getNick().c_str());
				 insertUser(ui);
					//users->insert(i->second);
			}
		} 
		else 
		{
			for(UserMapIter i = userMap.begin(); i != userMap.end(); ++i) 
			{
				UserInfo* ui = i->second;
				if(!ui->isHidden() && matchFilter(*ui, sel, doSizeCompare, mode, size)) 
				{
					insertUser(ui);
					//users->insert(ui);
					dcdebug("Update users2: %s", ui ->  getNick().c_str());
				}
			}
		}
	}
}

bool HubWindow::matchFilter(const UserInfo& ui, int sel, bool doSizeCompare, FilterModes mode, int64_t size) {
	if(filterString.empty())
		return true;

	bool insert = false;
	if(doSizeCompare) {
		switch(mode) {
			case EQUAL: insert = (size == ui.getIdentity().getBytesShared()); break;
			case GREATER_EQUAL: insert = (size <= ui.getIdentity().getBytesShared()); break;
			case LESS_EQUAL: insert = (size >= ui.getIdentity().getBytesShared()); break;
			case GREATER: insert = (size < ui.getIdentity().getBytesShared()); break;
			case LESS: insert = (size > ui.getIdentity().getBytesShared()); break;
			case NOT_EQUAL: insert = (size != ui.getIdentity().getBytesShared()); break;
		}
	} else {
		if(sel >= COLUMN_LAST) {
			for(int i = COLUMN_FIRST; i < COLUMN_LAST; ++i) {
				if(Util::findSubString(ui.getText(i), filterString) != string::npos) {
					insert = true;
					break;
				}
			}
		} else {
			if(Util::findSubString(ui.getText(sel), filterString) != string::npos)
				insert = true;
		}
	}

	return insert;
}

bool HubWindow::parseFilter(FilterModes& mode, int64_t& size) {
	tstring::size_type start = static_cast<tstring::size_type>(tstring::npos);
	tstring::size_type end = static_cast<tstring::size_type>(tstring::npos);
	int64_t multiplier = 1;

	if(filterString.empty()) {
		return false;
	}
	if(filterString.compare(0, 2, _T(">=")) == 0) {
		mode = GREATER_EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, _T("<=")) == 0) {
		mode = LESS_EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, _T("==")) == 0) {
		mode = EQUAL;
		start = 2;
	} else if(filterString.compare(0, 2, _T("!=")) == 0) {
		mode = NOT_EQUAL;
		start = 2;
	} else if(filterString[0] == _T('<')) {
		mode = LESS;
		start = 1;
	} else if(filterString[0] == _T('>')) {
		mode = GREATER;
		start = 1;
	} else if(filterString[0] == _T('=')) {
		mode = EQUAL;
		start = 1;
	}

	if(start == tstring::npos)
		return false;
	if(filterString.length() <= start)
		return false;

	if((end = Util::findSubString(filterString, _T("TiB"))) != tstring::npos) {
		multiplier = 1024LL * 1024LL * 1024LL * 1024LL;
	} else if((end = Util::findSubString(filterString, _T("GiB"))) != tstring::npos) {
		multiplier = 1024*1024*1024;
	} else if((end = Util::findSubString(filterString, _T("MiB"))) != tstring::npos) {
		multiplier = 1024*1024;
	} else if((end = Util::findSubString(filterString, _T("KiB"))) != tstring::npos) {
		multiplier = 1024;
	} else if((end = Util::findSubString(filterString, _T("TB"))) != tstring::npos) {
		multiplier = 1000LL * 1000LL * 1000LL * 1000LL;
	} else if((end = Util::findSubString(filterString, _T("GB"))) != tstring::npos) {
		multiplier = 1000*1000*1000;
	} else if((end = Util::findSubString(filterString, _T("MB"))) != tstring::npos) {
		multiplier = 1000*1000;
	} else if((end = Util::findSubString(filterString, _T("kB"))) != tstring::npos) {
		multiplier = 1000;
	} else if((end = Util::findSubString(filterString, _T("B"))) != tstring::npos) {
		multiplier = 1;
	}

	if(end == tstring::npos) {
		end = filterString.length();
	}

	tstring tmpSize = filterString.substr(start, end-start);
	size = static_cast<int64_t>(Util::toDouble(Text::fromT(tmpSize)) * multiplier);

	return true;
}

void HubWindow::on(Connecting, Client*) throw() {
	speak(ADD_STATUS_LINE, STRING(CONNECTING_TO) + client->getHubUrl() + "...");
	speak(SET_TAB_AND_WINDOW_TITLE, client->getHubUrl());
}

void HubWindow::on(Connected, Client*) throw() {
	speak(CONNECTED);
}

void HubWindow::on(UserUpdated, Client*, const OnlineUser& user) throw() {
	CID cid = user.getUser()->getCID();
	speak(UPDATE_USER_JOIN, user);
}

void HubWindow::on(UsersUpdated, Client*, const OnlineUserList& aList) throw() {
	for(OnlineUserList::const_iterator i = aList.begin(); i != aList.end(); ++i) {
		tasks.add(UPDATE_USER, new UserTask(*(*i)));
	}
	updateUsers = true;
}

void HubWindow::on(ClientListener::UserRemoved, Client*, const OnlineUser& user) throw() {
	speak(REMOVE_USER, user);
}

void HubWindow::on(Redirect, Client*, const string& line) throw() {
	if(ClientManager::getInstance()->isConnected(line)) {
		speak(ADD_STATUS_LINE, STRING(REDIRECT_ALREADY_CONNECTED));
		return;
	}
	redirect = line;
	if(BOOLSETTING(AUTO_FOLLOW)) {
		speak(FOLLOW);
	} else {
		speak(ADD_STATUS_LINE, STRING(PRESS_FOLLOW) + line);
	}
}

void HubWindow::on(Failed, Client*, const string& line) throw() {
	speak(ADD_STATUS_LINE, line);
	speak(DISCONNECTED);
}

void HubWindow::on(GetPassword, Client*) throw() {
	speak(GET_PASSWORD);
}

void HubWindow::on(HubUpdated, Client*) throw() {
	string hubName = client->getHubName();
	if(!client->getHubDescription().empty()) {
		hubName += " - " + client->getHubDescription();
	}
	hubName += " (" + client->getHubUrl() + ")";
#ifdef _DEBUG
	string version = client->getHubIdentity().get("VE");
	if(!version.empty()) {
		hubName += " - " + version;
	}
#endif
	speak(SET_TAB_AND_WINDOW_TITLE, hubName);
	speak(SET_TAB_TOOLTIP, client->getHubDescription());
}

void HubWindow::on(Message, Client*, const OnlineUser& from, const string& msg) throw() {
	speak(ADD_CHAT_LINE, Util::formatMessage(from.getIdentity().getNick(), msg));
}

void HubWindow::on(StatusMessage, Client*, const string& line) throw() {
	if(SETTING(FILTER_MESSAGES)) {
		if((line.find("Hub-Security") != string::npos) && (line.find("was kicked by") != string::npos)) {
			// Do nothing...
		} else if((line.find("is kicking") != string::npos) && (line.find("because:") != string::npos)) {
			speak(ADD_SILENT_STATUS_LINE, Text::toDOS(line));
		} else {
			speak(ADD_CHAT_LINE, Text::toDOS(line));
		}
	} else {
		speak(ADD_CHAT_LINE, Text::toDOS(line));
	}
}

void HubWindow::on(PrivateMessage, Client*, const OnlineUser& from, const OnlineUser& to, const OnlineUser& replyTo, const string& line) throw() {
	speak(from, to, replyTo, Util::formatMessage(from.getIdentity().getNick(), line));
}

void HubWindow::on(NickTaken, Client*) throw() {
	speak(ADD_STATUS_LINE, STRING(NICK_TAKEN));
}

void HubWindow::on(SearchFlood, Client*, const string& line) throw() {
	speak(ADD_STATUS_LINE, STRING(SEARCH_SPAM_FROM) + line);
}

bool HubWindow::speak()
{
	//QApplication::postEvent(this, HubEvent);
	//handleSpeaker();
	emit speakerSignal();
	//qDebug() << "Speaked";
}

/*
void HubWindow::customEvent(QEvent *event)
{
	if ((int)event->type() == HubEvent)
	{
		handleSpeaker();
	}
}
*/

const tstring& HubWindow::getNick(const UserPtr& aUser) {
	UserMapIter i = userMap.find(aUser);
	if(i == userMap.end())
		return Util::emptyStringT;

	UserInfo* ui = i->second;
	return ui->columns[COLUMN_NICK];
}


bool HubWindow::updateUser(const UserTask& u) {
	UserMapIter i = userMap.find(u.user);
	if(i == userMap.end()) {
		UserInfo* ui = new UserInfo(u);
		userMap.insert(make_pair(u.user, ui));
		//if(!ui->isHidden() && showUsers->getChecked())
		//users->insert(ui);
		
		if(!ui->isHidden())
		{
			insertUser(ui);
		}

		if(!filterString.empty())
			updateUserList(ui);
		return true;
	} else {
		UserInfo* ui = i->second;
		if(!ui->isHidden() && u.identity.isHidden()) {
		//if(!ui->isHidden() && u.identity.isHidden() && showUsers->getChecked()) {
			//users->erase(ui);
			eraseUser(ui);
			dcdebug("User erased\n");
		}

		resort = ui->update(u.identity, -1) || resort;
		if(true){
		//if(showUsers->getChecked()) {
			int pos = findUser(ui);
			//ported
			//int pos = users->find(ui);
			if(pos != -1) {
				//ported and extended
				//users->update(pos);
				updateSingleUser(pos, ui);
			}
			updateUserList(ui);
		}

		return false;
	}
}

/*
void HubWindow::handleSpeaker(Tasks s, const OnlineUser& u)
{
	updateUsers = false;
	TaskQueue::List t;
	tasks.get(t);
}
*/


void HubWindow::addStatus(const tstring& aLine, bool inChat /* = true */) {
	tstring line = Text::toT("[" + Util::getShortTimeString() + "] ") + aLine;

	setStatus(STATUS_STATUS, line);

	while(lastLinesList.size() + 1 > MAX_CLIENT_LINES)
		lastLinesList.erase(lastLinesList.begin());
	lastLinesList.push_back(line);

	setDirty(SettingsManager::BOLD_HUB);

	if(BOOLSETTING(STATUS_IN_CHAT) && inChat) {
		addChat(_T("*** ") + aLine);
	}
	if(BOOLSETTING(LOG_STATUS_MESSAGES)) {
		StringMap params;
		client->getHubIdentity().getParams(params, "hub", false);
		params["hubURL"] = client->getHubUrl();
		client->getMyIdentity().getParams(params, "my", true);
		params["message"] = Text::fromT(aLine);
		LOG(LogManager::STATUS, params);
	}
}

void HubWindow::addChat(const tstring& aLine) {
	tstring line;
	if(timeStamps) {
#ifdef Q_OS_WIN
		line = Text::toT("\r\n[" + Util::getShortTimeString() + "] ");
#else
		line = Text::toT("[" + Util::getShortTimeString() + "] ");
#endif
	} else {
#ifdef Q_OS_WIN
		line = _T("\r\n");
#else
		line = _T("");
#endif
	}
	line += aLine;

	/*
	int limit = chat->getTextLimit();
	if(chat->length() + static_cast<int>(line.size()) > limit) {
		HoldRedraw hold(chat);
		chat->setSelection(0, chat->lineIndex(chat->lineFromChar(limit / 10)));
		chat->replaceSelection(_T(""));
	}
#ifdef PORT_ME	
	BOOL noscroll = TRUE;
	POINT p = ctrlClient.PosFromChar(ctrlClient.GetWindowTextLength() - 1);
	CRect r;
	ctrlClient.GetClientRect(r);

	if( r.PtInRect(p) || MDIGetActive() != m_hWnd)
		noscroll = FALSE;
	else {
		ctrlClient.SetRedraw(FALSE); // Strange!! This disables the scrolling...????
	}
#endif
*/
	if(BOOLSETTING(LOG_MAIN_CHAT)) {
		StringMap params;
		params["message"] = Text::fromT(aLine);
		client->getHubIdentity().getParams(params, "hub", false);
		params["hubURL"] = client->getHubUrl();
		client->getMyIdentity().getParams(params, "my", true);
		LOG(LogManager::CHAT, params);
	}
	//chat->addText(line); //ported

	QString chatLine(StilUtils::TstrtoQ(line));
	
#ifndef Q_OS_WIN
	chatLine.replace(QString("\r\n"),QString("\n"));
#endif
	chatLine = TextUtil::plain2rich(chatLine);
	chatLine = TextUtil::linkify(chatLine);
	if (APPSETTING(i_SHOWSMILES))
		chatLine = TextUtil::emoticonify(chatLine);
	chatLine = TextUtil::legacyFormat(chatLine);
	editor->appendText(chatLine);
	//dcdebug("Added to chat: %s\n", Text::fromT(line).c_str());

#ifdef PORT_ME
	if(noscroll) {
		ctrlClient.SetRedraw(TRUE);
	}
#endif
	setDirty(SettingsManager::BOLD_HUB);
}

void HubWindow::removeUser(const UserPtr& aUser) {
	UserMapIter i = userMap.find(aUser);
	if(i == userMap.end()) {
		// Should never happen?
		dcassert(i != userMap.end());
		return;
	}

	UserInfo* ui = i->second;
	/*
	if(!ui->isHidden() && showUsers->getChecked())
		users->erase(ui);
	*/ 
	if(!ui->isHidden()) 
		eraseUser(ui);
		//dcdebug("User removed\n");
	
	userMap.erase(i);
	delete ui;
}

void HubWindow::clearUserList() {
	//users->clear(); //ported
	clearUser();
	
	for(UserMapIter i = userMap.begin(); i != userMap.end(); ++i) {
		delete i->second;
	}
	userMap.clear();
}

void HubWindow::handleFollow() {
	if(!redirect.empty()) {
		if(ClientManager::getInstance()->isConnected(redirect)) {
			addStatus(TSTRING(REDIRECT_ALREADY_CONNECTED));
			return;
		}

		url = redirect;

		// the client is dead, long live the client!
		client->removeListener(this);
		ClientManager::getInstance()->putClient(client);
		clearUserList();
		clearTaskList();
		client = ClientManager::getInstance()->getClient(url);
		client->addListener(this);
		client->connect();
	}
}

void HubWindow::clearTaskList() {
	tasks.clear();
}

void HubWindow::handleSpeaker()//Tasks s, const OnlineUser& u)
{
	updateUsers = false;
	TaskQueue::List t;
	tasks.get(t);

	HoldRedraw hold(userListView);
	
	for(TaskQueue::Iter i = t.begin(); i != t.end(); ++i) {
		if(i->first == UPDATE_USER) {
			updateUser(*static_cast<UserTask*>(i->second));
		} else if(i->first == UPDATE_USER_JOIN) {
			UserTask& u = *static_cast<UserTask*>(i->second);
			if(updateUser(u)) {
				if (showJoins || (favShowJoins && FavoriteManager::getInstance()->isFavoriteUser(u.user))) {
					addStatus(_T("*** ") + TSTRING(JOINS) + Text::toT(u.identity.getNick()));
				}
			}
		} else if(i->first == REMOVE_USER) {
			UserTask& u = *static_cast<UserTask*>(i->second);
			removeUser(u.user);
			if (showJoins || (favShowJoins && FavoriteManager::getInstance()->isFavoriteUser(u.user))) {
				addStatus(Text::toT("*** " + STRING(PARTS) + u.identity.getNick()));
			}
		} else if(i->first == CONNECTED) {
			addStatus(TSTRING(CONNECTED));
#ifdef PORT_ME
			setTabColor(GREEN);
#endif
		} else if(i->first == DISCONNECTED) {
			clearUserList();
#ifdef PORT_ME
			setTabColor(RED);
#endif
		} else if(i->first == ADD_CHAT_LINE) {
			addChat(Text::toT(static_cast<StringTask*>(i->second)->str));
		} else if(i->first == ADD_STATUS_LINE) {
			addStatus(Text::toT(static_cast<StringTask*>(i->second)->str));
		} else if(i->first == ADD_SILENT_STATUS_LINE) {
			addStatus(Text::toT(static_cast<StringTask*>(i->second)->str), false);
		} else if(i->first == SET_TAB_AND_WINDOW_TITLE) {
			setTabText(StilUtils::TstrtoQ(Text::toT(static_cast<StringTask*>(i->second)->str)));
		} else if(i->first == SET_TAB_TOOLTIP) {
			setTabToolTip(StilUtils::TstrtoQ(Text::toT(static_cast<StringTask*>(i->second)->str)));
		} else if(i->first == GET_PASSWORD) {
			if(client->getPassword().size() > 0) {
				client->password(client->getPassword());
				addStatus(TSTRING(STORED_PASSWORD_SENT));
			} else {
				if(!BOOLSETTING(PROMPT_PASSWORD)) {
					dcdebug("Message settext: /password\n");
					/*
					message->setText(_T("/password "));
					message->setFocus();
					message->setSelection(10, 10);
					*/
					waitingForPW = true;
				} 
				/*else {
					LineDlg linePwd(this, TSTRING(ENTER_PASSWORD), TSTRING(ENTER_PASSWORD), Util::emptyStringT, true);
					if(linePwd.run() == IDOK) {
						client->setPassword(Text::fromT(linePwd.getLine()));
						client->password(Text::fromT(linePwd.getLine()));
						waitingForPW = false;
					} else {
						client->disconnect(true);
					}
				}*/
			}
		} else if(i->first == PRIVATE_MESSAGE) {
			PMTask& pm = *static_cast<PMTask*>(i->second);
			if(pm.hub) {
				if(BOOLSETTING(IGNORE_HUB_PMS)) {
					addStatus(TSTRING(IGNORED_MESSAGE) + Text::toT(pm.str), false);
				} /*else if(BOOLSETTING(POPUP_HUB_PMS) || PrivateFrame::isOpen(pm.replyTo)) {
					PrivateFrame::gotMessage(getParent(), pm.from, pm.to, pm.replyTo, Text::toT(pm.str));
				} else {*/
					addChat(TSTRING(PRIVATE_MESSAGE_FROM) + getNick(pm.from) + _T(": ") + Text::toT(pm.str));
				//}
			} else if(pm.bot) {
				if(BOOLSETTING(IGNORE_BOT_PMS)) {
					addStatus(TSTRING(IGNORED_MESSAGE) + Text::toT(pm.str), false);
				} /*else if(BOOLSETTING(POPUP_BOT_PMS) || PrivateFrame::isOpen(pm.replyTo)) {
					PrivateFrame::gotMessage(getParent(), pm.from, pm.to, pm.replyTo, Text::toT(pm.str));
				} else {*/
					addChat(TSTRING(PRIVATE_MESSAGE_FROM) + getNick(pm.from) + _T(": ") + Text::toT(pm.str));
				//}
			} else {/*
				if(BOOLSETTING(POPUP_PMS) || PrivateFrame::isOpen(pm.replyTo) || pm.from == client->getMyIdentity().getUser()) {
					PrivateFrame::gotMessage(getParent(), pm.from, pm.to, pm.replyTo, Text::toT(pm.str));
				} else {*/
					addChat(TSTRING(PRIVATE_MESSAGE_FROM) + getNick(pm.from) + _T(": ") + Text::toT(pm.str));
				//}
			}
		} else if(i->first == FOLLOW) {
			handleFollow();
		}
		delete i->second;
	}
	/*
	if(resort && showUsers->getChecked()) {
		users->resort();
		resort = false;
	}

	return 0;
	*/
}

bool HubWindow::UserInfo::update(const Identity& identity, int sortCol) {
	bool needsSort = (getIdentity().isOp() != identity.isOp());
	tstring old;
	if(sortCol != -1)
		old = columns[sortCol];

	columns[COLUMN_NICK] = Text::toT(identity.getNick());
	columns[COLUMN_SHARED] = Text::toT(Util::formatBytes(identity.getBytesShared()));
	columns[COLUMN_DESCRIPTION] = Text::toT(identity.getDescription());
	columns[COLUMN_TAG] = Text::toT(identity.getTag());
	columns[COLUMN_CONNECTION] = Text::toT(identity.getConnection());
	string ip = identity.getIp();
	string country = ip.empty()?Util::emptyString:Util::getIpCountry(ip);
	if (!country.empty())
		ip = country + " (" + ip + ")";
	columns[COLUMN_IP] = Text::toT(ip);
	columns[COLUMN_EMAIL] = Text::toT(identity.getEmail());
	columns[COLUMN_CID] = Text::toT(identity.getUser()->getCID().toBase32());

	if(sortCol != -1) {
		needsSort = needsSort || (old != columns[sortCol]);
	}

	setIdentity(identity);
	return needsSort;
}

int HubWindow::UserInfo::getImage() const {
	int image = identity.isOp() ? IMAGE_OP : IMAGE_USER;

	if(identity.getUser()->isSet(User::DCPLUSPLUS))
		image+=2;
	if(SETTING(INCOMING_CONNECTIONS) == SettingsManager::INCOMING_FIREWALL_PASSIVE && !identity.isTcpActive()) {
		// Users we can't connect to...
		image+=4;
	}
	return image;
}

HubWindow::UserTask::UserTask(const OnlineUser& ou) : user(ou.getUser()), identity(ou.getIdentity()) { 

}

HubWindow::PMTask::PMTask(const OnlineUser& from_, const OnlineUser& to_, const OnlineUser& replyTo_, const string& m) : 
	StringTask(m), from(from_.getUser()), to(to_.getUser()), replyTo(replyTo_.getUser()), 
	hub(replyTo_.getIdentity().isHub()), bot(replyTo_.getIdentity().isBot()) 
{

}

int HubWindow::UserInfo::compareItems(const HubWindow::UserInfo* a, const HubWindow::UserInfo* b, int col) {
	if(col == COLUMN_NICK) {
		bool a_isOp = a->getIdentity().isOp(),
			b_isOp = b->getIdentity().isOp();
		if(a_isOp && !b_isOp)
			return -1;
		if(!a_isOp && b_isOp)
			return 1;
		if(BOOLSETTING(SORT_FAVUSERS_FIRST)) {
			bool a_isFav = FavoriteManager::getInstance()->isFavoriteUser(a->getIdentity().getUser()),
				b_isFav = FavoriteManager::getInstance()->isFavoriteUser(b->getIdentity().getUser());
			if(a_isFav && !b_isFav)
				return -1;
			if(!a_isFav && b_isFav)
				return 1;
		}
	}
	if(col == COLUMN_SHARED) {
		return compare(a->identity.getBytesShared(), b->identity.getBytesShared());;
	}
	return wcscmp(a->columns[col].c_str(), b->columns[col].c_str());
}

void HubWindow::insertUser(UserInfo *ui)
{/*
	//QString newUser = QString::fromStdString(ui->getNick());
	QTreeWidgetItem *userItem;
	
	//userItem = new QTreeWidgetItem(userlist, QStringList(newUser));
	userItem = new QTreeWidgetItem(userlist, QStringList(StilUtils::TstrtoQ(ui->getText(COLUMN_NICK))));
	
	for (int i=1;i<8;i++) 
	{
		userItem -> setText(i, StilUtils::TstrtoQ(ui->getText(i)));
	}
	
	userItem -> setIcon(0, QIcon(userIconsMap.value(ui->getImage())));*/
	QStringList userPropertiesList;
	
	for (int i=0;i<8;i++)
		userPropertiesList << StilUtils::TstrtoQ(ui->getText(i));
		
	if(ui->getIdentity().isOp())
		userPropertiesList << "Op";
	else
		userPropertiesList << "NotOp";
		
	userPropertiesList << QString::number(ui->getIdentity().getBytesShared());
	
	QIcon *icon = new QIcon(userIconsMap.value(ui->getImage()));
	UList->addUser(userPropertiesList, icon);		
}

void HubWindow::eraseUser(UserInfo *ui)
{	/*
	for (int i=0; i<userlist->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *topItem = userlist->topLevelItem(i);
		
		if (topItem -> text(0) == StilUtils::TstrtoQ(ui->getText(0))) 
		{
			userlist -> takeTopLevelItem(i);
			break;
		}
	}*/
	UList->removeUser(StilUtils::TstrtoQ(ui->getText(0)));
}

int HubWindow::findUser(UserInfo *ui)
{/*
	for (int i=0; i<userlist->topLevelItemCount(); i++)
	{
		QTreeWidgetItem *topItem = userlist->topLevelItem(i);
		
		if (topItem -> text(0) == StilUtils::TstrtoQ(ui->getText(0))) 
		{
			//maybe count started wih null?...
			return i;
		}
	}
	*/
	return UList->findUser(StilUtils::TstrtoQ(ui->getText(0)));
}

void HubWindow::clearUser()
{	
	//userlist -> clear();
	UList->clearList();
}

void HubWindow::updateSingleUser(int pos, UserInfo *ui)
{/*
	QTreeWidgetItem *item = userlist->topLevelItem(pos);

	item -> setIcon(0, QIcon(userIconsMap.value(ui->getImage())));
	
	for (int i=0;i<8;i++) 
	{
		item -> setText(i, StilUtils::TstrtoQ(ui->getText(i)));
	}*/
	QStringList userPropertiesList;
	
	for (int i=0;i<8;i++)
		userPropertiesList << StilUtils::TstrtoQ(ui->getText(i));
		
	if(ui->getIdentity().isOp())
		userPropertiesList << "Op";
	else
		userPropertiesList << "NotOp";
	
	userPropertiesList << QString::number(ui->getIdentity().getBytesShared());
	
	QIcon *icon = new QIcon(userIconsMap.value(ui->getImage()));
	UList->updateUser(pos, userPropertiesList, icon);
}

void HubWindow::setStatus(int s, const tstring& text)
{
	switch(s)
	{
		case STATUS_STATUS:
			chatStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_USERS:
			usersStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		case STATUS_SHARED:
			sharedStatusLabel -> setText(StilUtils::TstrtoQ(text));
			break;
		default:
			dcdebug("Set status for %d with text: %s\n", s, Text::fromT(text).c_str());
	}
}
// HubWindow
