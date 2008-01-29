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

#ifndef __MDI_C_H__
#define __MDI_C_H__

#include <QDialog>
#include <QString>
#include <QAction>
#include <QWidget>

#include "widgets/tabwidget.h"

//

class MdiChild : public QDialog
{
	Q_OBJECT
protected:
	TabWidget *prnt;
public:
	MdiChild(QWidget *parent);
	~MdiChild();
	
	int type;
	/*  0 - MdiChild (NONE)
		1 - Hub
		2 - Private Chat
		3 - Search
		4 - Favorite Hub List
		5 - FileList
		6 - DL Queue
		7 - Favorite Users
		8 - Ignored Users
		9 - Public Hubs
		10 - Finished DL
		11 - Finished UL
		12 - ADL Search
		13 - Search Spy
		14 - NotePad
		15 - DownLoads Frame
		16 - System Log
	*/
	QString idText;
	void setTabText(const QString &);
	void setTabTextColor(QColor &);
	void setTabToolTip(const QString &);
	void setTabIcon(const QIcon &);

public slots:
	void slot_setTabText(const QString &);
	void slot_setTabTextColor(QColor &);
	void slot_setTabToolTip(const QString &);
	void slot_setTabIcon(const QIcon &);
};

#endif // __MDI_C_H__
