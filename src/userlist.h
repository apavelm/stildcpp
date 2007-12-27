/***************************************************************************
 *   Copyright (C) 2007 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)                  *
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

#ifndef USERLIST_H
#define USERLIST_H
//
//#include "hub_win.h"

#include <QObject>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QComboBox>
#include <QTreeView>
#include <QAction>
#include <QHeaderView>
#include <QMenu>
#include <QDebug>

#include "userlistsortingmodel.h"

class HubWindow;

class HubUserList : public QObject
{
Q_OBJECT
public:
	HubUserList(QTreeView *&treeView, QLineEdit *&filterString, 
				QComboBox *&criteriaSortBox, const QStringList &columns);
	~HubUserList();
	void addUser(const QStringList &list, QIcon *&icon);
	void removeUser(const QString& str);
	void updateUser(int pos, const QStringList& strList, QIcon *&icon);
	int findUser(const QString& str);
	void clearList();
	
private slots:
	void filterChanged();
	void filterColumnChanged();
	void chooseColumn(QAction *action);
	void showColumnMenu(const QPoint &point);
	void set_topOp();
	void set_classicSort();
	
private:
	const int totalColumns;
	bool topOp;
	bool classicSort;
	QMenu *columnMenu;
	QTreeView *treeView;
	QLineEdit *filterString;
	QComboBox *criteriaSortBox;
	UserListSortingModel *proxyModel;
	QStandardItemModel *model;
	void checkAndReadSettings();
	void saveSettings();
	void resort();

};
#endif




