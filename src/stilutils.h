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
//#include "client/StringDefs.h"
//#include "mainwindowimpl.h"
#include <QString>
#include <QInputDialog>
#include <QObject>

using namespace dcpp;

class StilUtils  
{
public:
	//StilUtils();
	static std::string getNicks(const CID& cid);
	static std::string getNicks(const UserPtr& user);
	
	static tstring QtoTstr(const QString& qstr);
	static QString TstrtoQ(const tstring& tstr);
	
	static QString fmtBytes(int64_t aBytes);
	
	static pair<tstring, bool> getHubNames(const CID& cid);
	static pair<tstring, bool> getHubNames(const UserPtr& u);
	
	static bool checkCommand(QString& cmd, QString& param, QString& message, QString& status);
	static bool getUCParams(QWidget * parent, const UserCommand& uc, StringMap& sm) throw();
};

#endif // of StilUtils

