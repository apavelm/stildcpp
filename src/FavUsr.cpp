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

using namespace std;
using namespace dcpp;

int FavoriteUsersWindow::GetFavUserIndex(const QString & name)
{
	if (datalist.isEmpty()) return -1;
	for (int i = 0; i < datalist.size(); i++ )
		if ( StilUtils::TstrtoQ(Text::toT(datalist[i].getNick())) == name) return i;
	return -1;
}

FavoriteUser & FavoriteUsersWindow::GetFavUser(const QString & name)
{
	QList<FavoriteUser>::iterator i;
	for (i = datalist.begin(); i != datalist.end(); ++i)
		if ( StilUtils::TstrtoQ(Text::toT((*i).getNick())) == name) return *i;
}

void FavoriteUsersWindow::on_list_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	// Changing information in right panel
	FavoriteUser aUser = GetFavUser(c->text(COLUMN_NICK));
	UserInfo *ui = new UserInfo(aUser);
	
	cb_AutoSlot->setChecked(FavoriteUser::FLAG_GRANTSLOT);
	lbl_Hub->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_HUB)));
	lbl_time->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_SEEN)));
	lbl_CID->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_CID)));
	lbl_Desc->setText(StilUtils::TstrtoQ(ui->getText(COLUMN_DESCRIPTION)));
	//
	currentLabel->setText("<h1>" + c->text(COLUMN_NICK) + "</h1>");
	delete ui;
}

FavoriteUsersWindow::FavoriteUsersWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = 7;
	idText  = "";
	setTabText(tr("Favorite Users"));
	
	connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(on_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)) );
	connect(btn_Delete, SIGNAL(clicked()), this, SLOT(slot_remove_user()) );
	connect(btn_Ignore, SIGNAL(clicked()), this, SLOT(slot_ignore_user()) );
	connect(btn_Desc, SIGNAL(clicked()), this, SLOT(slot_desc_user()) );
	connect(cb_AutoSlot, SIGNAL(stateChanged(int)), this, SLOT(onAutoGrant(int)));
	
	connect(this, SIGNAL(sigSpeak(int, const UserPtr& )), this, SLOT(slotSpeak(int, const UserPtr& )), Qt::QueuedConnection);
	
	list->clear();
	datalist.clear();
	
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i) 
		addUser(i->second);

	FavoriteManager::getInstance()->addListener(this);
	
	list->setAlternatingRowColors(true);
	list->sortByColumn(0,Qt::AscendingOrder);
	list->setCurrentItem(list->topLevelItem(0));
}

void FavoriteUsersWindow::rebuilddatalist()
{
	datalist.clear();
	
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i) 
		datalist << i->second;
}

FavoriteUsersWindow::~FavoriteUsersWindow()
{
	datalist.clear();
	FavoriteManager::getInstance()->removeListener(this);
}

void FavoriteUsersWindow::addUser(const FavoriteUser& aUser) 
{
	datalist << aUser;
	QTreeWidgetItem *it = new QTreeWidgetItem(list);
	it->setText(0, StilUtils::TstrtoQ(Text::toT(aUser.getNick())));
	it->setIcon(0,QIcon(":/images/icon_error.png"));
}

void FavoriteUsersWindow::updateUser(const UserPtr& aUser) 
{
	for(int i = 0; i < datalist.size(); ++i) 
	{
		UserInfo *ui = new UserInfo(datalist[i]);
		if(ui->user == aUser) 
		{
			ui->columns[COLUMN_SEEN] = aUser->isOnline() ? _T("Online") : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", FavoriteManager::getInstance()->getLastSeen(aUser)));
			ui->columns[COLUMN_HUB] = aUser->isOnline() ? StilUtils::getHubNames(datalist[i].getUser()).first : Text::toT(datalist[i].getUrl());
		}
	}
}

void FavoriteUsersWindow::slot_remove_user()
{
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;
	UserInfo *ui = new UserInfo(datalist[GetFavUserIndex(it->text(0))]);
	qDebug() << "ssssssssss";
	if (ui) qDebug() << "ui - OK";
	
	// Next actions are equial, but SEGSEGV both =(
	//
	//ui->remove();
	//FavoriteManager::getInstance()->removeFavoriteUser(ui->user);
	qDebug() << "after";
	delete it;
	qDebug() << "deleted";
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
		FavoriteUser aUser = GetFavUser(it->text(COLUMN_NICK));
		UserInfo *ui = new UserInfo(aUser);
		lbl_Desc->setText(text);
		
		FavoriteManager::getInstance()->setUserDescription(ui->user, Text::fromT(StilUtils::QtoTstr(text)));
		rebuilddatalist();
		delete ui;
	}
}

void FavoriteUsersWindow::removeUser(const FavoriteUser& aUser) 
{
	QTreeWidgetItem *it = list->currentItem();
	for(int i = 0; i < datalist.size(); ++i) 
	{
		UserInfo *ui = new UserInfo(datalist[i]);
		if(ui->user == aUser.getUser()) 
		{
			// Next actions are equial, but SEGSEGV both =(
			//
			//FavoriteManager::getInstance()->removeFavoriteUser(ui->user);
			//ui->remove();
			if (it) delete it;
			return;
		}
	}
}

void FavoriteUsersWindow::onAutoGrant(int n)
{
	// I DON'T KNOW: IF IT'S WORKING
	if (!cb_AutoSlot->isEnabled()) return;
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;
	FavoriteUser f = GetFavUser(it->text(0));
	UserInfo *ui = new UserInfo(f);
	FavoriteManager::getInstance()->setAutoGrant(ui->user, (n));
	//rebuilddatalist();
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
	columns[COLUMN_SEEN] = user->isOnline() ? _T("Online") : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", u.getLastSeen()));
	columns[COLUMN_DESCRIPTION] = Text::toT(u.getDescription());
	columns[COLUMN_CID] = Text::toT(u.getUser()->getCID().toBase32());
}
