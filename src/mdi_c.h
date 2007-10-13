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

#ifndef __MDI_C_H__
#define __MDI_C_H__

#include <QDialog>
#include <QString>
#include <QAction>
#include <QWidget>

#include "highlighter.h"

#include "ui_PMWindow.h"
#include "ui_HUBWindow.h"
#include "ui_search.h"
#include "ui_favhublist.h"

//

class MdiChild : public QDialog
{
	Q_OBJECT
public:
	MdiChild(QWidget *parent);
	~MdiChild();
	QAction *action;
	
	int type;
	/*  0 - MdiChild (NONE)
		1 - Hub
		2 - Private Chat
		3 - Search
		4 - Favorite Hub List
	*/
	QString idText;
signals:
	void actionReleased(QAction *action);
};

class PMWindow : public MdiChild, private Ui::mdiPMwin
{
	Q_OBJECT
public:
	PMWindow(QWidget *parent, const QString &nick);
};

class HubWindow : public MdiChild, private Ui::mdiHUBwin
{
	Q_OBJECT
private:
	Highlighter *highlighter;
public:
	HubWindow(QWidget *parent, const QString &hubaddr);
	void setupeditor();
};

class SearchWindow : public MdiChild, private Ui::mdiSEARCHwin
{
	Q_OBJECT
private slots:
	void SearchBtn();
public:
	SearchWindow(QWidget *parent, const QString &what);
};

class FavoriteHubListWindow : public MdiChild, private Ui::mdiFAVHUBwin
{
	Q_OBJECT
public:
	FavoriteHubListWindow(QWidget *parent);
};


#endif // __MDI_C_H__
