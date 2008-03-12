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

#ifndef CONFIG_H
#define CONFIG_H

#include "defs.h"
#include "stilutils.h"
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
		i_HIDEONCLOSE, i_SHOWSPLASH, i_USETRAY, i_STARTHIDDEN, i_TABPOSIOTION
		,i_NOTEPADFONTSIZE, i_UPDATEIP, i_SHOWSMILES, i_HUBLEFTSIDE, i_TOOLBARICONSIZE, i_TOOLBARICONSTYLE
		,i_SORTUSERLISTOPSFIRST, i_SORTUSERLISTCLASSIC, i_ACT_ON_DBLCLICK_TRANSFERVIEW, i_SYSLOGFONTSIZE
		,i_TEXTWINDOWFONTSIZE
		,i_LAST };// DO NOT CHANGE THIS LINE
		
		enum EnumSettings2 
	{ 
		s_ICONSETPATH, s_DEFCHARSET, s_TRANSVIEW_COLUMN_VISIBILITY, s_DLWINDOW_COLUMN_VISIBILITY,
		s_SEARCHWINDOW_COLUMN_VISIBILITY, s_SEARCHSPYWINDOW_COLUMN_VISIBILITY, s_FINISHED_DL_COLUMN_VISIBILITY,
		s_FINISHED_UL_COLUMN_VISIBILITY, s_ADLSEARCH_COLUMN_VISIBILITY, s_PUBHUBS_COLUMN_VISIBILITY
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
	const int get(AppSettings::EnumSettings numname) { if ((numname<i_LAST)&&(numname>-1)) return intSettings[numname];else return -32000; }
	const QString gets(AppSettings::EnumSettings2 numname) { if ((numname<s_LAST)&&(numname>-1)) return strSettings[numname];else return ""; }
	const QByteArray getsb(AppSettings::EnumSettings2 numname) { if ((numname<s_LAST)&&(numname>-1)) return strSettings[numname];else return ""; }
	const int getDef(AppSettings::EnumSettings numname) { if ((numname<i_LAST)&&(numname>-1)) return intDefaults[numname];else return -32000; }
	const QString getDefs(AppSettings::EnumSettings2 numname) { if ((numname<s_LAST)&&(numname>-1)) return strDefaults[numname];else return ""; }
	void set(AppSettings::EnumSettings numname, int value) { intSettings[numname]=value; }
	void sets(AppSettings::EnumSettings2 numname, QString c) { strSettings[numname]=c.toUtf8(); }
	void setsb(AppSettings::EnumSettings2 numname, QByteArray c) { strSettings[numname]=c; }
private:
	friend class dcpp::Singleton<AppSettingsMgr>;
	AppSettingsMgr();
	virtual ~AppSettingsMgr() throw() {}
	
	QDomDocument xml;
	
	static const char * intTags[i_LAST];
	static const char * strTags[s_LAST];

	QList<int> intSettings, intDefaults;
	QList<QByteArray> strSettings, strDefaults;
};

};

// Shorthand accessor macros
#define APPSETTING(k) (AppSettings::AppSettingsMgr::getInstance()->get(AppSettings::k))
#define APPSTRING(k) (AppSettings::AppSettingsMgr::getInstance()->gets(AppSettings::k))
#define APPBYTEARRAY(k) (AppSettings::AppSettingsMgr::getInstance()->getsb(AppSettings::k))
#define SETAPPSETTING(k,v) (AppSettings::AppSettingsMgr::getInstance()->set(AppSettings::k,v))
#define SETAPPSTRING(k,v) (AppSettings::AppSettingsMgr::getInstance()->sets(AppSettings::k,v))
#define SETAPPBYTEARRAY(k,v) (AppSettings::AppSettingsMgr::getInstance()->setsb(AppSettings::k,v))
#define APPSETTINGDEF(k) (AppSettings::AppSettingsMgr::getInstance()->getDef(AppSettings::k))
#define APPSTRINGDEF(k) (AppSettings::AppSettingsMgr::getInstance()->getDefs(AppSettings::k))

#endif
