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

#include "useractmenu.h"

void UserActionMenu::sendPubM()
{
	emit sig_sendPublicMessage(s_Nick+": ");
}

QMenu * UserActionMenu::getMenuForUser(const QString & nick)
{
	s_Nick = nick;
	QMenu * r = new QMenu;
	
	QAction * header = new QAction(nick, this);
	header->setEnabled(false);
	
	r->addAction(header);
	r->addAction(a_getFileList(nick));
	r->addAction(a_MatchQueue(nick));
	r->addSeparator();
	r->addAction(a_SendPrivateMessage(nick));
	r->addAction(a_SendPublicMessage(nick));
	r->addSeparator();
	r->addAction(a_AddtoFav(nick));
	r->addAction(a_GrantExtraSlot(nick));
	r->addSeparator();
	r->addAction(a_RemoveUserFromQueue(nick));
	r->addAction(a_CopyNickToClipboard(nick));
	
	return r;
}

QAction * UserActionMenu::a_getFileList(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Get File List"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_MatchQueue(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Match Queue"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_SendPrivateMessage(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Send Private Message"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_SendPublicMessage(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Send Public Message"), this);
	
	connect(r, SIGNAL(triggered()), this, SLOT(sendPubM()) );
	return r;
}

QAction * UserActionMenu::a_AddtoFav(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Add to Favorites"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_GrantExtraSlot(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Grant Extra Slot"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_RemoveUserFromQueue(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Remove User From Queue"), this);
	
	//insert code here
	
	return r;
}

QAction * UserActionMenu::a_CopyNickToClipboard(const QString & nick)
{
	s_Nick = nick;
	QAction * r = new QAction(tr("Copy Nick to Clipboard"), this);
	
	//insert code here
	
	return r;
}
