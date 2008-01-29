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

#include "mdi_syslog.h"

SysLogWindow::~SysLogWindow()
{
	LogManager::getInstance()->removeListener(this);
}

SysLogWindow::SysLogWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = 16;
	idText = "System Log";
	setTabText(tr("System Log"));
	
	QFont f = textEdit->font();
	f.setPointSize(APPSETTING(i_SYSLOGFONTSIZE));
	textEdit->setFont(f);
	
	connect(this, SIGNAL(sigSpeak(QDateTime, const QString &)), this, SLOT(slotSpeak(QDateTime, const QString & )), Qt::QueuedConnection);
	
	deque<pair<time_t, string> > oldMessages = LogManager::getInstance()->getLastLogs();
	// Technically, we might miss a message or two here, but who cares...
	LogManager::getInstance()->addListener(this);
	
	for(deque<pair<time_t, string> >::iterator i = oldMessages.begin(); i != oldMessages.end(); ++i) {
		addLine(i->first, Text::toT(i->second));
	}
}

QDateTime SysLogWindow::convTime(time_t tm)
{
	struct tm* t = localtime(&tm);

	return QDateTime(QDate(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday), QTime(t->tm_hour, t->tm_min, t->tm_sec), Qt::LocalTime); //Or may be Qt::UTC ???
}

void SysLogWindow::addLine(time_t tm, const tstring& msg)
{
	addLine(convTime(tm), StilUtils::TstrtoQ( msg ) );
}

void SysLogWindow::addLine(QDateTime t, const QString & msg) 
{
	textEdit->textCursor().insertText("[" + t.toString("hh:mm:ss") + "] " + msg+"\n");
	//setDirty(SettingsManager::BOLD_SYSTEM_LOG);
}

void SysLogWindow::slotSpeak(QDateTime t, const QString & s)
{
	addLine(t, s);
}

void SysLogWindow::on(Message, time_t tm, const string& message) throw() 
{ 
	emit sigSpeak(convTime(tm), StilUtils::TstrtoQ(Text::toT(message)) ); 
}
