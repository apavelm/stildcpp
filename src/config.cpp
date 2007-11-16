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

const char * AppSettingsMgr::intTags[] = 
{
	"HideOnClose", "ShowSplash", "UseTray", "StartHidden", "PromptOnClose", "TabPosition"
	,"NotePadFontSize", "UpdateIPonStartup"
};

const char * AppSettingsMgr::strTags[] = 
{
	"TestValue"
};

void AppSettingsMgr::writeDefs()
{
	intDefaults << 1 << 1 << 1 << 0 << 1 << 0 << 10 << 0;
	strDefaults << "bra-bra-bra";
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
	
	intSettings = intDefaults;
	strSettings = strDefaults;
	
	xml.Clear();
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
	xml.LinkEndChild( decl );
	
	TiXmlElement * root = new TiXmlElement( "config" );
	xml.LinkEndChild( root );
	
	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue(" Settings for StilDC++ " );
	root->LinkEndChild( comment );
	
	TiXmlElement * _int = new TiXmlElement( "int" );
	root->LinkEndChild( _int );
	
	for( int i=0; i<i_LAST; i++)
	{
	TiXmlElement * looknfeel;
	looknfeel = new TiXmlElement( intTags[i] );  
	_int->LinkEndChild( looknfeel );
	looknfeel->SetAttribute("value", intSettings[i]);
	}
	
	TiXmlElement * _str = new TiXmlElement( "str" );
	root->LinkEndChild( _str );
	
	for( int i=0; i<s_LAST; i++)
	{

	TiXmlElement * tm;
	tm = new TiXmlElement( strTags[i] );
	_str->LinkEndChild( tm );
	
	std::string temp = strSettings[i].toStdString();
		if ( tm->NoChildren() ) tm->LinkEndChild( new TiXmlText( temp ) ) ;
		else
		{
			if ( 0 == _str->GetText() )
				tm->InsertBeforeChild( tm->FirstChild(), TiXmlText( temp ) );
			else tm->FirstChild()->SetValue( temp ); // There already is text, so change it
		}
	
	}
	
}

const int AppSettingsMgr::load(const QString &aFileName)
{
	bool sv = true;
	bool sv2 = true;
	bool fv[i_LAST];
	bool fv2[s_LAST];
	for (int i=0; i<i_LAST;i++) fv[i] = true;
	for (int i=0; i<s_LAST;i++) fv2[i] = true;
	
	xml.Clear();
	if (!xml.LoadFile(aFileName.toStdString()))  
		{
			setDefaults();
			save();
			return 1;
		}
	
	TiXmlHandle hDoc(&xml);
	TiXmlHandle hRoot(0);
	TiXmlElement* pElem;
	TiXmlNode * child;
	TiXmlElement* pNode;
	TiXmlNode * parent;
	TiXmlElement* pm;
	
	pElem=hDoc.FirstChildElement().Element();
	if (!pElem) 
		{
			setDefaults();
			save();
			return 2;
		}
	

	hRoot=TiXmlHandle(pElem);
	parent = hRoot.Node();
	child = 0;
	
	if (!strcmp(parent->Value(),"config"))
	while( child = parent->IterateChildren( child ) )
	{
		if (!strcmp(child->Value(),"int"))
		{
			sv = false;
			pNode = child->FirstChildElement();
			while (pNode)
			{
				pm = pNode;
				while (pm)
				{
					const char * tag = pm->Value();

				for( int i=0; i<i_LAST; i++)
				if ( !strcmp(tag, intTags[i]) ) 
				{
					fv[i]=false;
					if ( TIXML_SUCCESS != pm->QueryIntAttribute("value", &intSettings[i])) 
					{
						fprintf(stderr, "unable to load '%s' setting. Using default value instead\n", intTags[i]);
						intSettings[i]=intDefaults[i];
						return 4;
					}
					break;
				}
				pm = pm->NextSiblingElement();
				}
				if (pNode->NoChildren()) break; else pNode = pNode->NextSiblingElement();
			}
		}
		else
		if (!strcmp(child->Value(),"str"))
		{
			sv2 = false;
			pNode = child->FirstChildElement();
			const char * tst ="";
			while (pNode)
			{
				if (pNode) tst = pNode->GetText(); else break;
				const char * tag = pNode->Value();
				
				for( int i=0; i<s_LAST; i++)
				if ( !strcmp(tag,strTags[i]) )
				{
					fv2[i] = false;
					strSettings[i] = tst;
					break;
				}

				if (pNode->NoChildren()) break; 
					else pNode = pNode->NextSiblingElement();
			}
		}
	}
	if ((sv)||(sv2)) return 4;
	bool g = false;
	for (int i=0;i<i_LAST;i++)
		if (fv[i])
		{
			g = true; break;
		}
	if (!g)
		for (int i=0;i<s_LAST;i++)
		{
			g = true; break;
		}
	if (g) return 4;
	return 0;
}

void AppSettingsMgr::save(const QString &aFileName) 
{
	bool b=QDir(".").mkpath(QString::fromStdString(dcpp::Util::getConfigPath()));
	if ((!xml.SaveFile(aFileName.toStdString()))||(!b)) fprintf(stderr,"Unable to save configuration file!");
}

void AppSettingsMgr::save() 
{
	QString tmp(QString::fromStdString(dcpp::Util::getConfigPath()));
	tmp+="stildcpp.xml";
	save(tmp);
}

const int AppSettingsMgr::load()
{
	QString tmp(QString::fromStdString(dcpp::Util::getConfigPath()));
	tmp+="stildcpp.xml";
	const int r = load(tmp);
	setDefaults();
	save();
	return r;
}

} //of namespace

