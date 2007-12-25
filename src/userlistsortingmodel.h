#ifndef USERLISTSORTINGMODEL_H
#define USERLISTSORTINGMODEL_H
//
#include <QSortFilterProxyModel>
#include <QDebug>
//

class UserListSortingModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
	UserListSortingModel(QObject *parent = 0);
	
protected:
	bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;
private:
	bool testOp(const QModelIndex &index) const;
	int64_t getSize(const QModelIndex &index) const;
};

#endif
