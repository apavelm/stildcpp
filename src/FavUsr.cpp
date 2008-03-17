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

#include "FavUsr.h"

void FavoriteUsersWindow::updateVisuals(UserInfo * ui)
{
	cb_AutoSlot->setChecked(ui->autoslot);
	lbl_Hub->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_HUB)));
	lbl_time->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_SEEN)));
	lbl_CID->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_CID)));
	lbl_Desc->setPlainText(StilUtils::TstrtoQ(ui->getText(COLUMN_DESCRIPTION)));
	currentLabel->setText("<h1>" + StilUtils::TstrtoQ(ui->getText(COLUMN_NICK)) + "</h1>");
}

void FavoriteUsersWindow::slot_list_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	if (datalist.isEmpty()) return;
	// Changing information in right panel
	UserInfo * ui = new UserInfo(datalist.at(datalistitem.indexOf(c)));
	updateVisuals(ui);
	delete ui;
}

void FavoriteUsersWindow::listkeyPressEvent(QKeyEvent *e)
{
	if ( e->key() == Qt::Key_Delete) slot_remove_user();
}

FavoriteUsersWindow::FavoriteUsersWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_FAVORITE_USERS;
	
	datalist.clear();
	datalistitem.clear();
	list->clear();
	
	list->setAlternatingRowColors(true);
	list->sortByColumn(0,Qt::AscendingOrder);
	
	connect(list, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(listkeyPressEvent(QKeyEvent *)) );
	connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(slot_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)) );
	connect(btn_Delete, SIGNAL(clicked()), this, SLOT(slot_remove_user()) );
	connect(btn_Ignore, SIGNAL(clicked()), this, SLOT(slot_ignore_user()) );
	connect(btn_Desc, SIGNAL(clicked()), this, SLOT(slot_desc_user()) );
	connect(cb_AutoSlot, SIGNAL(stateChanged(int)), this, SLOT(onAutoGrant(int)));
	
	connect(this, SIGNAL(sigSpeak(int, const UserPtr& )), this, SLOT(slotSpeak(int, const UserPtr& )), Qt::QueuedConnection);
	
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i) 
		addUser(i->second);

	FavoriteManager::getInstance()->addListener(this);
	
	list->setCurrentItem(list->topLevelItem(0));
}

FavoriteUsersWindow::~FavoriteUsersWindow()
{
	datalist.clear();
	datalistitem.clear();
	list->clear();
	FavoriteManager::getInstance()->removeListener(this);
}

void FavoriteUsersWindow::addUser(const FavoriteUser& aUser) 
{
	datalist << aUser;
	setUpdatesEnabled(false);
	QTreeWidgetItem *it = new QTreeWidgetItem(list);
	datalistitem << it;
	it->setText(0, StilUtils::TstrtoQ(Text::toT(aUser.getNick())));
	it->setIcon(0,QIcon(":/images/fav_user1.png")); // or "fav_user2.png" ??
	setUpdatesEnabled(true);
}

void FavoriteUsersWindow::updateUser(const UserPtr& aUser) 
{
	for(int i = 0; i < datalist.size(); ++i) 
	{
		UserInfo *ui = new UserInfo(datalist[i]);
		if(ui->user == aUser) 
		{
			ui->columns[COLUMN_SEEN] = aUser->isOnline() ? T_("Online") : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", FavoriteManager::getInstance()->getLastSeen(aUser)));
			lbl_time->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_SEEN)));
		}
		delete ui;
	}
}

void FavoriteUsersWindow::slot_remove_user()
{
	QList<QTreeWidgetItem *> lt = list->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		int idx = datalistitem.indexOf(lt[i]);
		UserInfo * ui = new UserInfo(datalist[idx]);
		ui->remove();
		delete ui;
	}
}

void FavoriteUsersWindow::removeUser(const FavoriteUser& aUser)
{
	if (!datalist.isEmpty())
		for(int i = 0; i < datalist.size(); ++i) 
			if(datalist[i].getUser() == aUser.getUser()) 
			{
				// DELETING 
				setUpdatesEnabled(false);
				delete datalistitem.at(i);
				datalistitem.removeAt(i);
				datalist.removeAt(i);
				setUpdatesEnabled(true);
				// END OF DELETING
				return;
			}
}

void FavoriteUsersWindow::slot_ignore_user()
{
	
}

void FavoriteUsersWindow::slot_desc_user()
{
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;
	int idx = datalistitem.indexOf(it);
	UserInfo *ui = new UserInfo(datalist[idx]);
	FavoriteManager::getInstance()->setUserDescription(ui->user, Text::fromT(StilUtils::QtoTstr(lbl_Desc->toPlainText())));
	// Updating in runtime
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i)
		if ( StilUtils::TstrtoQ(Text::toT(i->second.getNick())) == StilUtils::TstrtoQ(Text::toT(datalist[idx].getNick())) )
		{
			datalist[idx] = i->second;
			break;
		}
	// releasing memory
	delete ui;
}

void FavoriteUsersWindow::onAutoGrant(int value)
{
	if (!cb_AutoSlot->isEnabled()) return;
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;
	int idx = datalistitem.indexOf(it);
	UserInfo *ui = new UserInfo(datalist[idx]);
	FavoriteManager::getInstance()->setAutoGrant(ui->user, static_cast<bool>(value));
	// Updating in runtime
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i)
		if ( StilUtils::TstrtoQ(Text::toT(i->second.getNick())) == StilUtils::TstrtoQ(Text::toT(datalist[idx].getNick())) )
		{
			datalist[idx] = i->second;
			break;
		}
	// releasing memory
	delete ui;
}

void FavoriteUsersWindow::on(UserAdded, const FavoriteUser& aUser) throw() 
{
	addUser(aUser);
}

void FavoriteUsersWindow::on(UserRemoved, const FavoriteUser& aUser) throw() 
{
	removeUser(aUser);
}

void FavoriteUsersWindow::speak(int a, const UserPtr& aUser)
{
	emit sigSpeak(a, aUser);
}

void FavoriteUsersWindow::slotSpeak(int a, const UserPtr& aUser)
{
	if (a == USER_UPDATED) updateUser(aUser);
}

void FavoriteUsersWindow::on(StatusChanged, const UserPtr& aUser) throw() 
{
	speak(USER_UPDATED, aUser);
}

FavoriteUsersWindow::UserInfo::UserInfo(const FavoriteUser& u) : UserInfoBase(u.getUser()) 
{
	update(u);
}

void FavoriteUsersWindow::UserInfo::remove()
{ 
	FavoriteManager::getInstance()->removeFavoriteUser(user); 
}

void FavoriteUsersWindow::UserInfo::update(const FavoriteUser& u)
{
	columns[COLUMN_NICK] = Text::toT(u.getNick());
	columns[COLUMN_HUB] = user->isOnline() ? StilUtils::getHubNames(u.getUser()).first : Text::toT(u.getUrl());
	columns[COLUMN_SEEN] = user->isOnline() ? T_("Online") : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", u.getLastSeen()));
	columns[COLUMN_DESCRIPTION] = Text::toT(u.getDescription());
	columns[COLUMN_CID] = Text::toT(u.getUser()->getCID().toBase32());
	autoslot = u.isSet(FavoriteUser::FLAG_GRANTSLOT); 
}
