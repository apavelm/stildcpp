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


#ifndef TEXTUTIL_H
#define TEXTUTIL_H

#include <QString>
#include <QRegExp>
#include <QTextDocument> // for escape()
#include <QList>

#include "iconset.h"
#include "rtparse.h"

namespace TextUtil 
{
	QString quote(const QString &, int width=60, bool quoteEmpty=false);
	QString plain2rich(const QString &);
	QString rich2plain(const QString &);
	QString resolveEntities(const QString &);
	QString linkify(const QString &);
	QString legacyFormat(const QString &);
	QString emoticonify(const QString &in);
};

#endif
