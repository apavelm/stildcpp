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

#include "config.h"

using namespace std;
using namespace dcpp;

namespace AppSettings
{

const char * AppSettingsMgr::settingTags[] = 
{
	"HideOnClose", "ShowSplash", "UseTray", "StartHidden", "PromptOnClose", "TabPosition"
	,"NotePadFontSize", "UpdateIPonStartup"
	,"LAST" // DO NOT DELETE
};

const int AppSettingsMgr::settingsDefaults[] =
{
	1, 1, 1, 0, 1, 0, 10, 0
	, 0 //DO NOT DELETE
};

void AppSettingsMgr::setDefaults()
{
	xml.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
	xml.LinkEndChild( decl );
	
	TiXmlElement * root = new TiXmlElement( "config" );
	xml.LinkEndChild( root );
	
	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue(" Settings for StilDC++ " );
	root->LinkEndChild( comment );
	
	TiXmlElement * glob = new TiXmlElement( "global" );
	root->LinkEndChild( glob );
	
	for (int i=0; i<i_LAST; i++) 
	{
	intSettings[i] = settingsDefaults[i];
	TiXmlElement * looknfeel;
	looknfeel = new TiXmlElement( settingTags[i] );  
	glob->LinkEndChild( looknfeel );
	looknfeel->SetAttribute("value", intSettings[i]);
	looknfeel->SetAttribute("default", settingsDefaults[i]);	
	}
}

AppSettingsMgr::AppSettingsMgr()
{
	for (int i=0;i<i_LAST;i++) intSettings[i]=0;
	setDefaults();
}

int AppSettingsMgr::load(dcpp::tstring aFileName)
{
	xml.Clear();
	if (!xml.LoadFile(dcpp::Text::fromT(aFileName)))  return 1;
	
	TiXmlHandle hDoc(&xml);
	TiXmlHandle hRoot(0);
	TiXmlElement* pElem;
	
	pElem=hDoc.FirstChildElement().Element();	
	if (!pElem) return 2;
	hRoot=TiXmlHandle(pElem);
	
	
	TiXmlElement* pNode = hRoot.FirstChild( "global" ).FirstChild().Element();
	for( int i=0; i<i_LAST; i++)
	{
		if ( TIXML_SUCCESS != pNode->QueryIntAttribute("value", &intSettings[i])) return 2;
		pNode = pNode->NextSiblingElement();
	}

	return 0;
}

void AppSettingsMgr::save(dcpp::tstring aFileName) 
{
	bool b=QDir(".").mkpath(QString::fromStdString(dcpp::Util::getConfigPath()));
	if ((!xml.SaveFile(dcpp::Text::fromT(aFileName)))||(!b)) fprintf(stderr,"Unable to save configuration file!");
}

void AppSettingsMgr::save() 
{
	QString tmp(QString::fromStdString(dcpp::Util::getConfigPath()));
	tmp+="stildcpp.xml";
	save(StilUtils::QtoTstr(tmp));
}

int AppSettingsMgr::load()
{
	QString tmp(QString::fromStdString(dcpp::Util::getConfigPath()));
	tmp+="stildcpp.xml";
	return load(StilUtils::QtoTstr(tmp));
}

} //of namespace

