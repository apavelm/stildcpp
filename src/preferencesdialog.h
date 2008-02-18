/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QDir>
#include <QFontDialog>
#include <QHeaderView>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLocale>
#include <QTextCodec>

#include "config.h"
#include "stilutils.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/SettingsManager.h"
#include "client/FavoriteManager.h"
#include "client/ShareManager.h"
#include "client/Text.h"
//

#include "ui_preferencesdialog.h"


class PreferencesDialogPrivate;

class PreferencesDialog : public QDialog, private Ui::PreferencesDialog 
{
	Q_OBJECT
	
private:
	void accept();
	void initCategoryList();
	void initGeneralPage();
	void initConnectionPage();
	void initDownloadsPage();
	void initDownloadsFavPage();
	
	void initSharingPage();
	void initMessagesPage();
	void initLNFPage();
	//////////////////////////////////////
	
	void applyGeneralPage();
	void applyConnectionPage();
	void applyDownloadsPage();
	
	void applySharingPage();
	void applyMessagesPage();
	void applyLNFPage();
	
private slots:
	void on_okBtn_clicked();
	void on_categoryList_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void ConnectionPageUpdate();
	void DownloadsFavPageRename();
	void DownloadsFavPageRemove();
	void DownloadsFavPageAdd();
	void DownloadsPageBrowse1();
	void DownloadsPageBrowse2();
	void DownloadsPageConfPublic();
	void SharingPageRename();
	void SharingPageRemove();
	void SharingPageAdd();
	void SharingPageHidden(int);
	void MessagesPageHelp();
	void MessagesPageBrowse();
	
public:
	PreferencesDialog(QWidget *);
};

#endif
