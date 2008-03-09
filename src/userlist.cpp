/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
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

#include "userlist.h"
#include "mainwindowimpl.h"
//

HubUserList::~HubUserList()
{
	//saveSettings();
	
	if (treeView) treeView = 0;
	if (filterString) filterString = 0;
	if (criteriaSortBox) criteriaSortBox = 0;
}

HubUserList::HubUserList(QTreeView *&_treeView, QLineEdit *&_filterString, 
							QComboBox *&_criteriaSortBox, const QStringList &columns) 
	:treeView(_treeView), filterString(_filterString), criteriaSortBox(_criteriaSortBox), totalColumns(columns.size())

{

	model = new QStandardItemModel(0, totalColumns);

	for (int i=0;i<totalColumns;i++)
		model->setHeaderData(i, Qt::Horizontal, columns.at(i));

	
	QObject::connect(filterString, SIGNAL(textChanged(const QString &)),
			this, SLOT(filterChanged()));
	QObject::connect(criteriaSortBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(filterColumnChanged()));
			
	columnMenu = new QMenu(treeView->header());
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	treeView->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeView->header(), SIGNAL(customContextMenuRequested(const QPoint&)), 
				this, SLOT(showColumnMenu(const QPoint&)));
	
	initUserMenu();
	
	checkAndReadSettings();
	
	proxyModel = new UserListSortingModel();
	proxyModel -> setSortingSettings(classicSort, topOp);
	
	proxyModel -> setDynamicSortFilter(true);
	treeView   -> setSelectionMode(QAbstractItemView::ExtendedSelection);
	treeView   -> setEditTriggers(QAbstractItemView::NoEditTriggers);
	proxyModel -> setSourceModel(model);
	treeView   -> setModel(proxyModel);
	//proxyModel -> setSortCaseSensitivity(Qt::CaseInsensitive);
	treeView -> sortByColumn(0, Qt::AscendingOrder);
	proxyModel->setFilterKeyColumn(-1);
	
	//Hide two last columns
	treeView   -> hideColumn(totalColumns - 1);
	treeView   -> hideColumn(totalColumns - 2);
	
}

void HubUserList::filterChanged()
{
	//TODO: make user choice for other filtering modes:  QRegExp::Wildcard and QRegExp::RegExp
	QRegExp regExp(filterString->text(), Qt::CaseInsensitive, QRegExp::FixedString);
	proxyModel->setFilterRegExp(regExp);
}

void HubUserList::filterColumnChanged()
{
	proxyModel->setFilterKeyColumn(criteriaSortBox->currentIndex() - 1);
}

void HubUserList::addUser(const QStringList &list, QIcon *&icon)
{
	QStandardItem *parentItem = model->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStringList::const_iterator constIterator;
	
	int i = 0;
	QStandardItem *item;
    
    for (i = 0, constIterator = list.constBegin(); constIterator != list.constEnd(); 
    		++constIterator, i++)
    {
    	item = new QStandardItem((*constIterator));
    	
    	if (i == 0) item->setIcon(*icon);
    		
    	itemList << item;
    }
    parentItem->appendRow(itemList);
}

int HubUserList::findUser(const QString& str)
{
	for (int i=0; i<model->rowCount(); i++)
	{
		QString userName = model->data(model->index(i,0)).toString();
		if (str == userName) return i;
	}
	
	return -1;
}

void HubUserList::removeUser(const QString& str)
{
	int row = findUser(str);
	if (row != -1) model->removeRow(row);
}

void HubUserList::clearList()
{
	model->removeRows(0, model->rowCount());
}

void HubUserList::updateUser(int pos, const QStringList& strList, QIcon *&icon)
{	
	QList<QStandardItem *> itemList;
	QStandardItem *item;
	
	//TODO: don't count last hidden row 
	for (int i=0; i<model->columnCount(); i++)
	{
		item = new QStandardItem(strList.at(i));
		if (i == 0) item->setIcon(*icon);
		itemList << item;
	}
	
	model->takeRow(pos);
	model->insertRow(pos, itemList);
}

void HubUserList::checkAndReadSettings()
{
	int read_topOp = APPSETTING(i_SORTUSERLISTOPSFIRST);
	int read_classicSort = APPSETTING(i_SORTUSERLISTCLASSIC);
	
	if //((classicSort==0 && topOp==2) || (classicSort==1 && topOp==1) || 
		(read_classicSort>1 || read_classicSort<0 || read_topOp>1 || read_topOp<0)//)
	{
		classicSort = true;
		topOp = true;
		
		saveSettings();
	}
	else
	{
		classicSort = static_cast<bool>(read_classicSort);
		topOp = static_cast<bool>(read_topOp);
	}
}

void HubUserList::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0)
		treeView->hideColumn(-index-1);
	else
	{
		treeView->showColumn(index);
		//treeView->resizeColumnToContents(index);
		//treeView->resizeColumnToContents(proxyModel->columnCount());
	}
}

void HubUserList::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QAction *action_classicSort = columnMenu->addAction(tr("Classic sort"), this, SLOT(set_classicSort()));
	action_classicSort->setCheckable(true);
	
	QAction *action_topOp = columnMenu->addAction(tr("Op on top"), this, SLOT(set_topOp()));
	action_topOp->setCheckable(true);
	
	action_classicSort->setChecked(classicSort);
	action_topOp->setChecked(topOp);
	action_topOp->setEnabled(!classicSort);
	
	columnMenu->addSeparator();
	
	for(int i = 0; i < proxyModel->columnCount()-2; i++)
	{
		QAction *action = new QAction(columnMenu);
		action->setText(proxyModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
		
		bool isHidden = treeView->header()->isSectionHidden(i);

		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
			
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
		
	}
	
	columnMenu->exec(treeView->header()->mapToGlobal(point));	
}

void HubUserList::set_topOp()
{
	QAction *action = columnMenu->actions().at(1);
	
	bool checked = action->isChecked();
	
	if (!checked)
		topOp = false;
	else
		topOp = true;
	
	saveSettings();
	proxyModel -> setSortingSettings(classicSort, topOp);
	resort();
}

void HubUserList::set_classicSort()
{
	QAction *action = columnMenu->actions().at(0);
	
	bool checked = action->isChecked();
	
	if (!checked)
		classicSort = false;
	else
		classicSort = true;
	
	saveSettings();
	proxyModel -> setSortingSettings(classicSort, topOp);
	resort();
}

void HubUserList::resort()
{
	proxyModel->clear();
}

void HubUserList::saveSettings()
{
	SETAPPSETTING(i_SORTUSERLISTOPSFIRST, static_cast<int>(topOp));
	SETAPPSETTING(i_SORTUSERLISTCLASSIC, static_cast<int>(classicSort));
}

void HubUserList::initUserMenu()
{
	userMenu = new QMenu(treeView);
	treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeView, SIGNAL(customContextMenuRequested(const QPoint&)), 
				this, SLOT(showUserMenu(const QPoint&)));

	//connect(treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	//		this, SLOT(updateActions(const QItemSelection &, const QItemSelection &)));
	
	userMenu->addAction(tr("Get file list"), this, SLOT(actionGetFilelist()));
	userMenu->addAction(tr("Match queue"), this, SLOT(actionMatchQueue()));
	userMenu->addAction(tr("Send private message"), this, SLOT(actionSendPM()));
	userMenu->addAction(tr("Add to favorites"), this, SLOT(actionAddToFavorites()));
	userMenu->addAction(tr("Grand extra slot"), this, SLOT(actionGrandExtraSlot()));

	userMenu->addSeparator();
	
	userMenu->addAction(tr("Remove user from queue"), this, SLOT(actionRemoveUserFromQueue()));
	userMenu->addAction(tr("Copy nick to clipboard"), this, SLOT(actionCopyNickToClipboard()));
}

void HubUserList::showUserMenu(const QPoint &point)
{
    if (treeView->indexAt(point).isValid())
        userMenu->exec(treeView->mapToGlobal(point));
}

void HubUserList::actionGetFilelist()
{	
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{    

		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			try
			{
				dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
				if (user)
					dcpp::QueueManager::getInstance()->addList(user, dcpp::QueueItem::FLAG_CLIENT_VIEW);
			}
			catch (const dcpp::Exception &e)
			{
				dcpp::LogManager::getInstance()->message(e.getError());
			}
		}
	}
}

void HubUserList::actionMatchQueue()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{    

		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			try
			{
				dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
				if (user)
					dcpp::QueueManager::getInstance()->addList(user, dcpp::QueueItem::FLAG_MATCH_QUEUE);
			}
			catch (const dcpp::Exception &e)
			{
				dcpp::LogManager::getInstance()->message(e.getError());
			}
		}
	}
	
}

void HubUserList::actionSendPM()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{    

		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
			
			if (user)
				//MainWindowImpl::getInstance()->OpenPM(proxyModel->sibling(index->row(), 0, *index).data().toString());
				MainWindowImpl::getInstance()->OpenPM(user);
		}
	}
}

void HubUserList::actionAddToFavorites()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{    

		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
			
			if (user)
				dcpp::FavoriteManager::getInstance()->addFavoriteUser(user);
		}
	}
}

void HubUserList::actionGrandExtraSlot()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{    

		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
			
			if (user)
				dcpp::UploadManager::getInstance()->reserveSlot(user);
		}
	}
}

void HubUserList::actionRemoveUserFromQueue()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
	{
		QString userCID = proxyModel->sibling(index->row(), 7, *index).data().toString();
	
		std::string cid = userCID.toStdString();
	
		if (!cid.empty())
		{
			dcpp::UserPtr user = dcpp::ClientManager::getInstance()->findUser(dcpp::CID(cid));
			
			if (user)
				dcpp::QueueManager::getInstance()->removeSource(user, dcpp::QueueItem::Source::FLAG_REMOVED);
		}
	}
	
}

void HubUserList::actionCopyNickToClipboard()
{
	const QModelIndexList indexes = treeView->selectionModel()->selectedRows();
	
	QString nicks;
	
	for (QModelIndexList::const_iterator index = indexes.constBegin(); index != indexes.constEnd(); ++index)
		nicks += proxyModel->sibling(index->row(), 0, *index).data().toString() + ' ';
	
	//Remove last space
	nicks.chop(1);
	
	StilUtils::copy2Clipboard(nicks);
}

/*
void HubUserList::updateActions(const QItemSelection &, const QItemSelection &)
{
	qDebug() << "Update actions";
}
*/
