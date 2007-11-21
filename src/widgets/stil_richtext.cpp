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


#include "stil_richtext.h"

static const int IconFormatType = QTextFormat::UserObject + 1;


//----------------------------------------------------------------------------
// TextIconFormat
//----------------------------------------------------------------------------

TextIconFormat::TextIconFormat(const QString &iconName, const QString &text) : QTextCharFormat()
{
	setObjectType(IconFormatType);
	QTextFormat::setProperty(IconName, iconName);
	QTextFormat::setProperty(IconText, text);
}

//----------------------------------------------------------------------------
// IconTextObjectInterface
//----------------------------------------------------------------------------

TextIconHandler::TextIconHandler(QObject *parent) : QObject(parent)
{
}

QSizeF TextIconHandler::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument)
	const QTextCharFormat charFormat = format.toCharFormat();
	return IconsetFactory::instance()->iconPixmap(charFormat.stringProperty(TextIconFormat::IconName)).size();
}

void TextIconHandler::drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
	Q_UNUSED(doc);
	Q_UNUSED(posInDocument);
	const QTextCharFormat charFormat = format.toCharFormat();
	const QPixmap pixmap = IconsetFactory::instance()->iconPixmap(charFormat.stringProperty(TextIconFormat::IconName));
	painter->drawPixmap(rect, pixmap, pixmap.rect());
}

//----------------------------------------------------------------------------
// StilRichText
//----------------------------------------------------------------------------

void StilRichText::install(QTextDocument *doc)
{
	Q_ASSERT(doc);
	static TextIconHandler *handler = 0;
	if (!handler) handler = new TextIconHandler(qApp);
	doc->documentLayout()->registerHandler(IconFormatType, handler);
}


// Make sure that QTextDocument has correctly layouted its text.
void StilRichText::ensureTextLayouted(QTextDocument *doc, int documentWidth, Qt::Alignment align, Qt::LayoutDirection layoutDirection, bool textWordWrap)
{
	Q_UNUSED(textWordWrap);
	Q_UNUSED(layoutDirection);
	Q_UNUSED(align);

	QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
	fmt.setMargin(0);
	doc->rootFrame()->setFrameFormat(fmt);
	doc->setTextWidth(documentWidth);
}

static QString unescape(const QString& escaped)
{
	QString plain = escaped;
	plain.replace("&lt;", "<");
	plain.replace("&gt;", ">");
	plain.replace("&amp;", "&");
	return plain;
}

typedef QQueue<TextIconFormat *> TextIconFormatQueue;

// Adds null format to queue for all ObjectReplacementCharacters that were already in the text. Returns passed \param text to save some code.
static QString preserveOriginalObjectReplacementCharacters(QString text, TextIconFormatQueue *queue)
{
	int objReplChars = 0;
	objReplChars += text.count(QChar::ObjectReplacementCharacter);
	objReplChars += text.count("<img ");
	for (int i = objReplChars; i; i--)
		queue->enqueue(0);
	return text;
}

// Replaces all <icon> tags with handy ObjectReplacementCharacters, and adds appropriate format to the \param queue. Returns processed \param text.
static QString convertIconsToObjectReplacementCharacters(QString text, TextIconFormatQueue *queue)
{
	// Format: <icon name="" text="">
	static QRegExp rxName("name=\"([^\"]+)\"");
	static QRegExp rxText("text=\"([^\"]+)\"");
	
	QString result;
	QString work = text;

	forever {
		int start = work.indexOf("<icon");
		if (start == -1)
			break;
		
		result += preserveOriginalObjectReplacementCharacters(work.left(start), queue);
		
		int end = work.indexOf(">", start);
		Q_ASSERT(end != -1);
		
		QString fragment = work.mid(start, end - start);
		if (rxName.indexIn(fragment) != -1) 
		{
			QString iconName = unescape(rxName.capturedTexts()[1]);
			QString iconText;
			if (rxText.indexIn(fragment) != -1)
				iconText = unescape(rxText.capturedTexts()[1]);
				
			queue->enqueue(new TextIconFormat(iconName, iconText));
			result += QChar::ObjectReplacementCharacter;
		}
		
		work = work.mid(end + 1);
	}
	return result + preserveOriginalObjectReplacementCharacters(work, queue);
}

// Applies text formats from \param queue to all ObjectReplacementCharacters in \param doc, starting from \param cursor's position.
static void applyFormatToIcons(QTextDocument *doc, TextIconFormatQueue *queue, QTextCursor &cursor)
{
	QTextCursor searchCursor = cursor;
	forever 
	{
		searchCursor = doc->find(QString(QChar::ObjectReplacementCharacter), searchCursor);
		if (searchCursor.isNull()) break;
		
		Q_ASSERT(!queue->isEmpty());
		TextIconFormat *format = queue->dequeue();
		if (format) 
		{ 
			searchCursor.setCharFormat(*format);
			delete format;
		}
	}
	// clear the selection that's left after successful QTextDocument::find()
	cursor.clearSelection();
}

// Sets entire contents of specified QTextDocument to text. \param text text to append to the QTextDocument. Please note that if you insert any <icon>s, attributes' values MUST be Qt::escaped.
void StilRichText::setText(QTextDocument *doc, const QString &text)
{
	QFont font = doc->defaultFont();
	doc->clear();
	QTextCursor cursor(doc);
	QTextCharFormat charFormat = cursor.charFormat();
	charFormat.setFont(font);
	cursor.setCharFormat(charFormat);
	appendText(doc, cursor, text);
}

// Appends a new paragraph with text to the end of the document. \param text text to append to the QTextDocument. Please note that if you             insert any <icon>s, attributes' values MUST be Qt::escaped.
void StilRichText::appendText(QTextDocument *doc, QTextCursor &cursor, const QString &text)
{
	cursor.beginEditBlock();
	cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
	cursor.clearSelection();
	if (!cursor.atBlockStart()) 
	{
		cursor.insertBlock();
		
		// clear trackbar for new blocks
		QTextBlockFormat blockFormat = cursor.blockFormat();
		blockFormat.clearProperty(QTextFormat::BlockTrailingHorizontalRulerWidth);
		cursor.setBlockFormat(blockFormat);
	}
	
	TextIconFormatQueue queue;
	// we need to save this to start searching from here when applying format to icons
	int initialpos = cursor.position();
	cursor.insertFragment(QTextDocumentFragment::fromHtml(convertIconsToObjectReplacementCharacters(text, &queue)));
	cursor.setPosition(initialpos);
	applyFormatToIcons(doc, &queue, cursor);
	
	cursor.endEditBlock();
}


// Call this function on your QTextDocument to get plain text representation, and all Icons will be replaced by their initial text.
QString StilRichText::convertToPlainText(const QTextDocument *doc)
{
	QString obrepl = QString(QChar::ObjectReplacementCharacter);
	QQueue<QTextCharFormat> queue;
	QTextCursor nc = doc->find(obrepl, 0);
	QTextCursor cursor;

	while (!nc.isNull()) {
		queue.enqueue(nc.charFormat());

		cursor = nc;
		nc = doc->find(obrepl, cursor);
	}

	QString raw = doc->toPlainText();
	QStringList parts = raw.split(obrepl);
	QString result = parts.at(0);

	for (int i = 1; i < parts.size(); ++i) {
		if (!queue.isEmpty()) {
			QTextCharFormat format = queue.dequeue();
			if ((format).objectType() == IconFormatType) 
			{
				result += format.stringProperty(TextIconFormat::IconText);
			}
		}
		result += parts.at(i);
	}
	return result;
}

