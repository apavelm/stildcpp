#include "userlist.h"
//

HubUserList::~HubUserList()
{
	if (treeView) treeView = 0;
	if (filterString) filterString = 0;
	if (criteriaSortBox) criteriaSortBox = 0;
}

HubUserList::HubUserList(QTreeView *&_treeView, QLineEdit *&_filterString, 
							QComboBox *&_criteriaSortBox, const QStringList &columns) 
	:treeView(_treeView), filterString(_filterString), criteriaSortBox(_criteriaSortBox), totalColumns(columns.size())

{

	model = new QStandardItemModel(0, totalColumns);

	for (int i=0;i<totalColumns;i++)
		model->setHeaderData(i, Qt::Horizontal, columns.at(i));
	
	QObject::connect(filterString, SIGNAL(textChanged(const QString &)),
			this, SLOT(filterChanged()));
	QObject::connect(criteriaSortBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(filterColumnChanged()));

	proxyModel = new UserListSortingModel();
	proxyModel -> setDynamicSortFilter(true);
	treeView   -> setEditTriggers(QAbstractItemView::NoEditTriggers);
	treeView   -> setModel(proxyModel);
	proxyModel -> setSourceModel(model);
	treeView -> sortByColumn(0, Qt::AscendingOrder);
	proxyModel->setFilterKeyColumn(-1);
	
	//Hide two last columns
	treeView   -> hideColumn(totalColumns - 1);
	treeView   -> hideColumn(totalColumns - 2);
	
}

void HubUserList::filterChanged()
{
	QRegExp regExp(filterString->text(), Qt::CaseInsensitive, QRegExp::RegExp);
	proxyModel->setFilterRegExp(regExp);
}

void HubUserList::filterColumnChanged()
{
	proxyModel->setFilterKeyColumn(criteriaSortBox->currentIndex() - 1);
}

void HubUserList::addUser(const QStringList &list, QIcon *&icon)
{
	QStandardItem *parentItem = model->invisibleRootItem();

	QList<QStandardItem *> itemList;
	QStringList::const_iterator constIterator;
	
	int i = 0;
	QStandardItem *item;
    
    for (i = 0, constIterator = list.constBegin(); constIterator != list.constEnd(); 
    		++constIterator, i++)
    {
    	item = new QStandardItem((*constIterator));
    	
    	if (i == 0) item->setIcon(*icon);
    		
    	itemList << item;
    }
    parentItem->appendRow(itemList);    
}

int HubUserList::findUser(const QString& str)
{
	for (int i=0; i<model->rowCount(); i++)
	{
		QString userName = model->data(model->index(i,0)).toString();
		if (str == userName) return i;
	}
	
	return -1;
}

void HubUserList::removeUser(const QString& str)
{
	int row = findUser(str);
	if (row != -1) model->removeRow(row);
}

void HubUserList::clearList()
{
	model->removeRows(0, model->rowCount());
}

void HubUserList::updateUser(int pos, const QStringList& strList, QIcon *&icon)
{	
	QList<QStandardItem *> itemList;
	QStandardItem *item;
	
	//TODO: don't count last hidden row 
	for (int i=0; i<model->columnCount(); i++)
	{
		item = new QStandardItem(strList.at(i));
		if (i == 0) item->setIcon(*icon);
		itemList << item;
	}
	
	model->takeRow(pos);
	model->insertRow(pos, itemList);
}
