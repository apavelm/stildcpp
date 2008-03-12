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

#ifndef __CONFPUBHUB_H__
#define __CONFPUBHUB_H__

#include <QDialog>
#include "stilutils.h"

#include "ui_confpubhub.h"

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/FavoriteManager.h"
#include "client/StringTokenizer.h"

class ConfPubHubDlg : public QDialog, private Ui::dlg_confpubhub
{ 
	Q_OBJECT
public:
	ConfPubHubDlg(QWidget *parent);
private:
	void addHubList(const tstring& address);
private slots:
	void slotOK();
	void slotCancel();
	void slotAdd();
	void slotEdit();
	void slotEdit2(QListWidgetItem*);
	void slotDelete();
	
	void slotKeyPress(QKeyEvent*);
};

#endif // 
