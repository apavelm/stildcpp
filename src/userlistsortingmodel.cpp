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

#include "userlistsortingmodel.h"
//

UserListSortingModel::UserListSortingModel(QObject *parent) 
	: QSortFilterProxyModel(parent)
{
	// TODO
}

bool UserListSortingModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
	QString leftString = sourceModel()->data(left).toString();
	QString rightString = sourceModel()->data(right).toString();
	
	int col = left.column();

	bool leftOp = testOp(left);
	bool rightOp = testOp(right);
	
	int64_t leftSize = getSize(left);
	int64_t rightSize = getSize(right);
	
	//Fixme: read settings every time from private fields, not from config
	//bool topOp = APPSETTING(i_SORTUSERLISTOPSFIRST);
	//bool classicSort = APPSETTING(i_SORTUSERLISTCLASSIC);

	//classicSort must be tested before topOp
	if ((classicSort && col == 0) || (!classicSort && topOp))
	{
		if (leftOp && !rightOp) return true;
		if (!leftOp && rightOp) return false;
	}
	
	if (col == 1) return leftSize < rightSize;
	
	return (QString::compare(leftString, rightString, Qt::CaseInsensitive) < 0);
}

bool UserListSortingModel::testOp(const QModelIndex &index) const
{
	QString indexOp = sourceModel()->data(index.sibling(index.row(), sourceModel()->columnCount() - 2)).toString();
	return (indexOp == "Op");
}

int64_t UserListSortingModel::getSize(const QModelIndex &index) const
{
	QString sizeString = sourceModel()->data(index.sibling(index.row(), sourceModel()->columnCount() - 1)).toString();
	return sizeString.toULongLong();
}

void UserListSortingModel::setSortingSettings(bool _classicSort, bool _topOp)
{
	classicSort = _classicSort;
	topOp = _topOp;
}
//
