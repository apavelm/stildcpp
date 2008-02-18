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


#include "iconset.h"

IconsetFactory* IconsetFactory::instance_ = NULL;

//----------------------------------------------------------------------------
// SmileIcon
//----------------------------------------------------------------------------

SmileIcon::SmileIcon(SmileIcon &from) :QObject(0)
{
	_icon = from.GetIcon();
	_name = from.GetName();
	_regExp = from.GetRegExp();
	_text.clear();
	_text = from.GetText(); 
}

//----------------------------------------------------------------------------
// Iconset
//----------------------------------------------------------------------------

Iconset::Iconset()
{
	_name = QString::null;
	_version = QString::null;
	_description = QString::null;
	_homeUrl = QString::null;
	_filename = QString::null;
	_list.empty();
	_dict.empty();
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
	IconsetFactory::instance()->unregisterIconset(this);
	_clear();
}

void Iconset::_append(QString n, SmileIcon *icon)
{
	if ( _dict.contains(n) )
		removeIcon(n);
		
	_dict[n] = icon;
	_list.append(icon);
}

void Iconset::_clear()
{
	_dict.clear();
	while ( !_list.isEmpty() )
		delete _list.takeFirst();
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

bool Iconset::load(const QString &filename)
{
	QByteArray data(zstream::getFileData(filename,"imagedef.xml"));
	if (data.isNull()) return false;

	QDomDocument doc;
	if (!doc.setContent(data)) return false;

	QDomElement icondef = doc.documentElement();
	if (icondef.tagName() != "stil_smile_pack") return false;

	// load meta info
	QDomElement meta = icondef.firstChildElement("meta");
	{
		_name = meta.firstChildElement("name").text();
		_version = meta.firstChildElement("version").text();
		_description = meta.firstChildElement("description").text();
		_homeUrl = meta.firstChildElement("homeurl").text();
		
		_authors.clear();
		QDomElement e = meta.firstChildElement("author");
		for (; !e.isNull(); e = e.nextSiblingElement("author")) _authors += e.text();
	}
	// load icon info
	QDomElement icn = icondef.firstChildElement("emo");
	for (; !icn.isNull(); icn = icn.nextSiblingElement("emo"))
	{
		
		QStringList texts;
		QStringList regexp;
		SmileIcon *c = new SmileIcon();
		QString nm;
		
		QDomElement ee = icn.firstChildElement("smile");
		nm = ee.text();

		ee = icn.firstChildElement("text");
		for (; !ee.isNull(); ee = ee.nextSiblingElement("text")) texts+=ee.text();
		if (texts.isEmpty()) return false;
	
		ee = icn.firstChildElement("icon");
		QPixmap pix;
		pix.loadFromData(zstream::getFileData(filename,ee.text()));
		if (!pix.isNull())
		{
			c->setName(nm);
			c->setText(texts);
			c->setIcon(pix);
			foreach(QString t, texts) regexp += QRegExp::escape(t);
			c->setRegExp( QRegExp(regexp.join("|") ));
		
			addIcon(nm,*c);
		}
	}
	_filename = filename;
	return !_list.isEmpty();
}

SmileIcon *Iconset::icon(const QString &name)
{
	if ( _dict.isEmpty() ) return 0; else return _dict[name];
}

void Iconset::addIcon(const QString &name, SmileIcon &icon)
{
	SmileIcon *newIcon = new SmileIcon(icon);
	removeIcon( name );
	_append( name, newIcon );
}

void Iconset::removeIcon(const QString &name)
{ 
	if ( _dict.contains(name) ) 
	{
		SmileIcon *i = _dict[name];
		_dict.erase( _dict.find(name) );
		_list.removeAll(i);
		delete i;
	}
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
}

void Iconset::addToFactory() const 
{
	IconsetFactory::instance()->registerIconset(this); 
}

void Iconset::removeFromFactory() const 
{
	IconsetFactory::instance()->unregisterIconset(this); 
}
