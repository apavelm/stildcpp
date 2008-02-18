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

#include "favHubLstWin.h"

void FavoriteHubListWindow::listkeyPressEvent(QKeyEvent *e)
{
	if ( e->key() == Qt::Key_Delete) slot_Remove();
}

void FavoriteHubListWindow::slot_list_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	// Changing information in right panel
	int idx = datalistitem.indexOf(list->indexFromItem(c));
	FavoriteHubEntryPtr entry = datalist.at(idx);//GetFavHubPtr(c->text(COLUMN_NAME));
	if (!entry) return;
	
	edt_hubName->setText( StilUtils::TstrtoQ(Text::toT(entry->getName())) );
	edt_hubDesc->setPlainText( StilUtils::TstrtoQ(Text::toT(entry->getDescription())) );
	edt_hubAddress->setText( StilUtils::TstrtoQ(Text::toT(entry->getServer())) );
	cb_autoConnect->setChecked(entry->getConnect());
	
	edt_Login->setText( StilUtils::TstrtoQ(Text::toT(entry->getNick(false))) );
	edt_Pass->setText( StilUtils::TstrtoQ(Text::toT(entry->getPassword())) );
	edt_Desc->setText( StilUtils::TstrtoQ(Text::toT(entry->getUserDescription())) );
	
	//
	currentLabel->setText("<h1>" + c->text(COLUMN_NAME) + "</h1>");
}


void FavoriteHubListWindow::slot_autoconn(int value)
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setConnect(static_cast<bool>(value));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_1()
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setName(Text::fromT(StilUtils::QtoTstr(edt_hubName->text())));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_2()
{
	tstring addressText = StilUtils::QtoTstr(edt_hubAddress->text());
	if(addressText.empty()) 
	{
		QMessageBox::information(this, tr("StilDC++"), tr("Hub address cannot be empty"), QMessageBox::Ok);
		return;
	}
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setServer(Text::fromT(addressText));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_3()
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setDescription(Text::fromT(StilUtils::QtoTstr(edt_hubDesc->toPlainText())));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_4()
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setNick(Text::fromT(StilUtils::QtoTstr(edt_Login->text())));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_5()
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setPassword(Text::fromT(StilUtils::QtoTstr(edt_Pass->text())));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotStore_6()
{
	int idx = datalistitem.indexOf(list->currentIndex());
	if (idx < 0) return;
	datalist[idx]->setUserDescription(Text::fromT(StilUtils::QtoTstr(edt_Desc->text())));
	FavoriteManager::getInstance()->save();
}

void FavoriteHubListWindow::slotKey_1(QKeyEvent *e)
{
	if ( ( e->key() == Qt::Key_Enter) || ( e->key() == Qt::Key_Return) ) 
		slotStore_1();
}

void FavoriteHubListWindow::slotKey_2(QKeyEvent *e)
{
	if ( ( e->key() == Qt::Key_Enter) || ( e->key() == Qt::Key_Return) )
		slotStore_2();
}

void FavoriteHubListWindow::slotKey_4(QKeyEvent *e)
{
	if ( ( e->key() == Qt::Key_Enter) || ( e->key() == Qt::Key_Return) )
		slotStore_4();
}

void FavoriteHubListWindow::slotKey_5(QKeyEvent *e)
{
	if ( ( e->key() == Qt::Key_Enter) || ( e->key() == Qt::Key_Return) )
		slotStore_5();
}

void FavoriteHubListWindow::slotKey_6(QKeyEvent *e)
{
	if ( ( e->key() == Qt::Key_Enter) || ( e->key() == Qt::Key_Return) )
		slotStore_6();
}

FavoriteHubListWindow::FavoriteHubListWindow(QWidget *parent) : MdiChild(parent), startup(true)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_FAVORITE_HUB_LIST;
	idText  = "";
	setTabText(tr("Favorite Hubs"));
	
	list->setAlternatingRowColors(true);
	list->sortByColumn(0,Qt::AscendingOrder);
	list->clear();
	
	connect(btn_hName_store, SIGNAL(clicked()), this, SLOT(slotStore_1()) );
	connect(btn_hAddr_store, SIGNAL(clicked()), this, SLOT(slotStore_2()) );
	connect(btn_hDesc_store, SIGNAL(clicked()), this, SLOT(slotStore_3()) );
	connect(btn_uName_store, SIGNAL(clicked()), this, SLOT(slotStore_4()) );
	connect(btn_uPass_store, SIGNAL(clicked()), this, SLOT(slotStore_5()) );
	connect(btn_uDesc_store, SIGNAL(clicked()), this, SLOT(slotStore_6()) );
	
	connect(edt_hubName, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(slotKey_1(QKeyEvent *)) );
	connect(edt_hubAddress, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(slotKey_2(QKeyEvent *)) );
	connect(edt_Login, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(slotKey_4(QKeyEvent *)) );
	connect(edt_Pass, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(slotKey_5(QKeyEvent *)) );
	connect(edt_Desc, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(slotKey_6(QKeyEvent *)) );
	
	connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(slot_list_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)) );
	connect(list, SIGNAL(sig_KeyPressEvent(QKeyEvent *)), this, SLOT(listkeyPressEvent(QKeyEvent *)) );
	connect(btn_Add, SIGNAL(clicked()), this, SLOT(slot_Add()) );
	connect(btn_Connect, SIGNAL(clicked()), this, SLOT(slot_Connect()) );
	connect(btn_Delete, SIGNAL(clicked()), this, SLOT(slot_Remove()) );
	connect(cb_autoConnect, SIGNAL(stateChanged(int)), this, SLOT(slot_autoconn(int)) );
	
	datalist.clear();
	datalistitem.clear();
	const FavoriteHubEntryList& fl = FavoriteManager::getInstance()->getFavoriteHubs();
	for(FavoriteHubEntryList::const_iterator i = fl.begin(); i != fl.end(); ++i)
		addEntry(*i);
	
	FavoriteManager::getInstance()->addListener(this);
	list->setCurrentItem(list->topLevelItem(0));
}

FavoriteHubListWindow::~FavoriteHubListWindow()
{
	datalist.clear();
	datalistitem.clear();
	list->clear();
	FavoriteManager::getInstance()->removeListener(this);
}

void FavoriteHubListWindow::addEntry(const FavoriteHubEntryPtr entry)
{
	datalist << entry;
	setUpdatesEnabled(false);
	QTreeWidgetItem *it = new QTreeWidgetItem(list);
	datalistitem << list->indexFromItem(it);
	it->setText(0, StilUtils::TstrtoQ(Text::toT(entry->getName())));
	it->setIcon(0,QIcon(":/images/hub.png"));
	setUpdatesEnabled(true);
}

void FavoriteHubListWindow::slot_Add()
{
	bool ok, ok2;
	QString text = QInputDialog::getText(this, "StilDC++" ,tr("Input name of new hub:"), QLineEdit::Normal, QString(), &ok);
	if (!(ok && !text.isEmpty())) return;
	QString text2 = QInputDialog::getText(this, "StilDC++" ,tr("Input address of new hub:"), QLineEdit::Normal, QString(), &ok2);
	if (!(ok2 && !text2.isEmpty()))
	{
		QMessageBox::information(this, tr("StilDC++"), tr("Hub address cannot be empty"), QMessageBox::Ok);
		return;
	}
	startup = false;
	FavoriteHubEntry f;
	f.setName(Text::fromT(StilUtils::QtoTstr(text)));
	f.setServer(Text::fromT(StilUtils::QtoTstr(text2)));
	FavoriteManager::getInstance()->addFavorite(f);
}

void FavoriteHubListWindow::slot_Connect()
{
	QTreeWidgetItem *it = list->currentItem();
	if (!it) return;

	if(SETTING(NICK).empty())
	{
		QMessageBox::critical(this, tr("StilDC++"), tr("Please enter a nickname in the settings dialog!"), QMessageBox::Ok);
		return;
	}

//	std::vector<unsigned> items = hubs->getSelected();
//	for(std::vector<unsigned>::iterator i = items.begin(); i != items.end(); ++i) {
//		FavoriteHubEntryPtr entry = reinterpret_cast<FavoriteHubEntryPtr>(hubs->getData(*i));
//		HubFrame::openWindow(getParent(), entry->getServer());
//	}
}

void FavoriteHubListWindow::slot_Remove()
{
	QList<QTreeWidgetItem *> lt = list->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		int idx = datalistitem.indexOf(list->indexFromItem(lt[i]));
		bool b;
		if (!BOOLSETTING(CONFIRM_HUB_REMOVAL) ) b = true;
		else
		{
			list->setCurrentItem(lt[i]);
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, tr("StilDC++"), tr("Do you realy want to delete hub: \"") + StilUtils::TstrtoQ(Text::toT(datalist[idx]->getName())) + "\"?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) b = true; else b = false;
		}
	
		if (b) FavoriteManager::getInstance()->removeFavorite(datalist[idx]);
		
		// After deleting next in list ModelIndexes changed!!!
		// It need to fix ModelIndexes 
		// Qt BUG ???
		for (int j = 0; j < datalistitem.size(); j++)
		{
			QTreeWidgetItem *w = list->itemFromIndex(datalistitem[j]);
			datalistitem[j] = list->indexFromItem(w);
		}
			list->setCurrentItem(list->topLevelItem(idx));
		////////////
	}
}

void FavoriteHubListWindow::on(FavoriteAdded, const FavoriteHubEntryPtr e) throw() 
{
	addEntry(e);
	if (!startup)
	{
		int idx = datalist.indexOf(e);
		list->setCurrentIndex(datalistitem[idx]);
		edt_hubAddress->setFocus(Qt::OtherFocusReason);
		startup = true;
	}
}

void FavoriteHubListWindow::on(FavoriteRemoved, const FavoriteHubEntryPtr e) throw() 
{
	int idx = datalist.indexOf(e);
	QTreeWidgetItem *w = list->itemFromIndex(datalistitem[idx]);
	// DELETING 
	setUpdatesEnabled(false);
	delete w;
	datalistitem.removeAt(idx);
	datalist.removeAt(idx);
	setUpdatesEnabled(true);
	// END OF DELETING
}
