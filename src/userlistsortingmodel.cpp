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
	int64_t leftSize, rightSize;
	bool leftOp, rightOp;
	switch(col)
	{
		case 0:
			leftString = sourceModel()->data(left).toString();
			rightString = sourceModel()->data(right).toString();
			leftOp = UserListSortingModel::testOp(left);
			rightOp = UserListSortingModel::testOp(right);
			
			if (leftOp && !rightOp) return true;
			if (!leftOp && rightOp) return false;
			return leftString < rightString;
			break;
			
		case 1:
			leftSize = getSize(left);
			rightSize = getSize(right);
			return leftSize < rightSize;
			break;
			
		default:
			return leftString < rightString;
	}
}

bool UserListSortingModel::testOp(const QModelIndex &index) const
{
	QString indexOp = sourceModel()->data(index.sibling(index.row(), sourceModel()->columnCount() - 2)).toString();
	return (indexOp == "Op");
}

int64_t UserListSortingModel::getSize(const QModelIndex &index) const
{
	int64_t size = sourceModel()->data(index.sibling(index.row(), sourceModel()->columnCount() - 1)).toInt();
	return size;
}
//
