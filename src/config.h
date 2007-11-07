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
#include "xml/tinyxml.h"
#include "stilutils.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Speaker.h"
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
		

class AppSettingsMgrListener {
public:
	virtual ~AppSettingsMgrListener() { }
	template<int I>	struct X { enum { TYPE = I }; };

	typedef X<0> load;
	typedef X<1> save;
	typedef X<2> get;
	typedef X<3> getDef;
	typedef X<4> set;

	virtual void on(load, dcpp::tstring) throw() { }
	virtual void on(save, dcpp::tstring) throw() { }
	virtual void on(get, int) throw() { }
	virtual void on(getDef, int) throw() { }
	virtual void on(set, int, int) throw() { }
};

class AppSettingsMgr: public dcpp::Singleton<AppSettingsMgr>, public dcpp::Speaker<AppSettingsMgrListener>
{
public:

	int load();
	void save();
	int load(dcpp::tstring);
	void save(dcpp::tstring);
	
	void setDefaults();
	int get(int numname) { if ((numname<i_LAST)&&(numname>-1)) return intSettings[numname];else return -32000; }
	int getDef(int numname) { if ((numname<i_LAST)&&(numname>-1)) return settingsDefaults[numname];else return -32000; }
	void set(int numname, int value) { intSettings[numname]=value; }
private:
	friend class dcpp::Singleton<AppSettingsMgr>;
	AppSettingsMgr();
	virtual ~AppSettingsMgr() throw() {}
	
	TiXmlDocument xml;
	
	static const char * settingTags[i_LAST+1];
	static const int settingsDefaults[i_LAST+1];
	int intSettings[i_LAST+1];
};

};

// Shorthand accessor macros
#define APPSETTING(k) (AppSettings::AppSettingsMgr::getInstance()->get(AppSettings::k))
#define SETAPPSETTING(k,v) (AppSettings::AppSettingsMgr::getInstance()->set(AppSettings::k,v))

#endif
