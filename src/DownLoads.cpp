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

#include "DownLoads.h"

int DownLoadsWindow::columnSizes[] = { 200, 300, 150, 200, 125, 100};

static const char* columnNames[] = {
	N_("Filename"),
	N_("Path"),
	N_("Status"),
	N_("Time left"),
	N_("Speed"),
	N_("Done"),
	N_("Size")
};

DownLoadsWindow::DownLoadsWindow(QWidget *parent) : MdiChild(parent), startup(true)
{
	setupUi(this);
	type = 15;
	idText = "Downloads";
	setTabText(tr("DownLoads"));
	
	datalist.clear();
	datalistitem.clear();
	
	// COLUMN LABELS (text)
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	downloads->setHeaderLabels(columns);
	
	// COLUMN WIDTHs
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOADSFRAME_WIDTHS))).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) downloads->setColumnWidth(i, columnSizes[i]);
	else 
		for (int i=0; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) downloads->setColumnWidth(i, columnSizes[i]);
				else downloads->setColumnWidth(i, clist[i].toInt());
			}
			
	// SETTING COLUMNS VISIBILITY
	QStringList vlist = APPSTRING(s_DLWINDOW_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) downloads->header()->setSectionHidden(i, vlist[i].toInt());
			
	// COLUMNS ORDER SET
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOADSFRAME_ORDER))).split(",");
	if (olist.size() == COLUMN_LAST)
		for (int j=0; j<COLUMN_LAST; j++) 
			downloads->header()->swapSections(downloads->header()->visualIndex(olist[j].toInt()), j);
	// END OF COLUMNS ORDER SET
	
	startup = false;
	
	connect(this, SIGNAL(sigSpeak(int , const QString & )), this, SLOT(slotSpeak(int , const QString & )), Qt::QueuedConnection);
	
	// COLUMN (HEADER) MENU
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	downloads->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(downloads->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	// ADDING LISTENERS
	QueueManager::getInstance()->addListener(this);
	DownloadManager::getInstance()->addListener(this);
}

DownLoadsWindow::~DownLoadsWindow()
{
	datalist.clear();
	datalistitem.clear();
	// SAVING COLUMN VISIBILITY
	QStringList vv;
	for (int i=0; i<COLUMN_LAST; i++) vv << QString::number(downloads->header()->isSectionHidden(i));
	SETAPPSTRING(s_DLWINDOW_COLUMN_VISIBILITY, vv.join(","));
	
	// SAVING COLUMN WIDTHs
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) w << QString::number(downloads->columnWidth(i));
	QString r = w.join(",");
	SettingsManager::getInstance()->set(SettingsManager::DOWNLOADSFRAME_WIDTHS, r.toStdString());
	
	//SAVING COLUMN ORDER
	QStringList ww;
	for (int i=0; i<COLUMN_LAST; i++) ww << QString::number(downloads->header()->visualIndex(i));
	QString rr = ww.join(",");
	SettingsManager::getInstance()->set(SettingsManager::DOWNLOADSFRAME_ORDER, rr.toStdString());
	
	// REMOVING LISTENERS
	QueueManager::getInstance()->removeListener(this);
	DownloadManager::getInstance()->removeListener(this);
	
	delete columnMenu;
}

void DownLoadsWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) downloads->hideColumn(-index-1);
	else downloads->showColumn(index);
}

void DownLoadsWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = downloads->header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(downloads->header()->mapToGlobal(point));
}

DownLoadsWindow::DownloadInfo::DownloadInfo(const string& target, int64_t size_, const TTHValue& tth_) : 
	path(target), 
	done(QueueManager::getInstance()->getPos(target)), 
	size(size_), 
	users(0),
	tth(tth_)
{
	columns[COLUMN_FILE] = Text::toT(Util::getFileName(target));
	columns[COLUMN_PATH] = Text::toT(Util::getFilePath(target));
	columns[COLUMN_SIZE] = Text::toT(Util::formatBytes(size));
	
	_update();
}

void DownLoadsWindow::DownloadInfo::_update(const DownLoadsWindow::TickInfo& ti) 
{
	users = ti.users;
	done = ti.done + QueueManager::getInstance()->getInstance()->getPos(ti.path);
	bps = ti.bps;
	_update();
}

void DownLoadsWindow::DownloadInfo::_update()
{
	if(users == 0) 
	{
		columns[COLUMN_STATUS] = T_("Waiting for slot");
		columns[COLUMN_TIMELEFT].clear();
		columns[COLUMN_SPEED].clear();
	} 
	else 
	{
		columns[COLUMN_STATUS] = str(TFN_("Downloading from %1% user", "Downloading from %1% users", users) % users);
		columns[COLUMN_TIMELEFT] = Text::toT(Util::formatSeconds(static_cast<int64_t>(timeleft())));
		columns[COLUMN_SPEED] = str(TF_("%1%/s") % Text::toT(Util::formatBytes(static_cast<int64_t>(bps))));
	}
	columns[COLUMN_DONE] = Text::toT(Util::formatBytes(done));
}

int DownLoadsWindow::find(const string& path) 
{
	for(int64_t i = 0; i < datalist.size(); ++i) 
	{
		DownloadInfo* di = datalist.at(i);
		if(Util::stricmp(di->path, path) == 0) return i;
	}
	return -1;
}

void DownLoadsWindow::on(DownloadManagerListener::Tick, const DownloadList& l) throw() 
{
	std::vector<TickInfo*> dis;
	for(DownloadList::const_iterator i = l.begin(); i != l.end(); ++i) {
		Download* d = *i;
		if(d->getType() != Transfer::TYPE_FILE) {
			continue;
		}
		
		TickInfo* ti = 0;
		for(std::vector<TickInfo*>::iterator j = dis.begin(); j != dis.end(); ++j) {
			TickInfo* ti2 = *j;
			if(Util::stricmp(ti2->path, d->getPath()) == 0) {
				ti = ti2;
				break;
			}
		}
		if(!ti) {
			ti = new TickInfo(d->getPath());
			dis.push_back(ti);
		}
		ti->users++;
		ti->bps += d->getAverageSpeed();
		ti->done += d->getPos();
	}
	
	for(std::vector<TickInfo*>::iterator i = dis.begin(); i != dis.end(); ++i)
		emit sigSpeak(SPEAKER_TICK, convTick2QString(*i));
		
}

void DownLoadsWindow::on(DownloadManagerListener::Complete, Download* d) throw() 
{
	emit sigSpeak(SPEAKER_DISCONNECTED, StilUtils::TstrtoQ(Text::toT(d->getPath())));
}

void DownLoadsWindow::on(DownloadManagerListener::Failed, Download* d, const string&) throw() 
{
	emit sigSpeak(SPEAKER_DISCONNECTED, StilUtils::TstrtoQ(Text::toT(d->getPath())));
}

void DownLoadsWindow::on(QueueManagerListener::Removed, QueueItem* qi) throw() 
{
	emit sigSpeak(SPEAKER_DISCONNECTED, StilUtils::TstrtoQ(Text::toT(qi->getTarget())));
}

void DownLoadsWindow::slotSpeak(int wParam, const QString & lParam)
{
	if(wParam == SPEAKER_TICK) 
	{
		TickInfo ti = convQString2Tick(lParam);
		if (strcmp(ti.path.c_str(), "") == 0) return; //check
		int i = find(ti.path);
		if(i == -1) 
		{
			int64_t size = QueueManager::getInstance()->getSize(ti.path);
			if(size == -1) return;
			TTHValue tth;
			if(QueueManager::getInstance()->getTTH(ti.path, tth)) 
			{
				datalist << new DownloadInfo(ti.path, size, tth);
				QStringList lst;
				for (int j=COLUMN_FIRST; j<COLUMN_LAST; j++) 
					lst << StilUtils::TstrtoQ(datalist.last()->getText(j));
				downloads->setUpdatesEnabled(false);
				QTreeWidgetItem *fItem = new QTreeWidgetItem(downloads, lst); // FOR FUTURE ADDING ProgressBar
				downloads->setUpdatesEnabled(true);
			} 
			else return;
		}
		DownloadInfo* di = datalist.at(i);
		di->_update(ti);
				downloads->setUpdatesEnabled(false);
				for (int j=COLUMN_FIRST; j<COLUMN_LAST; j++)
					downloads->itemFromIndex(datalistitem[i])->setText(i, StilUtils::TstrtoQ(datalist[i]->columns[j]) );
				downloads->setUpdatesEnabled(true);
	} 
	else 
		if(wParam == SPEAKER_DISCONNECTED) 
		{
			string path = Text::fromT(StilUtils::QtoTstr(lParam));
			int i = find(path);
			if(i != -1) 
			{
				DownloadInfo* di = datalist.at(i);
				if(--di->users == 0) di->bps = 0;
				di->_update();
				
				downloads->setUpdatesEnabled(false);
				for (int j=COLUMN_FIRST; j<COLUMN_LAST; j++)
					downloads->itemFromIndex(datalistitem[i])->setText(i, StilUtils::TstrtoQ(datalist[i]->columns[j]) );
				downloads->setUpdatesEnabled(true);
			}
		} 
	else 
		if(wParam == SPEAKER_REMOVED) 
		{
			string path = Text::fromT(StilUtils::QtoTstr(lParam));
			int i = find(path);
			if(i != -1) 
			{
				downloads->setUpdatesEnabled(false);
				if (datalist.at(i))
				{
					QTreeWidgetItem *w = downloads->itemFromIndex(datalistitem[i]);
					if (w) delete w;
					datalist.removeAt(i);
					datalistitem.removeAt(i);
				}
				downloads->setUpdatesEnabled(true);
			}
		}
		
	return;
}

const QString DownLoadsWindow::convTick2QString(TickInfo * t)
{
	QStringList sl;
	sl << StilUtils::TstrtoQ(Text::toT(t->path));
	sl << QString::number(t->done);
	sl << QString::number(t->bps, 'g', 20);
	sl << QString::number(t->users);
	return sl.join("<||-||>");
}

DownLoadsWindow::TickInfo DownLoadsWindow::convQString2Tick(const QString & s)
{
	QStringList sl = s.split("<||-||>");
	if (sl.size() != 4) return TickInfo("");
	TickInfo ti = TickInfo(Text::fromT(StilUtils::QtoTstr(sl[0])));
	ti.done = sl[1].toLongLong();
	ti.bps = sl[2].toDouble();
	ti.users = sl[3].toInt();
	return ti; 
}
