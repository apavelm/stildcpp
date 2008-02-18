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

#ifndef STIL_RICHTEXT_H
#define STIL_RICHTEXT_H

#include <QtGui>

#include "../iconset.h"

class TextIconFormat : public QTextCharFormat
{
public:
	TextIconFormat(const QString &iconName, const QString &text);

	enum Property {
		IconName = QTextFormat::UserProperty + 1,
		IconText = QTextFormat::UserProperty + 2
	};
};

class TextIconHandler : public QObject, public QTextObjectInterface
{
	Q_OBJECT
	Q_INTERFACES(QTextObjectInterface)
public:
	TextIconHandler(QObject *parent = 0);

	virtual QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format);
	virtual void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format);
};

class StilRichText
{
public:
	static void install(QTextDocument *doc);
	static void ensureTextLayouted(QTextDocument *doc, int documentWidth, Qt::Alignment align = Qt::AlignLeft, Qt::LayoutDirection layoutDirection = Qt::LeftToRight, bool textWordWrap = true);
	static void setText(QTextDocument *doc, const QString &text);
	static void appendText(QTextDocument *doc, QTextCursor &cursor, const QString &text);
	static QString convertToPlainText(const QTextDocument *doc);
};

#endif
