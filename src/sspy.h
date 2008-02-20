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


#ifndef __SEARCHSPY_H__
#define __SEARCHSPY_H__

#include "mdi_c.h"
#include "config.h"
#include "stilutils.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/ClientManagerListener.h"
#include "client/ShareManager.h"
#include "client/ClientManager.h"

#include "ui_searchspy.h"

using namespace std;
using namespace dcpp;

class SearchSpyWindow : public MdiChild, private Ui::mdi_searchspy, private ClientManagerListener
{
	Q_OBJECT
public:
	SearchSpyWindow(QWidget *parent);
	~SearchSpyWindow();
private:
	static const size_t AVG_TIME = 60;

	enum {
		SPEAK_SEARCH
	};

	enum {
		COLUMN_FIRST,
		COLUMN_STRING = COLUMN_FIRST,
		COLUMN_COUNT,
		COLUMN_TIME,
		COLUMN_LAST
	};

	static int columnSizes[COLUMN_LAST];

	bool bIgnoreTTH;

	size_t total, cur, perSecond[AVG_TIME];

	void initSecond();
	
	void speak(int a, tstring s) { emit sigSpeak(a, s); }
	QMenu * columnMenu;
	QMenu * cnxtMenu;
	QList<QTreeWidgetItem*> datalistitem;

	// ClientManagerListener
	virtual void on(ClientManagerListener::IncomingSearch, const string& s) throw();
private slots:
	void slotSpeak(int, tstring);
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void showCnxtMenu(const QPoint& point);
	void makeContextMenu();
	void slotSearch();
	void slotIgnore(int);
	bool eachSecond();
signals:
	void sigSpeak(int, tstring);
};

#endif // __SEARCHSPY_H__
