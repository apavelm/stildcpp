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
 
#include <QtGui>

#ifndef STIL_QDLTREEWIDGET_H
#define STIL_QDLTREEWIDGET_H

class QHubLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	QHubLineEdit(QWidget * parent = 0);
	void keyPressEvent(QKeyEvent *); 
signals:
	void sig_KeyPressEvent(QKeyEvent *);
};

class QDLTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:
	QDLTreeWidget(QWidget * parent = 0);
	QTreeWidgetItem* itemFromIndex(const QModelIndex&) const;
	QModelIndex indexFromItem(QTreeWidgetItem* it, int column = 0) const;
	void keyPressEvent(QKeyEvent *); 
signals:
	void sig_KeyPressEvent(QKeyEvent *);
};

#endif
