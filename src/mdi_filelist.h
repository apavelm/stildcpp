/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
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

#ifndef __MDI_FILELIST_H__
#define __MDI_FILELIST_H__

#include "mdi_c.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/DirectoryListing.h"
#include "client/ClientManager.h"
#include "client/ShareManager.h"
#include "client/User.h"
#include "stilutils.h"
#include "ui_filelistdialog.h"
#include <QTextCodec>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QHeaderView>
#include <QList>
#include <QtDebug>

using namespace dcpp;

class FileListDlg : public MdiChild, private Ui::DialogFileList
{
	Q_OBJECT
public:
	FileListDlg(QWidget *parent, const UserPtr &, int64_t, tstring &);
	void loadFile(const UserPtr &aUser, int64_t aSpeed);
	static string getNickFromFilename(const string& fileName);
	bool isConnected() { return usr->isOnline(); }
	
private slots:
	void dirSelected();
//	void dirSelectedOnContentTree();
	void dirDoubleClickedOnContentTree(QTreeWidgetItem * item);
	void downloadClicked();
	
private:
	void buildDir(DirectoryListing::Directory::Ptr dirPtr, QTreeWidgetItem* parent = NULL);
	void openContent(DirectoryListing::Directory::Ptr contDirPtr);
	void getPath(QTreeWidgetItem* item);
	void createActions();
	QTreeWidgetItem* findItemFromRightPanel(QTreeWidgetItem* item);
	DirectoryListing::Directory* getSelectedDir(void);
	//QList<QTreeWidgetItem *> dirList; 
	int shareItems;
	int64_t shareSize;

	static QIcon folderIcon;
	static QIcon fileIcon;

	DirectoryListing listing;
	QList<QTreeWidgetItem *> pathList;
	UserPtr usr;
	
};

#endif // __MDI_FILELIST_H__
