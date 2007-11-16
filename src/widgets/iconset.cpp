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


#include "iconset.h"

IconsetFactoryPrivate* IconsetFactoryPrivate::instance_ = NULL;

//----------------------------------------------------------------------------
// SmileIcon
//----------------------------------------------------------------------------

SmileIcon::SmileIcon() 
{
	
}

SmileIcon::~SmileIcon()
{
	
}

SmileIcon::SmileIcon(SmileIcon &from)
{
	_icon = from.GetIcon();
	_text = from.GetText();
	_name = from.GetName();
	_regExp = from.GetRegExp();
}

void SmileIcon::setIcon(const QByteArray &ba)
{
	_icon.loadFromData(ba);
}

void SmileIcon::setName(const QString &nm)
{
	_name = nm;
}

void SmileIcon::setRegExp(QRegExp rx)
{
	_regExp = rx;
}

void SmileIcon::setText(const QList<IconText> &txt)
{
	_text = txt;
}

void SmileIcon::setIcon(QPixmap &icn)
{
	_icon = icn;
}

//----------------------------------------------------------------------------
// IconsetFactory
//----------------------------------------------------------------------------

SmileIcon *IconsetFactory::iconPtr(const QString &name)
{
	SmileIcon *i = IconsetFactoryPrivate::instance()->icon(name);
	if ( !i ) qDebug("WARNING: IconsetFactory::icon(\"%s\"): icon not found", name.toStdString().c_str());

	return i;
}

SmileIcon IconsetFactory::icon(const QString &name)
{
	SmileIcon *i = iconPtr(name);
	if ( i ) return *i; 
}

const QPixmap &IconsetFactory::iconPixmap(const QString &name)
{
	SmileIcon *i = iconPtr(name);
	return i->GetIcon();
}

const QStringList IconsetFactory::icons()
{
	return IconsetFactoryPrivate::instance()->icons();
}

//----------------------------------------------------------------------------
// Iconset
//----------------------------------------------------------------------------

Iconset::Iconset()
{
	_name = "Unnamed";
	_version = QString::null;
	_description = QString::null;
	_homeUrl = QString::null;
}

Iconset::Iconset(const Iconset &from)
{
	setInformation(from);

	QListIterator<SmileIcon *> it( from._list );
	while ( it.hasNext() ) 
	{
		SmileIcon *icon = new SmileIcon(*it.next());
		_append(icon->GetName(), icon);
	}
}

Iconset::~Iconset()
{
	IconsetFactoryPrivate::instance()->unregisterIconset(this);
	_clear();
}

void Iconset::_append(QString n, SmileIcon *icon)
{
	if ( _dict.contains(n) )
		_remove(n);
		
	_dict[n] = icon;
	_list.append(icon);
}

void Iconset::_clear()
{
	_dict.clear();
	while ( !_list.isEmpty() )
		delete _list.takeFirst();
}

void Iconset::_remove(QString name)
{
	if ( _dict.contains(name) ) {
		SmileIcon *i = _dict[name];
		_dict.erase( _dict.find(name) );
		_list.removeAll(i);
		delete i;
	}
}

Iconset &Iconset::operator+=(const Iconset &i)
{
	QListIterator<SmileIcon *> it( i._list );
	while ( it.hasNext() ) {
		SmileIcon *icon = new SmileIcon(*it.next());
		_append( icon->GetName(), icon );
	}
	return *this;
}

int Iconset::count() const
{
	return _list.count();
}

QByteArray Iconset::loadData(const QString &fileName, const QString &dir)
{
	QByteArray ba;
	QFileInfo fi(dir);
	if ( fi.isDir() ) 
	{
		QFile file ( dir + "/" + fileName );
		if (!file.open(QIODevice::ReadOnly)) return ba;
		ba = file.readAll();
	}
	return ba;
}

bool Iconset::load(const QString &dir)
{
	//dir+"/icondef.xml"
	
	//////////////////////////////////////
	TiXmlDocument xml;
	xml.Clear();
	if ( !xml.LoadFile( (dir+"/imagedef.xml").toStdString() ) ) return false;
	
	TiXmlHandle hDoc(&xml);
	TiXmlHandle hRoot(0);
	TiXmlElement* pElem;
	TiXmlNode * child;
	TiXmlElement* pNode;
	TiXmlNode * parent;
	
	SmileIcon icon;
	QList<IconText> text;
	QString nm;
	
	const char * tst="";
	
	pElem=hDoc.FirstChildElement().Element();
	if (!pElem) return false;
	

	hRoot=TiXmlHandle(pElem);
	parent = hRoot.Node();
	child = 0;
	if (!strcmp(parent->Value(),"stil_smile_pack"))
	while( child = parent->IterateChildren( child ) )
	{
		if (!strcmp(child->Value(),"meta"))
		{
			pNode = child->FirstChildElement();
			while (pNode)
			{
				if (pNode) tst = pNode->GetText(); else break;
				
				const char * tag = pNode->Value();
				if ( !strcmp(tag,"name") ) _name = tst;
				else if ( !strcmp(tag,"version") ) _version = tst;
				else if ( !strcmp(tag,"description") ) _description = tst;
				else if ( !strcmp(tag,"author") ) _authors += tst;
				else if ( !strcmp(tag,"homeurl")  ) _homeUrl = tst;

				if (pNode->NoChildren()) break; 
					else pNode = pNode->NextSiblingElement();
			}
		}
		else
		if (!strcmp(child->Value(),"emo"))
		{
			pNode = child->FirstChildElement();

			while (pNode)
			{
				if (pNode) tst = pNode->GetText(); else break;
				
				const char * tag = pNode->Value();
				if ( !strcmp(tag,"smile") ) 
				{
					nm = tst;
				} else
				if ( !strcmp(tag,"text") ) 
				{
					QString lang = "";//e.attribute("xml:lang");
					//if ( lang.isEmpty() ) lang = ""; // otherwise there would be many warnings :-(
					text.append(IconText(lang, tst));
				} else
				if ( !strcmp(tag,"icon") ) 
				{
					QPixmap px;
					px.load(dir+"/"+tst);
					icon.setIcon(px);
				}

				icon.setText(text);
				icon.setName(nm);
				
				if (!pNode->NoChildren()) pNode = pNode->NextSiblingElement();
			}
				{
					// construct RegExp
					if ( text.count() ) 
					{
						QStringList regexp;
						foreach(IconText t, text) 
						{
							regexp += QRegExp::escape(t.text);
						}
					// make sure there is some form of whitespace on at least one side of the text string
					//regexp = QString("(\\b(%1))|((%2)\\b)").arg(regexp).arg(regexp);
					//
					// WAS: 
					icon.setRegExp( QRegExp(regexp.join("|") ));
					//icon.setRegExp ( QRegExp( "(\\b" + regexp.join(")|(") + "\\b)" ) );
					}
					_append( nm, new SmileIcon(icon) );
				}
		}
		
	} 
	return true;
}

SmileIcon *Iconset::icon(const QString &name)
{
	if ( _dict.isEmpty() ) return 0; else return _dict[name];
}

void Iconset::setIcon(const QString &name, SmileIcon &icon)
{
	SmileIcon *newIcon = new SmileIcon(icon);
	
	_remove( name );
	_append( name, newIcon );
}

void Iconset::removeIcon(const QString &name)
{ 
	_remove(name);
}

QListIterator<SmileIcon *> Iconset::iterator()
{
	QListIterator<SmileIcon *> it( _list );
	return it;
}

void Iconset::setInformation(const Iconset &from) 
{
	_name = from.name();
	_version = from.version();
	_description = from.description();
	_homeUrl = from.homeUrl();
	_filename = from.filename();
	_authors = from.authors();
	_info = from.info();
}

void Iconset::addToFactory() const 
{
	IconsetFactoryPrivate::instance()->registerIconset(this); 
}

void Iconset::removeFromFactory() const 
{
	IconsetFactoryPrivate::instance()->unregisterIconset(this); 
}
