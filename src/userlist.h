#ifndef USERLIST_H
#define USERLIST_H
//
//#include "hub_win.h"

#include "userlistsortingmodel.h"
//
#include <QObject>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QComboBox>
#include <QTreeView>
#include <QDebug>

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
	
private:
	const int totalColumns;
	QTreeView *treeView;
	QLineEdit *filterString;
	QComboBox *criteriaSortBox;
	UserListSortingModel *proxyModel;
	QStandardItemModel *model;

};
#endif




