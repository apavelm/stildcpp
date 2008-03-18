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

#include "pm_win.h"
#include "mainwindowimpl.h"

PMWindow::PMWindow(QWidget *parent, const UserPtr& _replyTo) : MdiChild(parent), replyTo(_replyTo)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_PRIVATE_CHAT;
	idText  = StilUtils::TstrtoQ(StilUtils::getNicks(replyTo));
	connect(this, SIGNAL(sigSpeak()), this, SLOT(slotSpeak()), Qt::QueuedConnection);
	
	editor->clear();
	setupeditor();
	connect(lineEdit, SIGNAL(SendText(const QString &)), this, SLOT(send_pm_msg(const QString &)));
	//connect(SendBtn, SIGNAL(pressed()), this, SLOT(send_pm_msg()));
	
	updateTitle();
	readLog();
	
	ClientManager::getInstance()->addListener(this);
	speak();
}

PMWindow::~PMWindow()
{
	ClientManager::getInstance()->removeListener(this);
}

bool PMWindow::isOnline()
{
	return replyTo->isOnline();
}

void PMWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void PMWindow::readLog()
{
	StringMap params;

	params["hubNI"] = Util::toString(ClientManager::getInstance()->getHubNames(replyTo->getCID()));
	params["hubURL"] = Util::toString(ClientManager::getInstance()->getHubs(replyTo->getCID()));
	params["userCID"] = replyTo->getCID().toBase32();
	params["userNI"] = ClientManager::getInstance()->getNicks(replyTo->getCID())[0];
	params["myCID"] = ClientManager::getInstance()->getMe()->getCID().toBase32();

	string path = Util::validateFileName(SETTING(LOG_DIRECTORY) + Util::formatParams(SETTING(LOG_FILE_PRIVATE_CHAT), params, true));

	try {
		if (SETTING(SHOW_LAST_LINES_LOG) > 0) {
			File f(path, File::READ, File::OPEN);

			int64_t size = f.getSize();

			if(size > 32*1024) {
				f.setPos(size - 32*1024);
			}

			StringList lines = StringTokenizer<string>(f.read(32*1024), "\r\n").getTokens();

			int linesCount = lines.size();

			int i = linesCount > (SETTING(SHOW_LAST_LINES_LOG) + 1) ? linesCount - (SETTING(SHOW_LAST_LINES_LOG) + 1) : 0;

			for(; i < (linesCount - 1); ++i){
				addStatus(_T("- ") + Text::toT(lines[i]));
			}

			f.close();
		}
	} catch(const FileException&){
	}
}

void PMWindow::send_pm_msg(const QString & _s)
{
	tstring s = StilUtils::QtoTstr(_s);
//	if(s.empty()) 
//	{
//		::MessageBeep(MB_ICONEXCLAMATION);
//	}

	bool resetText = true;
	bool send = false;
	// Process special commands
	if(s[0] == '/')
	{
		tstring cmd = s;
		tstring param;
		tstring message;
		tstring status;
		bool thirdPerson = false;
		if(StilUtils::checkCommand(cmd, param, message, status, thirdPerson))
		{
			if(!message.empty())
			{
				sendMessage(message, thirdPerson);
			}
			
			if(!status.empty())
			{
				addStatus(status);
			}
		} 
		else
			if(Util::stricmp(cmd.c_str(), _T("clear")) == 0) 
			{
				editor->clear();
			} 
			else
				if(Util::stricmp(cmd.c_str(), _T("grant")) == 0)
				{
					UploadManager::getInstance()->reserveSlot(replyTo);
					addStatus(T_("Slot granted"));
				}
				else
					if(Util::stricmp(cmd.c_str(), _T("close")) == 0)
					{
						//delete this;
					}
					else
						if((Util::stricmp(cmd.c_str(), _T("favorite")) == 0) || (Util::stricmp(cmd.c_str(), _T("fav")) == 0))
						{
							FavoriteManager::getInstance()->addFavoriteUser(replyTo);
							addStatus(T_("Favorite user added"));
						} 
						else
							if(Util::stricmp(cmd.c_str(), _T("getlist")) == 0)
							{
								// TODO handleGetList();
							}
							else
								if(Util::stricmp(cmd.c_str(), _T("log")) == 0)
								{
									StringMap params;
									params["hubNI"] = Util::toString(ClientManager::getInstance()->getHubNames(replyTo->getCID()));
									params["hubURL"] = Util::toString(ClientManager::getInstance()->getHubs(replyTo->getCID()));
									params["userCID"] = replyTo->getCID().toBase32();
									params["userNI"] = ClientManager::getInstance()->getNicks(replyTo->getCID())[0];
									params["myCID"] = ClientManager::getInstance()->getMe()->getCID().toBase32();
									MainWindowImpl::getInstance()->openTextWindow(Text::toT(Util::validateFileName(SETTING(LOG_DIRECTORY) + Util::formatParams(SETTING(LOG_FILE_PRIVATE_CHAT), params, true))));
								} 
								else
									if(Util::stricmp(cmd.c_str(), _T("help")) == 0)
									{
										addStatus(_T("*** ") + StilUtils::commands + _T(", /getlist, /clear, /grant, /close, /favorite, /log <system, downloads, uploads>"));
									}
									else
										{
											send = true;
										}
	} 
	else 
		{
			send = true;
		}
	
	if(send) {
		if(replyTo->isOnline()) {
			sendMessage(s);
		} else {
			addStatus(T_("User went offline"));
			resetText = false;
		}
	}
	if(resetText) {
		lineEdit->clear();
	}
}

void PMWindow::addChat(const tstring& aLine)
{
	tstring line;
	if(BOOLSETTING(TIME_STAMPS))
	{
		line = Text::toT("[" + Util::getShortTimeString() + "] ");
	} else {
		line = _T("");
	}
	line += aLine;

	//bool scroll = chat->scrollIsAtEnd();
/*
	size_t limit = chat->getTextLimit();
	if(chat->length() + line.size() > limit) {
		HoldRedraw hold2(chat, scroll);
		chat->setSelection(0, chat->lineIndex(chat->lineFromChar(limit / 10)));
		chat->replaceSelection(_T(""));
	}
	*/
	if(BOOLSETTING(LOG_PRIVATE_CHAT)) {
		StringMap params;
		params["message"] = Text::fromT(aLine);
		params["hubNI"] = Util::toString(ClientManager::getInstance()->getHubNames(replyTo->getCID()));
		params["hubURL"] = Util::toString(ClientManager::getInstance()->getHubs(replyTo->getCID()));
		params["userCID"] = replyTo->getCID().toBase32();
		params["userNI"] = ClientManager::getInstance()->getNicks(replyTo->getCID())[0];
		params["myCID"] = ClientManager::getInstance()->getMe()->getCID().toBase32();
		LOG(LogManager::PM, params);
	}
	//chat->addText(line);
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

	//setDirty(SettingsManager::BOLD_PM);
}

void PMWindow::addStatus(const tstring& aLine, bool inChat /* = true */)
{
	tstring line = Text::toT("[" + Util::getShortTimeString() + "] ") + aLine;

	//setStatus(STATUS_STATUS, line);

	if(BOOLSETTING(STATUS_IN_CHAT) && inChat)
	{
		addChat(_T("*** ") + aLine);
	}
}

void PMWindow::updateTitle()
{
	pair<tstring, bool> hubs = StilUtils::getHubNames(replyTo);
#ifdef PORT_ME
	if(hubs.second) {
		setTabColor(RGB(0, 255, 255));
	} else {
		setTabColor(RGB(255, 0, 0));
	}
#endif 
	setTabText( StilUtils::TstrtoQ(StilUtils::getNicks(replyTo) + _T(" - ") + hubs.first) );
}

void PMWindow::slotSpeak()
{
	updateTitle();
}

void PMWindow::on(ClientManagerListener::UserUpdated, const OnlineUser& aUser) throw()
{
	if(aUser.getUser() == replyTo) speak();
}
void PMWindow::on(ClientManagerListener::UserConnected, const UserPtr& aUser) throw()
{
	if(aUser == replyTo) speak();
}
void PMWindow::on(ClientManagerListener::UserDisconnected, const UserPtr& aUser) throw()
{
	if(aUser == replyTo) speak();
}

// PRIVATE MESSAGE WINDOW
