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


#ifndef ICONSET_H
#define ICONSET_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QtCore>
#include <QIcon>
#include <QRegExp>
#include <QtXml>

#include "zstream.h"

class SmileIcon : public QObject
{
	Q_OBJECT
public:
	SmileIcon(QObject * parent = 0): QObject(parent), _name(""), _regExp(""), _icon(0) { _text.clear(); }
	SmileIcon(SmileIcon &from);
	
	~SmileIcon() { }

	const QPixmap &GetIcon() { return _icon; }
	QStringList &GetText() { return _text; }
	const QRegExp &GetRegExp() { return _regExp; }
	QString GetName() { return _name; }
	
	void setName(const QString &nm) { _name = nm; }
	void setRegExp(QRegExp rx) { _regExp = rx; }
	void setText(const QStringList &txt) { _text = txt; }
	void setIcon(QPixmap &icn) { _icon = icn; }
	void setIcon(const QByteArray &ba) { _icon.loadFromData(ba); }
	
private:
	QString _name;
	QRegExp _regExp;
	QStringList _text;
	QPixmap _icon;
};


class Iconset
{
public:
	Iconset();
	Iconset(const Iconset &);
	~Iconset();

	Iconset &operator+=(const Iconset &);

	int count() const;

	bool load(const QString &filename);

	SmileIcon *icon(const QString &);
	void addIcon(const QString &, SmileIcon &);
	void removeIcon(const QString &);

	const QString &name() const { return _name; };
	const QString &version() const { return _version; };
	const QString &description() const { return _description; };
	const QStringList &authors() const { return _authors; };
	const QString &homeUrl() const { return _homeUrl; };
	const QString &filename() const { return _filename; };
	
	///////////////////////////////
	void _clear();
	void _append(QString n, SmileIcon *icon);
	////////////////////////


	void setFileName(const QString &f) { _filename = f; };
	void setInformation(const Iconset &from);


	QListIterator<SmileIcon *> iterator();
	void addToFactory() const;
	void removeFromFactory() const;

private:
	QString _name, _version, _description, _homeUrl, _filename;
	QStringList _authors;
	QHash<QString, SmileIcon *> _dict;	// unsorted hash for fast search
	QList<SmileIcon *> _list;		// sorted list
};

class IconsetFactory : public QObject
{
	Q_OBJECT
private:
	IconsetFactory() : QObject(0) , iconsets_(0) { }

	~IconsetFactory()
	{
		if (iconsets_) 
		{
			while (!iconsets_->empty())
				delete iconsets_->takeFirst();
			delete iconsets_;
			iconsets_ = 0;
		}
	}
	
	static IconsetFactory* instance_;
	QList<Iconset*>* iconsets_;
	
public:
	Iconset * GetIconset() { return iconsets_->first(); }
	
	void registerIconset(const Iconset *i)
	{
		if (!iconsets_)
			iconsets_ = new QList<Iconset*>;
	
		if (!iconsets_->contains((Iconset*)i))
			iconsets_->append((Iconset*)i);
	}
	
	void unregisterIconset(const Iconset *i)
	{
		if (iconsets_ && iconsets_->contains((Iconset*)i)) iconsets_->removeAll((Iconset*)i);
	}
	
	static IconsetFactory* instance()
	{
		if (!instance_) {
			instance_ = new IconsetFactory();
		}
		return instance_;
	}

	SmileIcon *icon(const QString &name)
	{
		if (!iconsets_)
			return 0;
	
		SmileIcon *i = 0;
		Iconset *iconset;
		foreach (iconset, *iconsets_) {
			if ( iconset )
				i = iconset->icon(name);
	
			if ( i )
				break;
		}
		return i;
	}
	
	const QPixmap &iconPixmap(const QString &name) { return icon(name)->GetIcon(); }

	const QStringList icons() const
	{
		QStringList list;

		Iconset* iconset;
		foreach(iconset, *iconsets_) {
			QListIterator<SmileIcon*> it = iconset->iterator();
			while (it.hasNext())
				list << it.next()->GetName();
		}

		return list;
	}
	
};

#endif
