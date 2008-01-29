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

#include "FavUsr.h"

void FavoriteUsersWindow::on_list_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	qDebug() << "on_list_currentItemChanged 1";
	if (!c) return;
	if (datalist.isEmpty()) return;
	// Changing information in right panel
	qDebug() << "on_list_currentItemChanged : step 1";
	int fff = datalistitem.indexOf(list->indexFromItem(c));
	if (fff<0) return; //DEBUG HACK. DELETE IT AFTER
	qDebug() << "on_list_currentItemChanged : step 1.2" << fff; 
	UserInfo *ui = datalist.at(fff);
	qDebug() << "on_list_currentItemChanged : step 2";
	cb_AutoSlot->setChecked(FavoriteUser::FLAG_GRANTSLOT);
	lbl_Hub->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_HUB)));
	lbl_time->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_SEEN)));
	lbl_CID->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_CID)));
	lbl_Desc->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_DESCRIPTION)));
	qDebug() << "on_list_currentItemChanged : step 3";
	//
	currentLabel->setText("<h1>" + c->text(COLUMN_NICK) + "</h1>");
	qDebug() << "on_list_currentItemChanged 2";
}

FavoriteUsersWindow::FavoriteUsersWindow(QWidget *parent) : MdiChild(parent)
{
	qDebug() << "constructor 1";
	setupUi(this);
	type = 7;
	idText  = "";
	setTabText(tr("Favorite Users"));
	
	datalist.clear();
	datalistitem.clear();
	list->clear();
	
	list->setAlternatingRowColors(true);
	list->sortByColumn(0,Qt::AscendingOrder);
	
	connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(on_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)) );
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
	qDebug() << "constructor 2";
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
	qDebug() << "addUser 1";
	datalist << new UserInfo(aUser);
	setUpdatesEnabled(false);
	QTreeWidgetItem *it = new QTreeWidgetItem(list);
	it->setText(0, StilUtils::TstrtoQ(Text::toT(aUser.getNick())));
	it->setIcon(0,QIcon(":/images/icon_error.png"));
	datalistitem << list->indexFromItem(it);
	setUpdatesEnabled(true);
	qDebug() << "addUser 2";
}

void FavoriteUsersWindow::updateUser(const UserPtr& aUser) 
{
	qDebug() << "UpdateUser 1";
	for(int i = 0; i < datalist.size(); ++i) 
	{
		UserInfo *ui = datalist.at(i);
		if(ui->user == aUser) 
		{
			ui->columns[COLUMN_SEEN] = aUser->isOnline() ? T_("Online") : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", FavoriteManager::getInstance()->getLastSeen(aUser)));
		}
	}
	qDebug() << "UpdateUser 2";
}

void FavoriteUsersWindow::slot_remove_user()
{
	qDebug() << "SlotRemoveUser 1";
	qDebug() << "SlotRemoveUser 1.1: was: " << datalist.size();
	qDebug() << "SlotRemoveUser 1.2: was: " << datalistitem.size();
	QList<QTreeWidgetItem *> lt = list->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		UserInfo * ui = datalist.at(datalistitem.indexOf(list->indexFromItem(lt[0])));
		ui->remove();
	}
	qDebug() << "SlotRemoveUser 1.8: now: " << datalistitem.size();
	qDebug() << "SlotRemoveUser 1.9: now: " << datalist.size();
	qDebug() << "SlotRemoveUser 2";
}

void FavoriteUsersWindow::removeUser(const FavoriteUser& aUser)
{
	qDebug() << "RemoveUser 1";
	if (!datalist.isEmpty())
		for(int i = 0; i < datalist.size(); ++i) 
		{
			UserInfo *ui = datalist.at(i);
			if(ui->user == aUser.getUser()) 
			{
				// DELETING 
				setUpdatesEnabled(false);
				delete list->itemFromIndex(datalistitem.at(i));
				datalistitem.removeAt(i);
				qDebug() << "TEST ";
				qDebug() << "TEST size" << datalist.size();
				qDebug() << "TEST :" << i;
				qDebug() << "TEST _bool_:" << (datalist.at(i));
				datalist.removeAt(i);
				qDebug() << "TEST size" << datalist.size();
				qDebug() << "TEST _bool_:" << (datalist.at(i));
				setUpdatesEnabled(true);
				// END OF DELETING
				qDebug() << "RemoveUser 2";
				return;
			}
		}
	qDebug() << "RemoveUser 0";
}

void FavoriteUsersWindow::slot_ignore_user()
{
	
}

void FavoriteUsersWindow::slot_desc_user()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Change User Description"), tr("New Description:"), QLineEdit::Normal, lbl_Desc->text() , &ok);
	if (ok) 
	{
		QTreeWidgetItem *it = list->currentItem();
		if (!it) return;
		UserInfo *ui = datalist.at(datalistitem.indexOf(list->indexFromItem(it)));
		lbl_Desc->setText(text);
		
		FavoriteManager::getInstance()->setUserDescription(ui->user, Text::fromT(StilUtils::QtoTstr(text)));
	}
}

void FavoriteUsersWindow::onAutoGrant(int value)
{
	qDebug() << "onAutoGrant 1";
	if (!cb_AutoSlot->isEnabled()) return;
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;
	UserInfo *ui = datalist.at(datalistitem.indexOf(list->indexFromItem(it)));
	if (ui) FavoriteManager::getInstance()->setAutoGrant(ui->user, static_cast<bool>(value));
	qDebug() << "onAutoGrant 2";
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
	if (a == USER_UPDATED)
	{
		UserInfoBase uib = aUser; //FIXME
		updateUser(uib.user);
	}
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
}
