/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
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

#include "mdi_filelist.h"

FileListDlg::FileListDlg(QWidget *parent, const UserPtr &aUser, int64_t aSpeed, tstring &strFile) 
					: MdiChild(parent), shareSize(0),shareItems(0),listing(aUser)
{	
	setupUi(this);
	
	type = 5;
	idText = "user";

	pathList.clear();
	
	vDir->header()->hide();
	
	createActions();
	
	connect(vDir, SIGNAL(itemSelectionChanged()), this, SLOT(dirSelected()));
	//connect(contentTree, SIGNAL(itemSelectionChanged()), this, SLOT(dirSelectedOnContentTree()));
	connect(contentTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, 
								SLOT(dirDoubleClickedOnContentTree(QTreeWidgetItem *)));
							
	QStringList contentLabels;
	contentLabels << QObject::tr("Filename") << QObject::tr("Size") << QObject::tr("Type") 
					<< QObject::tr("TTH") << QObject::tr("Exact size");
	contentTree -> setHeaderLabels(contentLabels);
	
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
						QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
						QIcon::Normal, QIcon::On);
	fileIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	const string nick = getNickFromFilename(Text::fromT(strFile));
	listing.loadFile(Text::fromT(strFile));
	listing.getRoot()->setName(nick);
	
	buildDir(listing.getRoot());
	
	shareItems = listing.getTotalFileCount();
	shareSize = listing.getTotalSize();
	
	dcdebug("ShareItems: %d\nSharedSize: %lld\n", shareItems, shareSize);
	dcdebug("UserName: %s\n",StilUtils::getNicks(aUser).c_str());
	
	openContent(listing.getRoot());

	//parent -> setWindowTitle("Testtitle"); // set window title in format: "FileList - John" to ./filelist/john.tar.bz2
}

void FileListDlg::createActions()
{
	QAction *downloadAction = new QAction(tr("Download"), contentTree);
	contentTree -> addAction(downloadAction);
	contentTree -> setContextMenuPolicy(Qt::ActionsContextMenu);
	connect(downloadAction, SIGNAL(triggered()), SLOT(downloadClicked()));
}

void FileListDlg::buildDir(DirectoryListing::Directory::Ptr dirPtr, QTreeWidgetItem* parent)
{
	DirectoryListing::Directory::Iter it;
	DirectoryListing::File::Iter fileIt;

	QString dirNameUnicode(StilUtils::TstrtoQ(Text::toT(dirPtr->getName())));
	QTreeWidgetItem *newItem;
	
	if (parent == NULL)
	{
		newItem = new QTreeWidgetItem(vDir, QStringList(dirNameUnicode));
		vDir -> expandItem(newItem);
		pathList.append(newItem);
	}
	else
		newItem = new QTreeWidgetItem(parent, QStringList(dirNameUnicode));
	
	newItem -> setIcon(0, folderIcon);

	std::sort(dirPtr->directories.begin(), dirPtr->directories.end(), DirectoryListing::Directory::DirSort());
	for (it = dirPtr->directories.begin(); it != dirPtr->directories.end(); ++it)
	{
		buildDir(*it, newItem);
	}
}

void FileListDlg::openContent(DirectoryListing::Directory::Ptr contDirPtr)
{	
	DirectoryListing::Directory::List *dirs = &(contDirPtr->directories);
	DirectoryListing::Directory::Iter dirIter;
	DirectoryListing::File::List *files = &(contDirPtr->files);
	DirectoryListing::File::Iter fileIter;
	
	contentTree -> clear();
	
	QTreeWidgetItem *contentItem;
	int64_t size;
	
	for (dirIter = dirs->begin(); dirIter != dirs->end(); ++dirIter)
	{
		QString dirName(StilUtils::TstrtoQ(Text::toT(Util::getFileName((*dirIter)->getName()))));
		
		size = (*dirIter)->getTotalSize(false);
		contentItem = new QTreeWidgetItem(contentTree, QStringList(dirName));
		contentItem -> setText(1, StilUtils::fmtBytes(size));
		contentItem -> setText(2, tr("Dir"));
		contentItem -> setText(4, QString::fromStdString(Util::formatExactSize(size)));
		contentItem -> setIcon(0, folderIcon);
	}
	
	for (fileIter = files->begin(); fileIter != files->end(); ++fileIter)
	{
		QString fileName(StilUtils::TstrtoQ(Text::toT(Util::getFileName((*fileIter)->getName()))));
		
		size = (*fileIter)->getSize();
		
		contentItem = new QTreeWidgetItem(contentTree, QStringList(fileName));
		contentItem -> setText(1, StilUtils::fmtBytes(size));
		contentItem -> setText(2, tr("File"));
		contentItem -> setText(3, QString::fromStdString((*fileIter)->getTTH().toBase32()));
		contentItem -> setText(4, QString::fromStdString(Util::formatExactSize(size)));
		contentItem -> setIcon(0, fileIcon);
	}

}

void FileListDlg::dirSelected()
{
	pathList.clear();
	
	QList<QTreeWidgetItem*> itemList;

	itemList = vDir -> selectedItems();

	getPath(itemList.first());
	openContent(getSelectedDir());

}

void FileListDlg::getPath(QTreeWidgetItem* item)
{	
	pathList.prepend(item);
	
	QTreeWidgetItem* parentItem = item -> parent();
	
	if (parentItem)
		getPath(parentItem);
	else
		return;	
}

DirectoryListing::Directory* FileListDlg::getSelectedDir()
{
	DirectoryListing::Directory* current = listing.getRoot();
	DirectoryListing::Directory::Iter iter;
	
	if (pathList.size() == 1) return listing.getRoot();
	
	// miss root directory and find items	
	for (int i=1; i<pathList.size(); i++)
	{
		const string name = Text::fromT(StilUtils::QtoTstr(pathList.at(i)->text(0)));
		iter = std::find(current->directories.begin(), current->directories.end(), name);
		
		if(iter != current->directories.end())
		{
			current = *iter;
			//qDebug() << "Item" << pathList.at(i)->text(0) << "found";
			continue;
		}
		/*
		else 
			qDebug() << "Item" << pathList.at(i)->text(0) << "not found";
		*/
	}
	
	return *iter;
}

/*
void FileListDlg::dirSelectedOnContentTree()
{
	
	QList<QTreeWidgetItem*> itemList;
	itemList = contentTree -> selectedItems();
	
	if (itemList.empty()) return;
	
	QTreeWidgetItem* item = itemList.first();
	
	if (item->text(2) != tr("Dir")) return;
	
}
*/

void FileListDlg::dirDoubleClickedOnContentTree(QTreeWidgetItem* item)
{
	if (item->text(2) != tr("Dir")) return;
	
	QTreeWidgetItem* currentItem = findItemFromRightPanel(item);

	vDir -> expandItem(currentItem);
	vDir -> setCurrentItem(currentItem);
	openContent(getSelectedDir());
}

QTreeWidgetItem* FileListDlg::findItemFromRightPanel(QTreeWidgetItem* item)
{
	QTreeWidgetItem* parent = pathList.last();
	int childs = parent -> childCount();
	
	for (int i=0; i<childs; i++)
	{
		if (parent->child(i)->text(0) == item->text(0)) return parent->child(i);
	}
}

void FileListDlg::downloadClicked()
{
	QTreeWidgetItem *curItem = contentTree->currentItem();
	
	qDebug() << "Item" << curItem->text(0) << "right clicked";
}

string FileListDlg::getNickFromFilename(const string& fileName)
{
	string name = Util::getFileName(fileName);
	
	// Strip off any extensions
	if(Util::stricmp(name.c_str() + name.length() - 6, ".DcLst") == 0) {
		name.erase(name.length() - 6);
	}

	if(Util::stricmp(name.c_str() + name.length() - 4, ".bz2") == 0) {
		name.erase(name.length() - 4);
	}

	if(Util::stricmp(name.c_str() + name.length() - 4, ".xml") == 0) {
		name.erase(name.length() - 4);
	}

	// Find CID
	string::size_type i = name.rfind('.');
	if(i == string::npos) {
		return NULL;
	}

	size_t n = name.length() - (i + 1);
	// CID's always 39 chars long...
	if(n != 39)
		return NULL;

	CID cid(name.substr(i + 1));
	if(cid.isZero())
		return NULL;
	
	return name.substr(0, i);
}
