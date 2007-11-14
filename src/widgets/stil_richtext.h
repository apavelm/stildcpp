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

#include <QTextDocument>
#include <QTextCursor>

class StilRichText
{
public:
	static void install(QTextDocument *doc);
	static void ensureTextLayouted(QTextDocument *doc, int documentWidth, Qt::Alignment align = Qt::AlignLeft, Qt::LayoutDirection layoutDirection = Qt::LeftToRight, bool textWordWrap = true);
	static void setText(QTextDocument *doc, const QString &text);
	static void insertIcon(QTextCursor &cursor, const QString &iconName, const QString &iconText);
	static void appendText(QTextDocument *doc, QTextCursor &cursor, const QString &text);
	static QString convertToPlainText(const QTextDocument *doc);
};
