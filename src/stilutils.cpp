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

#include "stilutils.h"

string StilUtils::getNicks(const CID& cid)
{
	return Util::toString(ClientManager::getInstance()->getNicks(cid));
}

string StilUtils::getNicks(const UserPtr& user)
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

// of stilutils
