/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
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

#include "stilutils.h"
#include "mainwindowimpl.h"

using namespace std;
using namespace dcpp;

int StilUtils::_type_time_t = qRegisterMetaType<time_t>("time_t");
int StilUtils::_type_tstring = qRegisterMetaType<tstring>("tstring");
int StilUtils::_type_UserPtr = qRegisterMetaType<UserPtr>("UserPtr");
int StilUtils::_type_FinishedItemPtr = qRegisterMetaType<FinishedItemPtr>("FinishedItemPtr");
int StilUtils::_type_Speaker = qRegisterMetaType<StilUtils::Speaker>("StilUtils::Speaker");

tstring StilUtils::commands = _T("/refresh, /me <msg>, /slots #, /search <string>, /dc++, /away <msg>, /back, /g <searchstring>, /imdb <imdbquery>, /u <url>, /rebuild");

const tstring msgs[] = { _T("\r\n-- I'm a happy dc++ user. You could be happy too.\r\n"),
_T("\r\n-- Neo-...what? Nope...never heard of it...\r\n"),
_T("\r\n-- Evolution of species: Ape --> Man\r\n-- Evolution of science: \"The Earth is Flat\" --> \"The Earth is Round\"\r\n-- Evolution of sharing: NMDC --> DC++\r\n"),
_T("\r\n-- I share, therefore I am.\r\n"),
_T("\r\n-- I came, I searched, I found...\r\n"),
_T("\r\n-- I came, I shared, I sent...\r\n"),
_T("\r\n-- I can set away mode, can't you?\r\n"),
_T("\r\n-- I don't have to see any ads, do you?\r\n"),
_T("\r\n-- I don't have to see those annoying kick messages, do you?\r\n"),
_T("\r\n-- I can resume my files to a different filename, can you?\r\n"),
_T("\r\n-- I can share huge amounts of files, can you?\r\n"),
_T("\r\n-- My client doesn't spam the chat with useless debug messages, does yours?\r\n"),
_T("\r\n-- I can add multiple users to the same download and have the client connect to another automatically when one goes offline, can you?\r\n"),
_T("\r\n-- These addies are pretty annoying, aren't they? Get revenge by sending them yourself!\r\n"),
_T("\r\n-- My client supports TTH hashes, does yours?\r\n"),
_T("\r\n-- My client supports XML file lists, does yours?\r\n")
};

#define MSGS 16


tstring StilUtils::getNicks(const CID& cid)
{
	return Text::toT(Util::toString(ClientManager::getInstance()->getNicks(cid)));
}

tstring StilUtils::getNicks(const UserPtr& user)
{
	return getNicks(user->getCID());
}

tstring StilUtils::QtoTstr(const QString& qstr)
{
#ifdef UNICODE
	return (tstring(qstr.toStdWString()));
#else
	return (tstring(qstr.toStdString()));
#endif	
}

QString StilUtils::TstrtoQ(const tstring& tstr)
{
#ifdef UNICODE
	return QString::fromStdWString(tstr);
#else
	return QString::fromStdString(tstr);
#endif
}

QString StilUtils::fmtBytes(int64_t aBytes)
{
	float res = aBytes;
	
	int s = 0;
	while (res > 1024)
	{
		res /= 1024;
		s++;
	}
	
	QString suffix = " B";
	if (s == 1) suffix = " KiB";
	if (s == 2) suffix = " MiB";
	if (s == 3) suffix = " GiB";
	if (s == 4) suffix = " TiB";
	if (s == 5) suffix = " PiB";
	if (s == 6) suffix = " EiB";
	if (s == 7) suffix = " ZiB";
	if (s == 8) suffix = " YiB";
	
	res = floor(res*100)/100;
	
	QString t;
	t.setNum(res,'G',4);
	t += suffix;
	return t;
}

pair<tstring, bool> StilUtils::getHubNames(const CID& cid)
{
	StringList hubs = ClientManager::getInstance()->getHubNames(cid);
	if(hubs.empty()) 
	{
		return make_pair(Text::toT("Offline"), false);
	} else {
		return make_pair(Text::toT(Util::toString(hubs)), true);
	}
}

pair<tstring, bool> StilUtils::getHubNames(const UserPtr& u)
{ 
	return getHubNames(u->getCID()); 
}

bool StilUtils::checkCommand(tstring& cmd, tstring& param, tstring& message, tstring& status, bool& thirdPerson)
{
	string::size_type i = cmd.find(' ');
	if(i != string::npos) {
		param = cmd.substr(i+1);
		cmd = cmd.substr(1, i - 1);
	} else {
		cmd = cmd.substr(1);
	}

	if(Util::stricmp(cmd.c_str(), _T("log")) == 0) {
		if(Util::stricmp(param.c_str(), _T("system")) == 0) {
			MainWindowImpl::getInstance()->openTextWindow(Text::toT(Util::validateFileName(SETTING(LOG_DIRECTORY) + "system.log")));
		} else if(Util::stricmp(param.c_str(), _T("downloads")) == 0) {
			MainWindowImpl::getInstance()->openTextWindow(Text::toT(Util::validateFileName(SETTING(LOG_DIRECTORY) + Util::formatTime(SETTING(LOG_FILE_DOWNLOAD), time(NULL)))));
		} else if(Util::stricmp(param.c_str(), _T("uploads")) == 0) {
			MainWindowImpl::getInstance()->openTextWindow(Text::toT(Util::validateFileName(SETTING(LOG_DIRECTORY) + Util::formatTime(SETTING(LOG_FILE_UPLOAD), time(NULL)))));
		} else {
			return false;
		}
	} else if(Util::stricmp(cmd.c_str(), _T("me")) == 0) {
		message = param;
		thirdPerson = true;
	} else if(Util::stricmp(cmd.c_str(), _T("refresh"))==0) {
		try {
			ShareManager::getInstance()->setDirty();
			ShareManager::getInstance()->refresh(true);
		} catch(const ShareException& e) {
			status = Text::toT(e.getError());
		}
	} else if(Util::stricmp(cmd.c_str(), _T("slots"))==0) {
		int j = Util::toInt(Text::fromT(param));
		if(j > 0) {
			SettingsManager::getInstance()->set(SettingsManager::SLOTS, j);
			status = T_("Slots set");
			ClientManager::getInstance()->infoUpdated();
		} else {
			status = T_("Invalid number of slots");
		}
	} else if(Util::stricmp(cmd.c_str(), _T("search")) == 0) {
		if(!param.empty()) {
			MainWindowImpl::getInstance()->OpenSearch(param);
		} else {
			status = T_("Specify a search string");
		}
	} else if(Util::stricmp(cmd.c_str(), _T("dc++")) == 0) {
		message = msgs[GET_TICK() % MSGS];
	} else if(Util::stricmp(cmd.c_str(), _T("away")) == 0) {
		if(Util::getAway() && param.empty()) {
			Util::setAway(false);
			Util::setManualAway(false);
			status = T_("Away mode off");
		} else {
			Util::setAway(true);
			Util::setManualAway(true);
			Util::setAwayMessage(Text::fromT(param));
			status = str(TF_("Away mode on: %1%") % Text::toT(Util::getAwayMessage()));
		}
	} else if(Util::stricmp(cmd.c_str(), _T("back")) == 0) {
		Util::setAway(false);
		Util::setManualAway(false);
		status = T_("Away mode off");
	} else if(Util::stricmp(cmd.c_str(), _T("g")) == 0) {
		if(param.empty()) {
			status = T_("Specify a search string");
		} else {
			StilUtils::openLink(_T("http://www.google.com/search?q=") + Text::toT(Util::encodeURI(Text::fromT(param))));
		}
	} else if(Util::stricmp(cmd.c_str(), _T("imdb")) == 0) {
		if(param.empty()) {
			status = T_("Specify a search string");
		} else {
			StilUtils::openLink(_T("http://www.imdb.com/find?q=") + Text::toT(Util::encodeURI(Text::fromT(param))));
		}
	} else if(Util::stricmp(cmd.c_str(), _T("u")) == 0) {
		if (param.empty()) {
			status = T_("Specify a URL");
		} else {
			StilUtils::openLink(Text::toT(Util::encodeURI(Text::fromT(param))));
		}
	} else if(Util::stricmp(cmd.c_str(), _T("rebuild")) == 0) {
		HashManager::getInstance()->rebuild();
	} else {
		return false;
	}

	return true;
}

bool StilUtils::checkCommand(QString& cmd, QString& param, QString& message, QString& status, bool& thirdPerson)
{
	tstring a = QtoTstr(cmd);
	tstring b = QtoTstr(param);
	tstring c = QtoTstr(message);
	tstring d = QtoTstr(status);
	return checkCommand( a, b, c, d, thirdPerson );
}

bool StilUtils::getUCParams(QWidget *parent, const UserCommand& uc, StringMap& sm) throw()
{
	string::size_type i = 0;
	StringMap done;

	while( (i = uc.getCommand().find("%[line:", i)) != string::npos)
		{
		i += 7;
		string::size_type j = uc.getCommand().find(']', i);
		if(j == string::npos)
			break;

		string name = uc.getCommand().substr(i, j-i);
		if(done.find(name) == done.end()) 
			{
				bool ok;
				QString text = QInputDialog::getText(parent, TstrtoQ(Text::toT(uc.getName())), TstrtoQ(Text::toT(name)), QLineEdit::Normal,  TstrtoQ(Text::toT(sm["line:" + name])), &ok);
				if (ok && !text.isEmpty()) done[name] = sm["line:" + name] = text.toStdString();
					else return false;
			}
		i = j + 1;
	}
	return true;
}

string StilUtils::linuxSeparator(const string &s)
{
	string str = s;
	for (string::iterator it = str.begin(); it != str.end(); ++it)
		if ((*it) == '\\')
			(*it) = '/';
	return str;
}

string StilUtils::windowsSeparator(const string &s)
{
	string str = s;
	for (string::iterator it = str.begin(); it != str.end(); ++it)
		if ((*it) == '/')
			(*it) = '\\';
	return str;
}

void StilUtils::copy2Clipboard(const QString & txt)
{
	QApplication::clipboard()->setText(txt, QClipboard::Clipboard);
	if(QApplication::clipboard()->supportsSelection())
		QApplication::clipboard()->setText(txt, QClipboard::Selection);
}

void StilUtils::copy2Clipboard(const tstring & txt)
{
	copy2Clipboard(TstrtoQ(txt));
}

void StilUtils::openLink(const tstring& url)
{
	openLink(TstrtoQ(url));
}

void StilUtils::openLink(const QString& url)
{
	
}

// of stilutils
