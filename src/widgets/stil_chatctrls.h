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

#ifndef STIL_CHATCTRLS_H
#define STIL_CHATCTRLS_H

#include <QTextEdit>
#include <QDateTime>

#include "stil_textview.h"

class ChatEdit;
class QEvent;
class QKeyEvent;
class QResizeEvent;
class QTimer;

class ChatView : public StilTextView
{
	Q_OBJECT
public:
	ChatView(QWidget* parent);
	~ChatView();

	void setDialog(QWidget* dialog);

	// reimplemented
	QSize sizeHint() const;

	void appendText(const QString &text);
	bool handleCopyEvent(QObject *object, QEvent *event, ChatEdit *chatEdit);

	QString formatTimeStamp(const QDateTime &time);

protected:
	// override the tab/esc behavior
	bool focusNextPrevChild(bool next);
	void keyPressEvent(QKeyEvent *);

protected slots:
	void autoCopy();

private:
	QWidget* dialog_;
};

class ChatEdit : public QTextEdit
{
	Q_OBJECT

public:
	ChatEdit(QWidget* parent);
	~ChatEdit();

	void setDialog(QWidget* dialog);

	// reimplemented
	QSize sizeHint() const;

protected slots:
 	void applySuggestion();
 	void addToDictionary();
	void optionsChanged();

protected:
	// override the tab/esc behavior
	bool focusNextPrevChild(bool next);
	void keyPressEvent(QKeyEvent *);
	void contextMenuEvent(QContextMenuEvent *e);

private:
	QWidget	*dialog_;
	QPoint last_click_;
	int previous_position_;
};


class LineEdit : public ChatEdit
{
	Q_OBJECT
public:
	LineEdit(QWidget* parent);
	~LineEdit();

	// reimplemented
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	// reimplemented
	void resizeEvent(QResizeEvent*);

private slots:
	void recalculateSize();
	void updateScrollBar();
};

#endif
