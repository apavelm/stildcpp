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

#include <QAbstractTextDocumentLayout>
#include <QAction>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QLayout>
#include <QMenu>
#include <QResizeEvent>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QTimer>

#include "stil_chatctrls.h"

//----------------------------------------------------------------------------
// ChatView
//----------------------------------------------------------------------------
ChatView::ChatView(QWidget *parent) : StilTextView(parent)
{
	_timefmt = "hh:mm:ss";
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	setReadOnly(true);
	setUndoRedoEnabled(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#ifndef Q_WS_X11 // linux has this feature built-in
	connect(this, SIGNAL(selectionChanged()), SLOT(autoCopy()));
	connect(this, SIGNAL(cursorPositionChanged()), SLOT(autoCopy()));
#endif
}

void ChatView::keyPressEvent(QKeyEvent *e)
{
	e->ignore();
}

void ChatView::appendText(const QString &text)
{
	bool doScrollToBottom = atBottom();
	
	// prevent scrolling back to selected text when 
	// restoring selection
	int scrollbarValue = verticalScrollBar()->value();
	
	StilTextView::appendText(text);
	
	if (doScrollToBottom)
		scrollToBottom();
	else
		verticalScrollBar()->setValue(scrollbarValue);
}

void ChatView::SetTimeFormatString(const QString &a)
{
	_timefmt=a;
}

QString ChatView::formatTimeStamp(const QDateTime &time)
{
	return time.toString(_timefmt);
}

//----------------------------------------------------------------------------
// LineEdit
//----------------------------------------------------------------------------

bool LineEdit::focusNextPrevChild(bool next)
{
	return QWidget::focusNextPrevChild(next);
}

void LineEdit::keyPressEvent(QKeyEvent *e)
{
	if ( ( (e->key() ==  Qt::Key_Enter) || ( e->key() ==  Qt::Key_Return ) ) &&  ( (e->modifiers() & Qt::ControlModifier) || (e->modifiers() & Qt::ShiftModifier) ) ) 
	{
		e->ignore();
		QTextEdit::keyPressEvent(e);
	} else
	if ( (e->key() ==  Qt::Key_Enter )  || ( e->key() == Qt::Key_Return ) )
	{
		if (!toPlainText().isEmpty())
		{
			emit SendText(toPlainText());
			clear();
		}
	}
	else
	{
		QTextEdit::keyPressEvent(e);
	}
}

LineEdit::LineEdit( QWidget *parent) : QTextEdit(parent)
{
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere); // no need for horizontal scrollbar with this
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setMinimumHeight(minimumSizeHint().height());

	connect(this, SIGNAL(textChanged()), SLOT(recalculateSize()));
	
	setAcceptRichText(false); // FROM ChatEdit
}

LineEdit::~LineEdit()
{
}

QSize LineEdit::minimumSizeHint() const
{
	QSize sh = QTextEdit::minimumSizeHint();
	sh.setHeight(fontMetrics().height() + 1);
	sh += QSize(0, QFrame::lineWidth() * 2);
	return sh;
}

QSize LineEdit::sizeHint() const
{
	QSize sh = QTextEdit::sizeHint();
	sh.setHeight(int(document()->documentLayout()->documentSize().height())+6); //FIXME: "6" - Magic Number
	sh += QSize(0, QFrame::lineWidth() * 2);
	((QTextEdit*)this)->setMaximumHeight(sh.height());
	return sh;
}

void LineEdit::resizeEvent(QResizeEvent* e)
{
	QTextEdit::resizeEvent(e);
}

void LineEdit::recalculateSize()
{
	updateGeometry();
}

void LineEdit::updateScrollBar()
{
	setVerticalScrollBarPolicy(sizeHint().height() > height() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);
	ensureCursorVisible();
}
