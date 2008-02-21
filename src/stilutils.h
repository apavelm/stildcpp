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

#ifndef STILUTILS_H
#define STILUTILS_H

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/CID.h"
#include "client/User.h"
#include "client/ClientManager.h"
#include "client/UserCommand.h"
#include "client/ResourceManager.h"

#include <QtCore>
#include <QInputDialog>
#include <QString>
#include <QObject>

using namespace dcpp;

 class StilUtils  
{
public:
	static int _type_time_t;
	static int _type_tstring;
	static int _type_UserPtr;
	static int _type_FinishedItemPtr;
	
	/*  0 - MdiChild (NONE)
		1 - Hub
		2 - Private Chat
		3 - Search
		4 - Favorite Hub List
		5 - FileList
		6 - DL Queue
		7 - Favorite Users
		8 - Ignored Users
		9 - Public Hubs
		10 - Finished DL
		11 - Finished UL
		12 - ADL Search
		13 - Search Spy
		14 - NotePad
		15 - DownLoads Frame
		16 - System Log
		17 - TextWindow (view file as text)
	*/
	
	enum tabWinTypes{
		WIN_TYPE_FIRST
		,WIN_TYPE_NONE = WIN_TYPE_FIRST
		,WIN_TYPE_HUB
		,WIN_TYPE_PRIVATE_CHAT
		,WIN_TYPE_SEARCH
		,WIN_TYPE_FAVORITE_HUB_LIST
		,WIN_TYPE_FILELIST
		,WIN_TYPE_DL_QUEUE
		,WIN_TYPE_FAVORITE_USERS
		,WIN_TYPE_IGNORED_USERS
		,WIN_TYPE_PUBLIC_HUBS
		,WIN_TYPE_FINISHED_DL
		,WIN_TYPE_FINISHED_UL
		,WIN_TYPE_ADL_SEARCH
		,WIN_TYPE_SEARCH_SPY
		,WIN_TYPE_NOTEPAD
		,WIN_TYPE_DOWNLOADS
		,WIN_TYPE_SYSTEM_LOG
		,WIN_TYPE_TEXT_WINDOW
		,WIN_TYPE_LAST
	};
	
	//StilUtils();
	static tstring getNicks(const CID& cid);
	static tstring getNicks(const UserPtr& user);
	
	static tstring QtoTstr(const QString& qstr);
	static QString TstrtoQ(const tstring& tstr);
	
	static QString fmtBytes(int64_t aBytes);
	
	static pair<tstring, bool> getHubNames(const CID& cid);
	static pair<tstring, bool> getHubNames(const UserPtr& u);
	
	static bool checkCommand(QString& cmd, QString& param, QString& message, QString& status);
	static bool getUCParams(QWidget * parent, const UserCommand& uc, StringMap& sm) throw();

	template<typename T>
	static TStringList getStrings(const T& t)
	{
		const size_t n = sizeof(t) / sizeof(t[0]);
		TStringList ret(n);
		for(size_t i = 0; i < n; ++i)
		{
			ret[i] = T_(t[i]);
		}
		return ret;
	}

	static std::string linuxSeparator(const std::string &s);
	static std::string windowsSeparator(const std::string &s);

};

#endif // of StilUtils

