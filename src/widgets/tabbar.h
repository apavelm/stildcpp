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

#ifndef _TABBAR_H_
#define _TABBAR_H_

#include <QtCore>
#include <QtGui>

#include "tabwidget.h"
#include "../config.h"

class TabWidget;

class TabBar : public QTabBar
{
	Q_OBJECT

public:
	TabBar(TabWidget * parent);
	~TabBar();

signals:
	void mouseDoubleClickTab(int tab);
	void tabDropped(int tab, TabBar *source);
	void contextMenu(QContextMenuEvent *event, int tab);

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
	
	void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	int findTabUnder(const QPoint &pos);
};

#endif /* _TABBAR_H_ */
