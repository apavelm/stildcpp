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

namespace AppSettings
{
	
	enum EnumSettings 
	{ 
		s_HIDEONCLOSE, s_SHOWSPLASH, s_USETRAY, s_STARTHIDDEN, s_PROMPTONCLOSE, s_TABPOSIOTION
		,s_LAST };// DO NOT CHANGE THIS LINE

class AppSettingsMgr
{
public:

	int load();
	void save();
	int load(const char *);
	void save(const char *);
	
	void setDefaults();
	int getValue(int numname) { if ((numname<s_LAST)&&(numname>-1)) return intSettings[numname];else return -32000; }
	int getDefValue(int numname) { if ((numname<s_LAST)&&(numname>-1)) return settingsDefaults[numname];else return -32000; }
	int *getValueArray() { return intSettings; }
	
	AppSettingsMgr();

private:
	
	TiXmlDocument xml;
	
	static const char * settingTags[s_LAST+1];
	static const int settingsDefaults[s_LAST+1];
	int intSettings[s_LAST+1];
};

static AppSettingsMgr *CfgMgr;

};

#endif
