/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
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

#ifndef CONFIG_H
#define CONFIG_H

#include "defs.h"
#include <QDir>
#include <QtXml>

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Util.h"
#include "client/Singleton.h"
//

namespace AppSettings
{
	
	enum EnumSettings 
	{ 
		i_HIDEONCLOSE, i_SHOWSPLASH, i_USETRAY, i_STARTHIDDEN, i_PROMPTONCLOSE, i_TABPOSIOTION
		,i_NOTEPADFONTSIZE, i_UPDATEIP
		,i_LAST };// DO NOT CHANGE THIS LINE
		
		enum EnumSettings2 
	{ 
		s_TEST
		,s_LAST };// DO NOT CHANGE THIS LINE
		
class AppSettingsMgr: public dcpp::Singleton<AppSettingsMgr>
{
public:

	const int load();
	void save();
	const int load(const QString &);
	void save(const QString &);
	
	void setDefaults();
	void writeDefs();
	const int get(int numname) { if ((numname<i_LAST)&&(numname>-1)) return intSettings[numname];else return -32000; }
	const QString gets(int numname) { if ((numname<s_LAST)&&(numname>-1)) return strSettings[numname];else return ""; }
	const int getDef(int numname) { if ((numname<i_LAST)&&(numname>-1)) return intDefaults[numname];else return -32000; }
	const QString getDefs(int numname) { if ((numname<s_LAST)&&(numname>-1)) return strDefaults[numname];else return ""; }
	void set(int numname, int value) { intSettings[numname]=value; }
	void sets(int numname, QString &c) { strSettings[numname]=c; }
private:
	friend class dcpp::Singleton<AppSettingsMgr>;
	AppSettingsMgr();
	virtual ~AppSettingsMgr() throw() {}
	
	QDomDocument xml;
	
	static const char * intTags[i_LAST];
	static const char * strTags[s_LAST];

	QList<int> intSettings, intDefaults;
	QStringList strSettings, strDefaults;
};

};

// Shorthand accessor macros
#define APPSETTING(k) (AppSettings::AppSettingsMgr::getInstance()->get(AppSettings::k))
#define APPSTRING(k) (AppSettings::AppSettingsMgr::getInstance()->gets(AppSettings::k))
#define SETAPPSETTING(k,v) (AppSettings::AppSettingsMgr::getInstance()->set(AppSettings::k,v))
#define SETAPPSTRING(k,v) (AppSettings::AppSettingsMgr::getInstance()->sets(AppSettings::k,v))
#define APPSETTINGDEF(k) (AppSettings::AppSettingsMgr::getInstance()->getDef(AppSettings::k))
#define APPSTRINGDEF(k) (AppSettings::AppSettingsMgr::getInstance()->getDefs(AppSettings::k))

#endif
