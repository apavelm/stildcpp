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

#include "sspy.h"
#include "mainwindowimpl.h"

int SearchSpyWindow::columnSizes[] = { 420, 50, 75 };
const size_t SearchSpyWindow::AVG_TIME; // TODO gcc needs this - why?

static const char* columnNames[] = 
{
	N_("Search String"),
	N_("Count"),
	N_("Time")
};

SearchSpyWindow::SearchSpyWindow(QWidget *parent) : MdiChild(parent),
	bIgnoreTTH(BOOLSETTING(SPY_FRAME_IGNORE_TTH_SEARCHES)),
	total(0),
	cur(0)
{	
	memset(perSecond, 0, sizeof(perSecond));
	setupUi(this);
	type = StilUtils::WIN_TYPE_SEARCH_SPY;
	datalistitem.clear();
	connect(this, SIGNAL(sigSpeak(int, tstring)), this, SLOT(slotSpeak(int, tstring)), Qt::QueuedConnection);
	ignoreTTH->setChecked(bIgnoreTTH);
	connect(ignoreTTH, SIGNAL(stateChanged(int)), this, SLOT(slotIgnore(int)) );
	
	// labels
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	searches->setHeaderLabels(columns);
	// width
	QStringList clist = StilUtils::TstrtoQ(Text::toT(SETTING(SPYFRAME_WIDTHS))).split(",");
	if (clist.size() != COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) searches->setColumnWidth(i, columnSizes[i]);
	else 
		for (int i=0; i<COLUMN_LAST; i++) 
			{
				if ( (clist[i]==QString()) || (clist[i].toInt() <= 0)) searches->setColumnWidth(i, columnSizes[i]);
				else searches->setColumnWidth(i, clist[i].toInt());
			}
	// visibility
	QStringList vlist = APPSTRING(s_SEARCHSPYWINDOW_COLUMN_VISIBILITY).split(",");
	if (vlist.size() == COLUMN_LAST)
		for (int i=0; i<COLUMN_LAST; i++) searches->header()->setSectionHidden(i, vlist[i].toInt());
	// order
	QStringList olist = StilUtils::TstrtoQ(Text::toT(SETTING(SPYFRAME_ORDER))).split(",");
	if (olist.size() == COLUMN_LAST)
		for (int j=0; j<COLUMN_LAST; j++) searches->header()->swapSections(searches->header()->visualIndex(olist[j].toInt()), j);
		
	// COLUMN (HEADER) MENUS
	columnMenu = new QMenu();
	connect(columnMenu, SIGNAL(triggered(QAction*)), this, SLOT(chooseColumn(QAction *)));
	searches->header()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(searches->header(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showColumnMenu(const QPoint&)));
	
	// CONTEXT MENU
	cnxtMenu = new QMenu();
	searches->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(searches, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showCnxtMenu(const QPoint&)));
	
	ShareManager::getInstance()->setHits(0);
	ClientManager::getInstance()->addListener(this);

	startTimer(1000);
}

SearchSpyWindow::~SearchSpyWindow()
{
	ClientManager::getInstance()->removeListener(this);
	datalistitem.clear();
	searches->clear();
	delete cnxtMenu;
	delete columnMenu;
	
	// visibility
	QStringList vv;
	for (int i=0; i<COLUMN_LAST; i++) 
		vv << QString::number(searches->header()->isSectionHidden(i));
	SETAPPSTRING(s_SEARCHSPYWINDOW_COLUMN_VISIBILITY, vv.join(","));
	// width
	QStringList w;
	for (int i=0; i<COLUMN_LAST; i++) 
		w << QString::number(searches->columnWidth(i));
	SettingsManager::getInstance()->set(SettingsManager::SPYFRAME_WIDTHS, w.join(",").toStdString());
	// order
	QStringList ww;
	for (int i=0; i<COLUMN_LAST; i++) 
		ww << QString::number(searches->header()->visualIndex(i));
	SettingsManager::getInstance()->set(SettingsManager::SPYFRAME_ORDER, ww.join(",").toStdString());

	SettingsManager::getInstance()->set(SettingsManager::SPY_FRAME_IGNORE_TTH_SEARCHES, bIgnoreTTH);
}

void SearchSpyWindow::chooseColumn(QAction *action)
{
	int index = action->data().toInt();

	if(index < 0) searches->hideColumn(-index-1);
	else searches->showColumn(index);
}

void SearchSpyWindow::showColumnMenu(const QPoint &point)
{
	columnMenu->clear();
	
	QStringList columns;
	foreach(tstring name, StilUtils::getStrings(columnNames))
		columns << StilUtils::TstrtoQ(name);
	
	for(int i = 0; i < COLUMN_LAST; i++)
	{
		QAction *action = new QAction(columns[i], columnMenu);
		
		bool isHidden = searches->header()->isSectionHidden(i);
		
		if (!isHidden)
			action->setData(-i-1);
		else
			action->setData(i);
		
		action->setCheckable(true);
		action->setChecked(!isHidden);
		columnMenu->addAction(action);
	}
	
	columnMenu->exec(searches->header()->mapToGlobal(point));
}

void SearchSpyWindow::showCnxtMenu(const QPoint& point)
{
	cnxtMenu->clear();
	cnxtMenu->addAction(QIcon(":/images/search.png"), StilUtils::TstrtoQ(T_("Search")) ,this ,SLOT(slotSearch()) );
	cnxtMenu->exec(mapToGlobal(point));
}

void SearchSpyWindow::slotSearch()
{
	tstring searchString = StilUtils::QtoTstr(searches->currentItem()->text(0));
	if(Util::strnicmp(searchString.c_str(), _T("TTH:"), 4) == 0)
		MainWindowImpl::getInstance()->SearchFunc(searchString.substr(4), 0, SearchManager::SIZE_DONTCARE, SearchManager::TYPE_TTH);
	else
		MainWindowImpl::getInstance()->SearchFunc(searchString);
}

void SearchSpyWindow::timerEvent(QTimerEvent *)
{
	size_t tot = std::accumulate(perSecond, perSecond + AVG_TIME, 0u);
	size_t t = std::max(1u, std::min(cur, AVG_TIME));
	
	float x = static_cast<float>(tot)/t;

	cur++;
	perSecond[cur % AVG_TIME] = 0;
	lbl_ave->setText(StilUtils::TstrtoQ(str(TF_("Average/s: %1%") % x)));
}

void SearchSpyWindow::slotIgnore(int)
{
	bIgnoreTTH = ignoreTTH->isChecked();
}

void SearchSpyWindow::slotSpeak(int q, tstring x)
{
	if(q == SPEAK_SEARCH)
	{
		total++;
		// Not thread safe, but who cares really...
		perSecond[cur % AVG_TIME]++;
		
		QList<QTreeWidgetItem*> lst = searches->findItems(StilUtils::TstrtoQ(x), Qt::MatchFixedString);
		QList<QTreeWidgetItem*>::iterator iter;
		
		if(lst.isEmpty()) 
		{
			QStringList a;
			a << StilUtils::TstrtoQ(x) << StilUtils::TstrtoQ(Text::toT(Util::toString(1))) << StilUtils::TstrtoQ(Text::toT(Util::getTimeString()));
			QTreeWidgetItem *it = new QTreeWidgetItem(searches, a);
			datalistitem << it;
			//it->setIcon(0,QIcon(":/images/hub.png"));
			
			if(datalistitem.count() > 500) 
			{
				delete datalistitem.at(0);
				datalistitem.removeAt(0);
			}
		} 
		else 
			{
				for (iter = lst.begin(); iter!=lst.end(); ++iter )
				{
					int c = QString((*iter)->text( COLUMN_COUNT )).toInt() + 1;
					(*iter)->setText(COLUMN_COUNT, QString::number(c));
					(*iter)->setText(COLUMN_TIME, StilUtils::TstrtoQ(Text::toT(Util::getTimeString())) );
				}
			}
			
		lbl_total->setText(StilUtils::TstrtoQ( str(TF_("Total: %1%") % total) ));
		lbl_hits->setText(StilUtils::TstrtoQ( str(TF_("Hits: %1%") % ShareManager::getInstance()->getHits()) ));
		double ratio = total > 0 ? ((double)ShareManager::getInstance()->getHits()) / (double)total : 0.0;
		lbl_hitratio->setText(StilUtils::TstrtoQ( str(TF_("Hit Ratio: %1%") % ratio) ));
	}
}

void SearchSpyWindow::on(ClientManagerListener::IncomingSearch, const string& s) throw()
{
	if(bIgnoreTTH && s.compare(0, 4, "TTH:") == 0)
		return;
	tstring* x = new tstring(Text::toT(s));
	tstring::size_type i = 0;
	while( (i=x->find(_T('$'))) != string::npos)
	{
		(*x)[i] = _T(' ');
	}
	speak(SPEAK_SEARCH, *x);
}
