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
 
#include "waitusr.h"
#include "mainwindowimpl.h"

WaitingUsersWindow::WaitingUsersWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_WAITING_USERS;
	datalist.clear();
	datalist2.clear();
	datalistitem.clear();
	queued->clear();
	
	connect(this, SIGNAL(sigSpeak(int, UserPtr, QString)), this, SLOT(slotSpeak(int, UserPtr, QString)), Qt::QueuedConnection);
	connect(queued, SIGNAL(sig_KeyPressEvent(QKeyEvent*)), this, SLOT(onKeyPress(QKeyEvent *)) );
	
	// CONTEXT MENU
	cnxtMenu = new QMenu();
	queued->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(queued, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	UploadManager::getInstance()->addListener(this);
	
	
	// Load queue
	UserList users = UploadManager::getInstance()->getWaitingUsers();
	for (UserList::iterator uit = users.begin(); uit != users.end(); ++uit) 
	{
		QTreeWidgetItem * lastInserted = new QTreeWidgetItem(queued);
		lastInserted->setText(0, StilUtils::TstrtoQ((StilUtils::getNicks(*uit) + _T(" - ") + StilUtils::getHubNames(*uit).first)) );
		datalist << *uit;
		datalist2 << QString();
		datalistitem << lastInserted;
			
		UploadManager::FileSet files = UploadManager::getInstance()->getWaitingUserFiles(*uit);
		for (UploadManager::FileSet::const_iterator fit = files.begin(); fit != files.end(); ++fit) 
		{
			QTreeWidgetItem * w = new QTreeWidgetItem();
			w->setText(0, StilUtils::TstrtoQ(Text::toT(*fit)) );
			lastInserted->addChild(w);
			datalist << *uit;
			datalist2 << StilUtils::TstrtoQ(Text::toT(*fit));
			datalistitem << w;
		}
	}
}

void WaitingUsersWindow::onAddFile(const UserPtr& aUser, const QString& aFile)
{
	int idx = datalist.indexOf(aUser);
	if ( idx >= 0 )
	{
		// found User-root item
		QTreeWidgetItem * w = datalistitem.at(idx);
		QTreeWidgetItem * w2 = w->parent();
		if (w2) w = w2;
		idx = datalistitem.indexOf(w);
		
		QTreeWidgetItem * n = new QTreeWidgetItem();
		n->setText(0, aFile);
		w->addChild(n);
		datalist << aUser;
		datalist2 << aFile;
		datalistitem << n;
	}
	else // if such aUser is new User
	{
		QTreeWidgetItem * n = new QTreeWidgetItem(queued);
		n->setText(0, StilUtils::TstrtoQ(StilUtils::getNicks(aUser) + _T(" - ") + StilUtils::getHubNames(aUser).first) );
		datalist << aUser;
		datalist2 << QString();
		datalistitem << n;
		QTreeWidgetItem * n2 = new QTreeWidgetItem();
		n->addChild(n2);
		n2->setText(0, aFile);
		datalist << aUser;
		datalist2 << aFile;
		datalistitem << n2;
	}
}

WaitingUsersWindow::~WaitingUsersWindow()
{
	datalist.clear();
	datalist2.clear();
	datalistitem.clear();
	queued->clear();
	UploadManager::getInstance()->removeListener(this);
	delete cnxtMenu;
}


void WaitingUsersWindow::makeContextMenu() 
{
	cnxtMenu->clear();
	cnxtMenu->addAction(/*QIcon(":/images/filelist.png"),*/ StilUtils::TstrtoQ(T_("&Get file list")) ,this ,SLOT(onGetList()) );
	cnxtMenu->addAction(/*QIcon(":/images/copy.png"),*/ StilUtils::TstrtoQ(T_("Copy Filename")) ,this ,SLOT(onCopyFilename()) );
	cnxtMenu->addAction(/*QIcon(":/images/remove.png"),*/ StilUtils::TstrtoQ(T_("&Remove")) ,this ,SLOT(onRemove()) );
	cnxtMenu->addAction(/*QIcon(":/images/grantslot.png"),*/ StilUtils::TstrtoQ(T_("Grant &extra slot")) ,this ,SLOT(onGrantSlot()) );
	cnxtMenu->addAction(/*QIcon(":/images/add_fav.png"),*/ StilUtils::TstrtoQ(T_("Add To &Favorites")) ,this ,SLOT(onAddToFavorites()) );
	cnxtMenu->addAction(/*QIcon(":/images/pm.png"),*/ StilUtils::TstrtoQ(T_("&Send private message")) ,this ,SLOT(onPrivateMessage()) );
	//cnxtMenu->setDefaultAction(t);
}

UserPtr WaitingUsersWindow::getSelectedUser()
{
	int idx = datalistitem.indexOf(queued->currentItem());
	return (idx > -1 ? datalist.at(idx) : NULL);
}

void WaitingUsersWindow::showCnxtMenu(const QPoint& point)
{
	if (queued->indexAt(point).isValid())
	{
		makeContextMenu();
		cnxtMenu->exec(mapToGlobal(point));
	}
}

void WaitingUsersWindow::onKeyPress(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Delete) onRemove();
}

void WaitingUsersWindow::slotSpeak(int a, UserPtr b, QString c) 
{
	if(a == SPEAK_ADD_FILE)
	{
		onAddFile(b, c);
		//setDirty(SettingsManager::BOLD_WAITING_USERS);
	}
	
	if(a == SPEAK_REMOVE_USER) 
	{
		onRemoveUser(b);
		//setDirty(SettingsManager::BOLD_WAITING_USERS);
	}
}

void WaitingUsersWindow::onRemoveUser(const UserPtr& aUser)
{
	int idx = datalist.indexOf(aUser);
	if (idx < 0) return;
	QTreeWidgetItem * w = datalistitem.at(idx);
	datalist.removeAt(idx);
	datalist2.removeAt(idx);
	datalistitem.removeAt(idx);
	delete w;
}

// UploadManagerListener
void WaitingUsersWindow::on(UploadManagerListener::WaitingRemoveUser, const UserPtr& aUser) throw()
{
	speak(SPEAK_REMOVE_USER, aUser, QString());
}

void WaitingUsersWindow::on(UploadManagerListener::WaitingAddFile, const UserPtr& aUser, const string& aFilename) throw()
{
	speak(SPEAK_ADD_FILE, aUser, StilUtils::TstrtoQ(Text::toT(aFilename)));
}

// Context slots
void WaitingUsersWindow::onPrivateMessage()
{
	UserPtr user = getSelectedUser();
	if (user)
		MainWindowImpl::getInstance()->OpenPM(user);
}

void WaitingUsersWindow::onGrantSlot()
{
	UserPtr user = getSelectedUser();
	if (user)
		UploadManager::getInstance()->reserveSlot(user);
}

void WaitingUsersWindow::onAddToFavorites()
{
	UserPtr user = getSelectedUser();
	if (user)
		FavoriteManager::getInstance()->addFavoriteUser(user);
}
void WaitingUsersWindow::onGetList()
{
	UserPtr user = getSelectedUser();
	if (user)
		QueueManager::getInstance()->addList(user, QueueItem::FLAG_CLIENT_VIEW);
}

void WaitingUsersWindow::onCopyFilename()
{
	QTreeWidgetItem * selectedItem = queued->currentItem();
	QTreeWidgetItem * parentItem =  (selectedItem == NULL ? NULL : selectedItem->parent() );

	if (!selectedItem || !parentItem || selectedItem == parentItem) return;
	
	tstring txt = StilUtils::QtoTstr(selectedItem->text(0));
	tstring::size_type i = txt.find(_T('('));
	if(i != tstring::npos && i > 0) txt.erase(i - 1);
	if(!txt.empty()) StilUtils::copy2Clipboard(txt);
}

void WaitingUsersWindow::onRemove()
{
	UserPtr user = getSelectedUser();
	if (user)
		UploadManager::getInstance()->clearUserFiles(user);
}
