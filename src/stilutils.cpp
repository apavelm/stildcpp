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

using namespace std;
using namespace dcpp;

int StilUtils::_type_time_t = qRegisterMetaType<time_t>("time_t");
int StilUtils::_type_tstring = qRegisterMetaType<tstring>("tstring");
int StilUtils::_type_UserPtr = qRegisterMetaType<UserPtr>("UserPtr");
int StilUtils::_type_FinishedItemPtr = qRegisterMetaType<FinishedItemPtr>("FinishedItemPtr");


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

bool StilUtils::checkCommand(QString& cmd, QString& param, QString& message, QString& status)
{
	//if (cmd.at(0) == '/')
	//{		
		int space = cmd.indexOf(' ');
		
		if ((space != -1))// && cmd.size() > space + 1)
		{			
			param = cmd.mid(space + 1);
			cmd = cmd.mid(1, space - 1);
		}
		else
		{
			cmd = cmd.mid(1);
		}
		
		//cmd = cmd.toLower();

		if (QString::compare(cmd, "away", Qt::CaseInsensitive) == 0)
		{
			if (Util::getAway() && param.isEmpty())
			{
				Util::setAway(false);
				Util::setManualAway(false);
				status = StilUtils::TstrtoQ(T_("Away mode off"));
			}
			else
			{
				Util::setAway(true);
				Util::setManualAway(true);
				Util::setAwayMessage(Text::fromT(StilUtils::QtoTstr(param)));
				status = StilUtils::TstrtoQ(T_("Away mode on: ") + Text::toT(Util::getAwayMessage()));
			}
		}
		else if (QString::compare(cmd, "back", Qt::CaseInsensitive) == 0)
		{
			Util::setAway(FALSE);
			status = StilUtils::TstrtoQ(T_("Away mode off"));
		}
		else
			return false;
			
		return true;
	
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

// of stilutils
