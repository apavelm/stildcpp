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
 
#include "UserInfoBase.h"

#include "client/QueueManager.h"
#include "client/LogManager.h"
#include "client/FavoriteManager.h"
#include "client/UploadManager.h"
#include "client/User.h"

//#include "PrivateFrame.h"
//#include "hub_win.h"

using namespace std;
using namespace dcpp;

void UserInfoBase::matchQueue() {
	try {
		QueueManager::getInstance()->addList(user, QueueItem::FLAG_MATCH_QUEUE);
	} catch(const Exception& e) {
		LogManager::getInstance()->message(e.getError());
	}
}
void UserInfoBase::getList() {
	try {
		QueueManager::getInstance()->addList(user, QueueItem::FLAG_CLIENT_VIEW);
	} catch(const Exception& e) {
		LogManager::getInstance()->message(e.getError());
	}
}
void UserInfoBase::browseList() {
	if(user->getCID().isZero())
		return;
	try {
		QueueManager::getInstance()->addPfs(user, "");
	} catch(const Exception& e) {
		LogManager::getInstance()->message(e.getError());
	}
}

void UserInfoBase::addFav() {
	FavoriteManager::getInstance()->addFavoriteUser(user);
}

void UserInfoBase::pm(QWidget* mdiParent) {
	//OpenPM(mdiParent, user);
}

void UserInfoBase::grant() {
	UploadManager::getInstance()->reserveSlot(user);
}

void UserInfoBase::removeAll() {
	QueueManager::getInstance()->removeSource(user, QueueItem::Source::FLAG_REMOVED);
}

void UserInfoBase::UserTraits::operator()(UserInfoBase* ui) { 
	if(ui->getUser()->isSet(User::NMDC)) 
		adcOnly = false;
	bool fav = FavoriteManager::getInstance()->isFavoriteUser(ui->getUser());
	if(fav)
		nonFavOnly = false;
	if(!fav)
		favOnly = false;
}

void UserInfoBase::connectFav(QWidget* mdiParent) {
	dcpp::tstring url = dcpp::Text::toT(FavoriteManager::getInstance()->getUserURL(user));
	if(!url.empty()) {
		//OpenHub(mdiParent, url);
	}
}
