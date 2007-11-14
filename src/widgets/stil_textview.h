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

#ifndef STIL_TEXTVIEW
#define STIL_TEXTVIEW

#include <QTextEdit>
#include <QMimeData>
#include <QTextCursor>

class StilTextView : public QTextEdit
{
	Q_OBJECT
public:
	StilTextView(QWidget *parent = 0);

	bool atBottom();

	virtual void appendText(const QString &text);	

	QString getHtml() const;
	
	struct Selection {
		int start, end;
	};
	Selection saveSelection(QTextCursor &cursor);
	void restoreSelection(QTextCursor &cursor, Selection selection);
	
public slots:
	void scrollToBottom();
	void scrollToTop();
	
protected:
	// make these functions unusable, because they modify
	// document structure and we can't override them to
	// handle Icons correctly
	void append(const QString &) { }
	void toHtml() const { }
	void toPlainText() const { }
	void insertHtml(const QString &) { }
	void insertPlainText(const QString &) { }
	void setHtml(const QString &) { }
	void setPlainText(const QString &) { }

	// reimplemented
	void contextMenuEvent(QContextMenuEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	QMimeData *createMimeDataFromSelection() const;
	void resizeEvent(QResizeEvent *);

	//class Private;
//private:
//	Private *d;
};

#endif
