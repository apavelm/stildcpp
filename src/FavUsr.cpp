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

FavoriteUsersWindow::FavoriteUsersWindow(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = 7;
	idText  = "";
	setWindowTitle("Fav_Usr");
	
	connect(this, SIGNAL(sigSpeak(int, const UserPtr& )), this, SLOT(slotSpeak(int, const UserPtr& )), Qt::QueuedConnection);
	
	list->clear();
	QStringList lst;
	QString tmp = StilUtils::TstrtoQ(Text::toT(SETTING(USERSFRAME_WIDTHS)));
	if (!tmp.isEmpty())
	{
		lst = tmp.split(",");
		for (int i=0; i<list->columnCount(); i++)
			list->setColumnWidth(i,lst[i].toLong());
	}
	
	FavoriteManager::FavoriteMap ul = FavoriteManager::getInstance()->getFavoriteUsers();
	for(FavoriteManager::FavoriteMap::iterator i = ul.begin(); i != ul.end(); ++i) 
		addUser(i->second);

	FavoriteManager::getInstance()->addListener(this);
	list->sortByColumn(0,Qt::AscendingOrder);
}

FavoriteUsersWindow::~FavoriteUsersWindow()
{
	//SettingsManager::getInstance()->set(SettingsManager::USERSFRAME_ORDER, WinUtil::toString(users->getColumnOrder()));
	QStringList lst;
	for (int i=0; i<list->columnCount(); i++)
		lst << QString::number(list->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::USERSFRAME_WIDTHS, Text::fromT(StilUtils::QtoTstr(lst.join(","))) );
	FavoriteManager::getInstance()->removeListener(this);
}

void FavoriteUsersWindow::addUser(const FavoriteUser& aUser) 
{
	QStringList lst;
	lst << "";//StilUtils::TstrtoQ(Text::toT(aUser.getNick()));
	lst << "dsfds";//aUser.user.isOnline() ? StilUtils::TstrtoQ(StilUtils::getHubNames(aUser.getUser()).first) : StilUtils::TstrtoQ(Text::toT(aUser.getUrl()));
	lst << "dfsd";//aUser.user.isOnline() ? "Online" : StilUtils::TstrtoQ(Text::toT(Util::formatTime("%Y-%m-%d %H:%M:%S", aUser.getLastSeen())));
	lst << StilUtils::TstrtoQ(Text::toT(aUser.getDescription()));
	lst << StilUtils::TstrtoQ(Text::toT(aUser.getUser()->getCID().toBase32()));
	
	QTreeWidgetItem *it = new QTreeWidgetItem(list, lst);
	
	QCheckBox * chk = new QCheckBox(StilUtils::TstrtoQ(Text::toT(aUser.getNick())));
	//connect(chk, SIGNAL(stateChanged()), this, SLOT(onAutoGrant(int)));
	chk->setChecked(FavoriteUser::FLAG_GRANTSLOT);
	list->setItemWidget(it, 0, chk);
}

void FavoriteUsersWindow::updateUser(const UserPtr& aUser) 
{
	//for(int i = 0; i < list->count(); ++i) 
	{
	//	UserInfo *ui = users->getData(i);
	//	if(ui->user == aUser) 
		{
	//		ui->columns[COLUMN_SEEN] = aUser->isOnline() ? TSTRING(ONLINE) : Text::toT(Util::formatTime("%Y-%m-%d %H:%M", FavoriteManager::getInstance()->getLastSeen(aUser)));
	//		users->update(i);
		}
	}
}

void FavoriteUsersWindow::removeUser(const FavoriteUser& aUser) 
{
//FavoriteManager::getInstance()->removeFavoriteUser(user); 	
	
	//for(int i = 0; i < list->count(); ++i) 
	{
	//	UserInfo *ui = list->getData(i);
	//	if(ui->user == aUser.getUser()) 
		{
			//QTreeWidgetItem *it = list->items
			//users->erase(i);
			
			return;
		}
	}
}

void FavoriteUsersWindow::onAutoGrant(int n)
{
	//FavoriteManager::getInstance()->setAutoGrant(users->getData(l->iItem)->user, users->isChecked(l->iItem));
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

