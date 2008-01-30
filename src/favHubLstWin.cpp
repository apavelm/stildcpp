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

#include "favHubLstWin.h"

dcpp::FavoriteHubEntryPtr FavoriteHubListWindow::GetFavHubPtr(const QString & name)
{
	QList<FavoriteHubEntryPtr>::iterator i;
	for (i = datalist.begin(); i != datalist.end(); ++i)
		if ( StilUtils::TstrtoQ(Text::toT((*i)->getName())) == name) return *i;
	return NULL;
}

int FavoriteHubListWindow::GetFavHubIndex(const QString & name)
{
	if (datalist.isEmpty()) return -1;
	for (int i = 0; i < datalist.size(); i++ )
		if ( StilUtils::TstrtoQ(Text::toT(datalist[i]->getName())) == name) return i;
	return -1;
}

void FavoriteHubListWindow::on_list_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	// Changing information in right panel
	FavoriteHubEntryPtr entry = GetFavHubPtr(c->text(COLUMN_NAME));
	if (!entry) return;
	
	edt_hubName->setText( StilUtils::TstrtoQ(Text::toT(entry->getName())) );
	edt_hubDesc->setText( StilUtils::TstrtoQ(Text::toT(entry->getDescription())) );
	edt_hubAddress->setText( StilUtils::TstrtoQ(Text::toT(entry->getServer())) );
	cb_autoConnect->setChecked(entry->getConnect());
	
	edt_Login->setText( StilUtils::TstrtoQ(Text::toT(entry->getNick(false))) );
	edt_Pass->setText( StilUtils::TstrtoQ(Text::toT(entry->getPassword())) );
	edt_Desc->setText( StilUtils::TstrtoQ(Text::toT(entry->getUserDescription())) );
	
	//l.push_back(tstring(entry->getPassword().size(), '*'));
	
	//
	currentLabel->setText("<h1>" + c->text(COLUMN_NAME) + "</h1>");
}

FavoriteHubListWindow::FavoriteHubListWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_FAVORITE_HUB_LIST;
	idText  = "";
	setTabText(tr("Favorite Hubs"));
	
	connect(btn_Add, SIGNAL(clicked()), this, SLOT(slot_Add()) );
	connect(btn_Connect, SIGNAL(clicked()), this, SLOT(slot_Connect()) );
	connect(btn_Delete, SIGNAL(clicked()), this, SLOT(slot_Remove()) );
	
	datalist.clear();
	const FavoriteHubEntryList& fl = FavoriteManager::getInstance()->getFavoriteHubs();
	for(FavoriteHubEntryList::const_iterator i = fl.begin(); i != fl.end(); ++i)
		addEntry(*i);
	
	FavoriteManager::getInstance()->addListener(this);
	
	list->setAlternatingRowColors(true);
	list->sortByColumn(0,Qt::AscendingOrder);
	list->setCurrentItem(list->topLevelItem(0));
}

FavoriteHubListWindow::~FavoriteHubListWindow()
{
	FavoriteManager::getInstance()->removeListener(this);
	datalist.clear();
}

void FavoriteHubListWindow::addEntry(const dcpp::FavoriteHubEntryPtr entry, int index)
{
	datalist << entry;
	QTreeWidgetItem *it = new QTreeWidgetItem(list);
	it->setText(0, StilUtils::TstrtoQ(Text::toT(entry->getName())));
	it->setIcon(0,QIcon(":/images/icon_error.png"));
}

void FavoriteHubListWindow::rebuilddatalist()
{
	datalist.clear();
	const FavoriteHubEntryList& fl = FavoriteManager::getInstance()->getFavoriteHubs();
	for(FavoriteHubEntryList::const_iterator i = fl.begin(); i != fl.end(); ++i)
		datalist << *i;
}

void FavoriteHubListWindow::slot_Add()
{
	
}

void FavoriteHubListWindow::slot_Connect()
{
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;

	if(SETTING(NICK).empty())
	{
//		createMessageBox().show(TSTRING(ENTER_NICK), _T(APPNAME) _T(" ") _T(VERSIONSTRING), WidgetMessageBox::BOX_OK, WidgetMessageBox::BOX_ICONSTOP);
//		return;
	}

//	std::vector<unsigned> items = hubs->getSelected();
//	for(std::vector<unsigned>::iterator i = items.begin(); i != items.end(); ++i) {
//		FavoriteHubEntryPtr entry = reinterpret_cast<FavoriteHubEntryPtr>(hubs->getData(*i));
//		HubFrame::openWindow(getParent(), entry->getServer());
//	}
}

void FavoriteHubListWindow::slot_Remove()
{
	bool b = true;
	QTreeWidgetItem *it = list->currentItem();
	if (it)
	{
		if (!BOOLSETTING(CONFIRM_HUB_REMOVAL) ) b = true;
		else
		{
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr("StilDC++"), tr("Do you realy want to delete?"), QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) b = true; else b = false;
		}
	
		if (b) FavoriteManager::getInstance()->removeFavorite(reinterpret_cast<FavoriteHubEntryPtr>(GetFavHubPtr(it->text(0))));
	}
}

void FavoriteHubListWindow::on(FavoriteAdded, const dcpp::FavoriteHubEntryPtr e) throw() 
{
	addEntry(e);
}

void FavoriteHubListWindow::on(FavoriteRemoved, const dcpp::FavoriteHubEntryPtr e) throw() 
{
	//->erase(hubs->findData(reinterpret_cast<LPARAM>(e)));
	QTreeWidgetItem *it = list->currentItem();
	if (it) delete it;
}
