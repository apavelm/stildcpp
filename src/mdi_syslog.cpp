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

#include "mdi_syslog.h"

SysLogWindow::~SysLogWindow()
{
	LogManager::getInstance()->removeListener(this);
}

SysLogWindow::SysLogWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_SYSTEM_LOG;
	idText = "System Log";
	setTabText(tr("System Log"));
	
	QFont f = textEdit->font();
	f.setPointSize(APPSETTING(i_SYSLOGFONTSIZE));
	textEdit->setFont(f);
	
	connect(this, SIGNAL(sigSpeak(time_t, tstring)), this, SLOT(slotSpeak(time_t, tstring)), Qt::QueuedConnection);
	
	deque<pair<time_t, string> > oldMessages = LogManager::getInstance()->getLastLogs();
	// Technically, we might miss a message or two here, but who cares...
	LogManager::getInstance()->addListener(this);
	
	for(deque<pair<time_t, string> >::iterator i = oldMessages.begin(); i != oldMessages.end(); ++i) {
		addLine(i->first, Text::toT(i->second));
	}
}

void SysLogWindow::addLine(time_t t, const tstring& msg)
{
	QTextCursor cursor(textEdit->document());
	int p = cursor.position();
	cursor.movePosition(QTextCursor::End);
	cursor.beginEditBlock();
	cursor.insertText( "\r\n[" + StilUtils::TstrtoQ(Text::toT(Util::getShortTimeString(t))) + "] " + StilUtils::TstrtoQ(msg) );
	cursor.endEditBlock();
	cursor.setPosition(p);
}

void SysLogWindow::slotSpeak(time_t t, tstring s)
{
	addLine(t, s);
}

void SysLogWindow::on(Message, time_t tm, const string& message) throw() 
{ 
	emit sigSpeak(tm, Text::toT(message)); 
}
