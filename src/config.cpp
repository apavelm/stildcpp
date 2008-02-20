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

#include "config.h"

using namespace std;
using namespace dcpp;

namespace AppSettings
{

const char * AppSettingsMgr::intTags[] = 
{
	"HideOnClose", "ShowSplash", "UseTray", "StartHidden", "PromptOnClose", "TabPosition"
	,"NotePadFontSize", "UpdateIPonStartup", "ShowSmiles", "SwapUserListHub", "MainToolBarIconSize"
	,"MainToolBarStyle", "OPsGoesFirst", "ClassicSort", "ActionOnTransferDblClick", "SysLogFontSize", "TextWindowFontSize"
};

const char * AppSettingsMgr::strTags[] = 
{
	"IconSetPath", "DefaultHubEncoding", "TransferViewColumnVisibility", "DLWindowColumnVisibility", "SearchWindowColumnVisibility"
};

void AppSettingsMgr::writeDefs()
{
	intDefaults << 1 << 1 << 1 << 0 << 1 << 0 << 10 << 0 << 1 << 0 << 48 << Qt::ToolButtonIconOnly << 1 << 1 << 0 << 10 << 10;
	strDefaults << "/home/irq/stildcpp/images/emotions/default.icp";
	strDefaults << "CP1251";
	strDefaults << "0,0,0,0,0,0,0,0,0,0,0,0";
	strDefaults << "0,0,0,0,0,0,0,0,0,0,0,0";
	strDefaults << "0,0,0,0,0,0,0,0,0,0,0,0";
	intSettings = intDefaults;
	strSettings = strDefaults;
}

AppSettingsMgr::AppSettingsMgr()
{
	setDefaults();
}

void AppSettingsMgr::setDefaults()
{
	intSettings.clear();
	intDefaults.clear();
	strSettings.clear();
	strDefaults.clear();
	writeDefs();
}

const int AppSettingsMgr::load(const QString &aFileName)
{
QFileInfo fi(aFileName);
if (fi.exists() && fi.isReadable())
{
	QFile file(aFileName);
	if (!file.open(QIODevice::ReadOnly)) return 4;
	QByteArray data = file.readAll();
	file.close();
	if (data.isNull()) return 4;

	if (!xml.setContent(data)) return 3;

	QDomElement x_config = xml.documentElement();
	if (x_config.tagName() != "config") return 2;

	// load int vars
	QDomElement x_int = x_config.firstChildElement("int");
		for( int i=0; i<i_LAST; i++)
			intSettings[i] = (x_int.firstChildElement(intTags[i]).attribute("value",QByteArray::number(intDefaults[i]))).toLong();

	// load str vars
	QDomElement x_str = x_config.firstChildElement("str");
	for( int i=0; i<s_LAST; i++)
	{
		strSettings[i] = (x_str.firstChildElement(strTags[i]).text()).toUtf8();
		if (strSettings[i].isEmpty()) strSettings[i]=strDefaults[i];
	}
	
	return 0;
}
	else return 4;
}

void AppSettingsMgr::save(const QString &aFileName) 
{
	QDir(".").mkpath(StilUtils::TstrtoQ(Text::toT(dcpp::Util::getConfigPath())));
	
	// Creating structure of XML file
	xml.clear();
	
	QDomNode decl = xml.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	xml.appendChild(decl);
	
	QDomNode x_config = xml.createElement("config");
	
	QDomElement x_int = xml.createElement("int");
	for( int i=0; i<i_LAST; i++)
	{
		QDomElement x_i = xml.createElement(intTags[i]);
		x_i.setAttribute("value",intSettings[i]);
		x_int.appendChild(x_i);
	}
	x_config.appendChild(x_int);
	
	QDomElement x_str = xml.createElement("str");
	for( int i=0; i<s_LAST; i++)
	{
		QDomElement x_s = xml.createElement(strTags[i]);
		QDomText newTitleText = xml.createTextNode(strSettings[i]);
		x_s.appendChild(newTitleText);
		x_str.appendChild(x_s);
	}
	x_config.appendChild(x_str);
	
	xml.appendChild(x_config);
	
	// Saving XML File
	
	QFile file(aFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) 
	{
		file.write(xml.toByteArray());
		file.close();
	}
}

void AppSettingsMgr::save() 
{
	QString tmp(StilUtils::TstrtoQ(Text::toT(dcpp::Util::getConfigPath())));
	tmp+="stildcpp.xml";
	save(tmp);
}

const int AppSettingsMgr::load()
{
	QString tmp(StilUtils::TstrtoQ(Text::toT(dcpp::Util::getConfigPath())));
	tmp+="stildcpp.xml";
	const int r = load(tmp);
	if (r) save();
	return r;
}

} //of namespace

