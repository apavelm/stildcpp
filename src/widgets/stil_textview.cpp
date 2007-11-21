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


#include "stil_textview.h"



URLObject *URLObject::getInstance()
{
	static URLObject *urlObject = 0;
	if (!urlObject)
		urlObject = new URLObject();
	return urlObject;
}

QMenu *URLObject::createPopupMenu(const QString &lnk)
{
	_link = lnk;
	if ( _link.isEmpty() )
		return 0;

	int colon = _link.indexOf(':');
	if ( colon == -1 )
		colon = 0;
	QString service = _link.left( colon );

	QMenu *m = new QMenu;
	
	if ( service == "mailto") 
	{
		m->addAction(_act_mailto);
	}
	else if (service == "magnet")
	{
		m->addAction(_act_magnet);
	}
	else //if ( service == "http" || service == "https" || service.isEmpty() ) 
	{
		m->addAction(_act_browser);
	}
	
	m->addSeparator();
	m->addAction(_act_copy);
	
	return m;
}

//----------------------------------------------------------------------------
// StilTextView
//----------------------------------------------------------------------------

StilTextView::StilTextView(QWidget *parent) : QTextEdit(parent)
{

	setReadOnly(true);
	StilRichText::install(document());
	
	anchorOnMousePress = QString();
	hadSelectionOnMousePress = false;

	viewport()->setMouseTracking(true); // we want to get all mouseMoveEvents
}

// This function returns true if vertical scroll bar is  at its maximum position.
bool StilTextView::atBottom()
{
	// If errors is occured incrise 'diff' constant
	const int diff = fontMetrics().height() + 1;
	return (verticalScrollBar()->maximum() - verticalScrollBar()->value()) <= diff;
}

void StilTextView::scrollToBottom()
{
	verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void StilTextView::scrollToTop()
{
	verticalScrollBar()->setValue(verticalScrollBar()->minimum());
}

void StilTextView::appendText(const QString &text)
{
	QTextCursor cursor = textCursor();
	Selection selection = saveSelection(cursor);
	
	StilRichText::appendText(document(), cursor, text);
	
	restoreSelection(cursor, selection);
	setTextCursor(cursor);
}

StilTextView::Selection StilTextView::saveSelection(QTextCursor &cursor)
{
	Selection selection;
	selection.start = selection.end = -1;

	if (cursor.hasSelection())
	{
		selection.start = cursor.selectionStart();
		selection.end   = cursor.selectionEnd();
	}
	
	return selection;
}

void StilTextView::restoreSelection(QTextCursor &cursor, Selection selection)
{
	if (selection.start != -1 && selection.end != -1) 
	{
		cursor.setPosition(selection.start, QTextCursor::MoveAnchor);
		cursor.setPosition(selection.end,   QTextCursor::KeepAnchor);
	}
}

// Returns HTML markup for selected text. If no text is selected, returns HTML markup for all text.
QString StilTextView::getHtml() const
{
	StilTextView *tv = (StilTextView *)this;
	QTextCursor cursor = tv->textCursor();
	int position = verticalScrollBar()->value();
	
	bool unselectAll = false;
	
	QMimeData *mime = createMimeDataFromSelection();
	QString result = mime->html();
	delete mime;
	
	// we need to restore original position if selectAll() 
	// was called, because setTextCursor() (which is necessary
	// to clear selection) will move vertical scroll bar
	if (unselectAll) {
		cursor.clearSelection();
		tv->setTextCursor(cursor);
		tv->verticalScrollBar()->setValue(position);
	}
	
	return result;
}

void StilTextView::contextMenuEvent(QContextMenuEvent *e) 
{
	QMenu *menu;
	if (!anchorAt(e->pos()).isEmpty())
		menu = URLObject::getInstance()->createPopupMenu(anchorAt(e->pos()));
	else
	{
		QTextCursor tu = textCursor();
		Selection s = saveSelection(tu);
		QTextCursor tc = cursorForPosition(last_click_);
		int current_position = textCursor().position();
		tc.select(QTextCursor::WordUnderCursor);
		QString txt = tc.selectedText();
		
		// TODO: if ( isUserOnline(txt) && isUser(txt) ) specialMenu; else
			menu = createStandardContextMenu(); 
		
		tc.setPosition(current_position);
		restoreSelection(tc, s);
		setTextCursor(tc);
	}
	menu->exec(e->globalPos());
	e->accept();
	delete menu;
}

// Copied (with modifications) from QTextBrowser
void StilTextView::mouseMoveEvent(QMouseEvent *e)
{
	QTextEdit::mouseMoveEvent(e);

	QString anchor = anchorAt(e->pos());
	viewport()->setCursor(anchor.isEmpty() ? Qt::ArrowCursor : Qt::PointingHandCursor);
}

// Copied (with modifications) from QTextBrowser
void StilTextView::mousePressEvent(QMouseEvent *e)
{
	last_click_=e->pos();
	anchorOnMousePress = anchorAt(e->pos());
	if (!textCursor().hasSelection() && !anchorOnMousePress.isEmpty()) 
	{
		QTextCursor cursor = textCursor();
		QPoint mapped = QPoint(e->pos().x() + horizontalScrollBar()->value(), e->pos().y() + verticalScrollBar()->value());
		const int cursorPos = document()->documentLayout()->hitTest(mapped, Qt::FuzzyHit);
		if (cursorPos != -1)
			cursor.setPosition(cursorPos);
		setTextCursor(cursor);
	}

	QTextEdit::mousePressEvent(e);

	hadSelectionOnMousePress = textCursor().hasSelection();
}

// Copied (with modifications) from QTextBrowser
void StilTextView::mouseReleaseEvent(QMouseEvent *e)
{
	QTextEdit::mouseReleaseEvent(e);
	
	if (!(e->button() & Qt::LeftButton)) return;

	const QString anchor = anchorAt(e->pos());
	if (anchor.isEmpty()) return;
	
	if (!textCursor().hasSelection() || (anchor == anchorOnMousePress && hadSelectionOnMousePress))
		URLObject::getInstance()->popupAction(anchor);
}

// This is overridden in order to properly convert Icons back to their original text.
QMimeData *StilTextView::createMimeDataFromSelection() const
{
	QTextDocument *doc = new QTextDocument();
	QTextCursor cursor(doc);
	cursor.insertFragment(textCursor().selection());
	QString text = StilRichText::convertToPlainText(doc);
	delete doc;
	
	QMimeData *data = new QMimeData;
	data->setText(text);
	data->setHtml(Qt::convertFromPlainText(text));
	return data;
}

// Scrolling to bottom, if resized
void StilTextView::resizeEvent(QResizeEvent *e)
{
	bool atEnd = verticalScrollBar()->value() == verticalScrollBar()->maximum();
	bool atStart = verticalScrollBar()->value() == verticalScrollBar()->minimum();
	double value = 0;
	if (!atEnd && !atStart)
		value = (double)verticalScrollBar()->maximum() / (double)verticalScrollBar()->value();

	QTextEdit::resizeEvent(e);

	if (atEnd)
		verticalScrollBar()->setValue(verticalScrollBar()->maximum());
	else 
		if (value != 0)
			verticalScrollBar()->setValue((int) ((double)verticalScrollBar()->maximum() / value));
}

