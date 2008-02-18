/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
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

#ifndef USERLISTSORTINGMODEL_H
#define USERLISTSORTINGMODEL_H
//
#include <QSortFilterProxyModel>
#include <QDebug>

#include "config.h"
//

class UserListSortingModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
	UserListSortingModel(QObject *parent = 0);
	void setSortingSettings(bool _classicSort, bool _topOp);
	
protected:
	bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
private:
	bool testOp(const QModelIndex &index) const;
	int64_t getSize(const QModelIndex &index) const;
	bool classicSort;
	bool topOp;
};

#endif
