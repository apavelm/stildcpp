/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
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

#include "ui_preferencesdialog.h"
#include <QDialog>
#include <QDir>
#include <QFontDialog>
#include <QHeaderView>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLocale>

class PreferencesDialogPrivate;

class PreferencesDialog : public QDialog, private Ui::PreferencesDialog 
{
	Q_OBJECT
	
private:
	void accept();
	void initCategoryList();
	void initLookNFeelPage();
	
private slots:
	void on_okBtn_clicked();
	void on_categoryList_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	
public:
	PreferencesDialog(QWidget *);
/*
public slots:
	void updateIconSet();
	void updateTranslation();

private slots:
	void on_addButton_clicked();
	void on_categoryList_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
	void on_deleteButton_clicked();
	void on_downButton_clicked();
	void on_fontButton_clicked();
	void on_patternLine_textChanged(const QString &);
	void on_testLine_textChanged(const QString &);
	void on_upButton_clicked();
	void crossfadeChanged();
	void iconsetChanged(QListWidgetItem *);
	void localeChanged(QListWidgetItem *);
	void notifierChanged(int);
	void outputChanged(QTreeWidgetItem *, int);
	void serverSelectionChanged();
	void styleChanged(QListWidgetItem *);

private:
	void initServerPage();
	void initConnectionPage();
	void initLookAndFeelPage();
	void initIconSetPage();
	void initLibraryPage();
	void initDirectoriesPage();
	void initPlaylistPage();
	void initStylePage();
	void initShortcutPage();
	void initLanguagePage();
	void initDynamicPlaylistPage();
	void initNotificationsPage();
	void initTagGuesserPage();
	void initTrayIconPage();
	void setFontString(const QFont &);
*/	
};

#endif
