/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *   Copyright (C) 2007 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)                  *
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
};

#endif // of StilUtils

