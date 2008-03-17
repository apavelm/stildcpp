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

#include "pm_win.h"

PMWindow::PMWindow(QWidget *parent, const UserPtr& replyTo, const tstring& aMessage) : MdiChild(parent), usr(replyTo)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_PRIVATE_CHAT;
	idText  = StilUtils::TstrtoQ(StilUtils::getNicks(replyTo));
	
	editor->clear();
	setupeditor();
	connect(SendBtn,SIGNAL(pressed()),this,SLOT(send_pm_msg()));
}

bool PMWindow::isOnline()
{
	return usr->isOnline();
}

void PMWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void PMWindow::send_pm_msg()
{
//	QFile file("/home/irq/stildcpp/src/main.cpp");
//	if (file.open(QFile::ReadOnly | QFile::Text))
//	editor->setPlainText(file.readAll());
}

// PRIVATE MESSAGE WINDOW
