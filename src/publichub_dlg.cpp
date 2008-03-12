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

#include "publichub_dlg.h"


ConfPubHubDlg::ConfPubHubDlg(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(btn_ok, SIGNAL(clicked()), this, SLOT(slotOK()));
	connect(btn_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	setWindowTitle(StilUtils::TstrtoQ(T_("Configured Public Hub Lists")));
	
	connect(hubLists, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotEdit2(QListWidgetItem*)) );
	connect(hubLists, SIGNAL(sig_KeyPressEvent(QKeyEvent*)), this, SLOT(slotKeyPress(QKeyEvent*)) );
	connect(editBox, SIGNAL(returnPressed()), this, SLOT(slotAdd()) );
	
	btn_add->setText(StilUtils::TstrtoQ(T_("&Add")));
	connect(btn_add, SIGNAL(clicked()), this, SLOT(slotAdd()) );
	btn_edit->setText(StilUtils::TstrtoQ(T_("&Edit")));
	connect(btn_edit, SIGNAL(clicked()), this, SLOT(slotEdit()) );
	btn_delete->setText(StilUtils::TstrtoQ(T_("&Remove")));
	connect(btn_delete, SIGNAL(clicked()), this, SLOT(slotDelete()) );
	
	StringList lists(FavoriteManager::getInstance()->getHubLists());
	for(StringIterC idx = lists.begin(); idx != lists.end(); ++idx)
		addHubList(Text::toT(*idx));
	
	editBox->setFocus();
	
	show();
}

void ConfPubHubDlg::addHubList(const tstring& address)
{
	hubLists->addItem(StilUtils::TstrtoQ(address));
}

void ConfPubHubDlg::slotOK()
{
	tstring tmp;
	int j = hubLists->count();
	for(int i = 0; i < j; ++i)
	{
		if(i != 0)
			tmp += ';';
		tmp += StilUtils::QtoTstr(hubLists->item(i)->text());
	}
	SettingsManager::getInstance()->set(SettingsManager::HUBLIST_SERVERS, Text::fromT(tmp));
	accept();
}

void ConfPubHubDlg::slotCancel()
{
	reject();
}

void ConfPubHubDlg::slotAdd()
{
	StringTokenizer<tstring> t(StilUtils::QtoTstr(editBox->text()), ';');
	for(TStringList::reverse_iterator i = t.getTokens().rbegin(); i != t.getTokens().rend(); ++i)
		if(!i->empty())
			addHubList(*i);
	editBox->clear();
}

void ConfPubHubDlg::slotEdit()
{
	if (hubLists->currentItem())
		slotEdit2(hubLists->currentItem());
}

void ConfPubHubDlg::slotEdit2(QListWidgetItem* item)
{
	bool ok;
	QString it = item->text();
	QString text = QInputDialog::getText(this, "Hublist" ,tr("Edit the hublist"), QLineEdit::Normal, it, &ok);
	if (ok)
		item->setText(text);
}

void ConfPubHubDlg::slotDelete()
{
	if (hubLists->currentItem()) delete hubLists->currentItem();
}
	
void ConfPubHubDlg::slotKeyPress(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Insert) 
	{
		if (!editBox->text().isEmpty()) slotAdd();
	} 
	else
	if (e->key() == Qt::Key_Delete) 
	{
		if (hubLists->currentItem()) slotDelete();
	} 
	else
	if ( (e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return) )
	{
		slotEdit();
	} 
}
