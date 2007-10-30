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

#include "hub_win.h"

HubWindow::HubWindow(QWidget *parent, const QString &hubaddr) : MdiChild(parent)
{	
	setupUi(this);
	type = 1;
	idText  = hubaddr;
	setWindowTitle(tr("Hub: ")+hubaddr);
	editor->clear();
	setupeditor();
	connect(SendBtn,SIGNAL(pressed()),this,SLOT(send_msg()));
}

void HubWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void HubWindow::send_msg()
{
//	QFile file("/home/irq/test.txt");
//	if (file.open(QFile::ReadOnly | QFile::Text))
//	editor->setPlainText(file.readAll());
	
//	editor->document()->addResource(QTextDocument::ImageResource, QUrl(":)"), QImage(":/images/emotions/default/grin.png"));
//	lineEdit->document()->addResource(QTextDocument::ImageResource, QUrl(":)"), QImage(":/images/emotions/default/grin.png"));
//	editor->textCursor().movePosition(QTextCursor::End);
//	editor->textCursor().insertImage(":)");
	
}

// HubWindow
