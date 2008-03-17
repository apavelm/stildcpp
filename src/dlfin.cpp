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
 
#include "dlfin.h"
#include "mainwindowimpl.h"

int DownloadFinishedWindow::columnSizes[] = { 100, 110, 290, 125, 80, 80, 80, 90 };

const char* DownloadFinishedWindow::columnNames[] = { 
	N_("Filename"),
	N_("Time"),
	N_("Path"), 
	N_("Nick"),
	N_("Hub"),
	N_("Size"),
	N_("Speed"),
	N_("CRC Checked")
};

DownloadFinishedWindow::DownloadFinishedWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = StilUtils::WIN_TYPE_FINISHED_DL;
	
	datalist.clear();
	datalistitem.clear();
	connect(this, SIGNAL(sigSpeak(int, FinishedItemPtr)), this, SLOT(slotSpeak(int, FinishedItemPtr)), Qt::QueuedConnection);
	
	// labels
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	items->setHeaderLabels(columns);
	// width
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(FINISHED_WIDTHS))).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) items->setColumnWidth(i, columnSizes[i]);
	else 
		for (int i=0; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) items->setColumnWidth(i, columnSizes[i]);
				else items->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	QStringList vlist = APPSTRING(s_FINISHED_DL_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) items->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(FINISHED_ORDER))).split(",");
	if (olist.size() == COLUMN_LAST)
		for (int j=0; j<COLUMN_LAST; j++) items->header()->swapSections(items->header()->visualIndex(olist[j].toInt()), j);
		
	// COLUMN (HEADER) MENUS
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	items->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(items->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	// CONTEXT MENU
	cnxtMenu = new QMenu();
	items->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(items, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	FinishedManager::getInstance()->addListener(this);
	updateList(FinishedManager::getInstance()->lockList(false));
	FinishedManager::getInstance()->unlockList();
}

DownloadFinishedWindow::~DownloadFinishedWindow()
{
	datalist.clear();
	datalistitem.clear();
	FinishedManager::getInstance()->removeListener(this);
	// visibility
	QStringList vv;
	for (int i=0; i<COLUMN_LAST; i++) 
		vv << QString::number(items->header()->isSectionHidden(i));
	SETAPPSTRING(s_FINISHED_DL_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) 
		w << QString::number(items->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::FINISHED_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i=0; i<COLUMN_LAST; i++) 
		ww << QString::number(items->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::FINISHED_ORDER, ww.join(",").toStdString());
	
	items->clear();
	delete cnxtMenu;
	delete columnMenu;
}


void DownloadFinishedWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) items->hideColumn(-index-1);
	else items->showColumn(index);
}

void DownloadFinishedWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = items->header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(items->header()->mapToGlobal(point));
}

void DownloadFinishedWindow::makeContextMenu() 
{
	cnxtMenu->clear();
	cnxtMenu->addAction(/*QIcon(":/images/search.png"),*/ StilUtils::TstrtoQ(T_("&View as text")) ,this ,SLOT(slotViewAsText()) );
	QAction * t = cnxtMenu->addAction(/*QIcon(":/images/search.png"),*/ StilUtils::TstrtoQ(T_("&Open")) ,this ,SLOT(slotOpen()) );
	cnxtMenu->addAction(/*QIcon(":/images/search.png"),*/ StilUtils::TstrtoQ(T_("Open &folder")) ,this ,SLOT(slotOpenFolder()) );
	cnxtMenu->addAction(/*QIcon(":/images/search.png"),*/ StilUtils::TstrtoQ(T_("&Remove")) ,this ,SLOT(slotRemove()) );
	cnxtMenu->addAction(/*QIcon(":/images/search.png"),*/ StilUtils::TstrtoQ(T_("Remove &all")) ,this ,SLOT(slotRemoveAll()) );
	cnxtMenu->setDefaultAction(t);
}

void DownloadFinishedWindow::slotViewAsText()
{
	QList<QTreeWidgetItem *> lt = items->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		int n = datalistitem.indexOf(lt[i]); 
		QString fn = StilUtils::TstrtoQ(datalist.at(n)->getText(COLUMN_PATH)) + StilUtils::TstrtoQ(datalist.at(n)->getText(COLUMN_FILE));
		MainWindowImpl::getInstance()->openTextWindow(fn);
	}
}

void DownloadFinishedWindow::slotOpen()
{
	QList<QTreeWidgetItem *> lt = items->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		int n = datalistitem.indexOf(lt[i]);
		datalist.at(n)->openFile();
	}
}

void DownloadFinishedWindow::slotOpenFolder()
{
	QList<QTreeWidgetItem *> lt = items->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		int n = datalistitem.indexOf(lt[i]);
		datalist.at(n)->openFolder();
	}
}

void DownloadFinishedWindow::slotRemove()
{
	QList<QTreeWidgetItem *> lt = items->selectedItems();
	if (lt.isEmpty()) return;
	for (int i = 0; i < lt.size(); i++)
	{
		QTreeWidgetItem * w = lt[i];
		int n = datalistitem.indexOf(w);
		FinishedManager::getInstance()->remove(datalist.at(n)->entry, false);
		delete datalist.at(n);
		datalist.removeAt(n);
		delete w;
		datalistitem.removeAt(n);
	}
}

void DownloadFinishedWindow::slotRemoveAll()
{
	FinishedManager::getInstance()->removeAll(false);
}

void DownloadFinishedWindow::showCnxtMenu(const QPoint& point)
{
	if (items->indexAt(point).isValid())
	{
		makeContextMenu();
		cnxtMenu->exec(mapToGlobal(point));
	}
}

void DownloadFinishedWindow::updateStatus()
{
	lbl_count->setText(StilUtils::TstrtoQ( str(TFN_("%1% item", "%1% items", datalistitem.size()) % datalistitem.size()) ));
	lbl_bytes->setText(StilUtils::TstrtoQ( Text::toT(Util::formatBytes(totalBytes)) ));
	lbl_speed->setText(StilUtils::TstrtoQ( str(TF_("%1%/s") % Text::toT(Util::formatBytes((totalTime > 0) ? totalBytes * ((int64_t)1000) / totalTime : 0))) ));
}

void DownloadFinishedWindow::updateList(const FinishedItemList& fl)
{
	items->setUpdatesEnabled(false);
	for(FinishedItemList::const_iterator i = fl.begin(); i != fl.end(); ++i) {
		addEntry(*i);
	}
	updateStatus();
	items->setUpdatesEnabled(true);
}

void DownloadFinishedWindow::addEntry(FinishedItemPtr entry)
{
	totalBytes += entry->getChunkSize();
	totalTime += entry->getMilliSeconds();
	
	ItemInfo * ii = new ItemInfo(entry);
	datalist << ii;
	QStringList lst;
	for (int i = 0; i < COLUMN_LAST; i++)
		lst <<  StilUtils::TstrtoQ( ii->getText(i) );
	QTreeWidgetItem *it = new QTreeWidgetItem(items, lst);
	datalistitem << it;
	it->setIcon(0, ii->getImage());
}

void DownloadFinishedWindow::clearList()
{
	items->setUpdatesEnabled(false);
	for (int i = 0; i<datalistitem.size(); i++)
	{
		delete datalistitem.at(0);
		datalistitem.removeAt(0);
		delete datalist.at(0);
		datalist.removeAt(0);
	}
	items->clear();
	items->setUpdatesEnabled(true);
}

void DownloadFinishedWindow::slotSpeak(int type, FinishedItemPtr entry) 
{
	if(type == SPEAK_ADD_LINE)
	{
		addEntry(entry);
//		this->setDirty(SettingsManager::BOLD_FINISHED_DOWNLOADS);
		updateStatus();
	}
	else 
		if(type == SPEAK_REMOVE)
		{
			updateStatus();
		} 
		else 
			if(type == SPEAK_REMOVE_ALL) 
			{
				clearList();
				updateStatus();
			}
}

void DownloadFinishedWindow::on(Added, bool upload, FinishedItemPtr entry) throw()
{
	if(upload == false)
		speak(SPEAK_ADD_LINE, entry);
}

void DownloadFinishedWindow::on(Removed, bool upload, FinishedItemPtr entry) throw()
{
	if(upload == false) 
	{
		totalBytes -= entry->getChunkSize();
		totalTime -= entry->getMilliSeconds();
		speak(SPEAK_REMOVE);
	}
}

void DownloadFinishedWindow::on(RemovedAll, bool upload) throw()
{
	if(upload == false) 
	{
		totalBytes = 0;
		totalTime = 0;
		speak(SPEAK_REMOVE_ALL);
	}
}
