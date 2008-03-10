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


#include "adlsearch.h"

int ADLSearchWindow::columnSizes[] = { 160, 90, 120, 90, 90 };
static const char* columnNames[] = {
	N_("Enabled / Search String"),
	N_("Source Type"),
	N_("Destination Directory"),
	N_("Min Size"),
	N_("Max Size")
};

ADLSearchWindow::ADLSearchWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_ADL_SEARCH;
	setTabText(tr("Automatic Directory Listing Search"));
	connect(btn_add, SIGNAL(clicked()), this, SLOT(slotAdd()) );
	connect(btn_remove, SIGNAL(clicked()), this, SLOT(slotRemove()) );
	connect(btn_properties, SIGNAL(clicked()), this, SLOT(slotProp()) );
	
	datalist.clear();
	datalistitem.clear();
	
	// labels
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	list->setHeaderLabels(columns);
	// width
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(ADLSEARCHFRAME_WIDTHS))).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) list->setColumnWidth(i, columnSizes[i]);
	else 
		for (int i=0; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) list->setColumnWidth(i, columnSizes[i]);
				else list->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	QStringList vlist = APPSTRING(s_ADLSEARCH_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) list->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(ADLSEARCHFRAME_ORDER))).split(",");
	if (olist.size() == COLUMN_LAST)
		for (int j=0; j<COLUMN_LAST; j++) list->header()->swapSections(list->header()->visualIndex(olist[j].toInt()), j);
		
	// COLUMN (HEADER) MENUS
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	list->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(list->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	// CONTEXT MENU
	cnxtMenu = new QMenu();
	list->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(list, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	// Load all searches
	ADLSearchManager::SearchCollection& collection = ADLSearchManager::getInstance()->collection;
	for(ADLSearchManager::SearchCollection::iterator i = collection.begin(); i != collection.end(); ++i)
		addEntry(*i);
	
	list->setCurrentItem(list->topLevelItem(0));
}

ADLSearchWindow::~ADLSearchWindow()
{
	ADLSearchManager::getInstance()->Save();

	// visibility
	QStringList vv;
	for (int i=0; i<COLUMN_LAST; i++) 
		vv << QString::number(list->header()->isSectionHidden(i));
	SETAPPSTRING(s_ADLSEARCH_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) 
		w << QString::number(list->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::ADLSEARCHFRAME_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i=0; i<COLUMN_LAST; i++) 
		ww << QString::number(list->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::ADLSEARCHFRAME_ORDER, ww.join(",").toStdString());
	
	list->clear();
	
	datalist.clear();
	datalistitem.clear();
	delete cnxtMenu;
	delete columnMenu;
}

void ADLSearchWindow::slotAdd()
{
	ADLSearch * _search = new ADLSearch();
	ADLPropertiesDialog * dlg = new ADLPropertiesDialog(this, _search);
	int rslt = dlg->exec();
	if (rslt == QDialog::Accepted)
	{
		ADLSearchManager::SearchCollection& collection = ADLSearchManager::getInstance()->collection;
		int index;
		// Add new search to the end or if selected, just before
		
		if (list->currentItem())
			index = datalistitem.indexOf(list->currentItem());
		else index = -1;
		
		if (datalistitem.count() > 0)
			collection.insert(collection.begin() + index, *_search);
		else 
			collection.push_back(*_search);

		addEntry(*_search, index);
	}
	else delete _search;
}

void ADLSearchWindow::slotProp()
{
	if (!list->currentItem()) return;
	int dx = datalistitem.indexOf(list->currentItem());
	if (dx < 0) return;
	ADLSearch * search = datalist.at(dx);
	ADLPropertiesDialog * h = new ADLPropertiesDialog(this, search);
	int rslt = h->exec();
	if (rslt == QDialog::Accepted)
	{
		QStringList l;
		l << StilUtils::TstrtoQ(Text::toT(search->searchString));
		l << StilUtils::TstrtoQ(Text::toT(search->SourceTypeToString(search->sourceType)));
		l << StilUtils::TstrtoQ(Text::toT(search->destDir));
		l << StilUtils::TstrtoQ((search->minFileSize >= 0) ? Text::toT(Util::toString(search->minFileSize)) + _T(" ") + Text::toT(search->SizeTypeToString(search->typeFileSize)) : Util::emptyStringT);
		l << StilUtils::TstrtoQ((search->maxFileSize >= 0) ? Text::toT(Util::toString(search->maxFileSize)) + _T(" ") + Text::toT(search->SizeTypeToString(search->typeFileSize)) : Util::emptyStringT);
		
		for (int i = 0; i < COLUMN_LAST; i++)
			datalistitem.at(dx)->setText(i, l.at(i));
			
		datalistitem.at(dx)->setCheckState(0, ( search->isActive ? Qt::Checked : Qt::Unchecked) );
	}
}

void ADLSearchWindow::slotRemove()
{
	ADLSearchManager::SearchCollection& collection = ADLSearchManager::getInstance()->collection;
	
	QList<QTreeWidgetItem *> lt = list->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++) 
	{
		int dx = datalistitem.indexOf(lt.at(i));
		collection.erase(collection.begin() + dx);
		delete datalist.at(dx);
		delete datalistitem.at(dx);
		datalist.removeAt(dx);
		datalistitem.removeAt(dx);
		if (dx < datalistitem.count())
			list->setCurrentItem(datalistitem.at(dx));
		else 
			if (datalistitem.count() > 0) list->setCurrentItem(datalistitem.at(datalistitem.count()-1));
	}
}

void ADLSearchWindow::addEntry(ADLSearch& search, int index)
{
	QStringList l;
	l << StilUtils::TstrtoQ(Text::toT(search.searchString));
	l << StilUtils::TstrtoQ(Text::toT(search.SourceTypeToString(search.sourceType)));
	l << StilUtils::TstrtoQ(Text::toT(search.destDir));
	l << StilUtils::TstrtoQ((search.minFileSize >= 0) ? Text::toT(Util::toString(search.minFileSize)) + _T(" ") + Text::toT(search.SizeTypeToString(search.typeFileSize)) : Util::emptyStringT);
	l << StilUtils::TstrtoQ((search.maxFileSize >= 0) ? Text::toT(Util::toString(search.maxFileSize)) + _T(" ") + Text::toT(search.SizeTypeToString(search.typeFileSize)) : Util::emptyStringT);
	
	if (index == -1)
	{
		QTreeWidgetItem *it = new QTreeWidgetItem(list, l);
		it->setCheckState(0, ( search.isActive ? Qt::Checked : Qt::Unchecked) );
		datalist << new ADLSearch(search);
		datalistitem << it;
	}
	else
	{
		QTreeWidgetItem *it = new QTreeWidgetItem(l);
		it->setCheckState(0, ( search.isActive ? Qt::Checked : Qt::Unchecked) );
		list->insertTopLevelItem(index, it);		
		datalist.insert(index, new ADLSearch(search) );
		datalistitem.insert(index, it);
	}
}


void ADLSearchWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) list->hideColumn(-index-1);
	else list->showColumn(index);
}

void ADLSearchWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = list->header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(list->header()->mapToGlobal(point));
}

void ADLSearchWindow::showCnxtMenu(const QPoint& point)
{
	QModelIndex mi = list->indexAt(point);
	QAction *a, *b, *c;
	
		cnxtMenu->clear();
		a = cnxtMenu->addAction(/*QIcon(":/images/add.png"),*/ StilUtils::TstrtoQ(T_("&New...")) ,this ,SLOT(slotAdd()) );
		b = cnxtMenu->addAction(/*QIcon(":/images/properties.png"),*/ StilUtils::TstrtoQ(T_("&Properties")) ,this ,SLOT(slotProp()) );
		c = cnxtMenu->addAction(/*QIcon(":/images/remove.png"),*/ StilUtils::TstrtoQ(T_("&Remove")) ,this ,SLOT(slotRemove()) );
		b->setEnabled(false);
		c->setEnabled(false);
	
	if (mi.isValid())
	{	
		QTreeWidgetItem * w = list->itemFromIndex(mi);
		if (w)
		{
			b->setEnabled(true);
			c->setEnabled(true);
		}
	}
	cnxtMenu->exec(mapToGlobal(point));
}
