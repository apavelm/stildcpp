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

#include "tabbar.h"

TabBar::TabBar(TabWidget *parent) : QTabBar(parent)
{
	setAcceptDrops(true);
}

TabBar::~TabBar()
{
}

void TabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	const QPoint pos = event->pos();
	int tab = findTabUnder(pos);
	if (tab >= 0 && tab < count())
	{
		emit mouseDoubleClickTab(tab);
	}
}

int TabBar::findTabUnder(const QPoint &pos)
{
	for (int i = 0; i < count(); i++)
	{
		if (tabRect(i).contains(pos)) return i;
	}
	return -1;
} 

void TabBar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
	{
		TabWidget * w = qobject_cast<TabWidget*>(parent());
		w->slotCloseTab(findTabUnder(event->pos()));
		return;
	}
	else
	if (event->button() == Qt::LeftButton)
	{
		int dragtab = findTabUnder(event->pos());
		if (dragtab == -1) return;
		
		TabWidget * w = qobject_cast<TabWidget*>(parent());
		w->setCurrentIndex(dragtab);

		QByteArray itemData;
		itemData.setNum(dragtab);

		QMimeData *mimeData = new QMimeData;
		mimeData->setData("application/stildcpp-drag-tab", itemData);

		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
	
		QPixmap px = tabIcon(dragtab).pixmap(QSize(APPSETTING(i_TOOLBARICONSIZE),APPSETTING(i_TOOLBARICONSIZE)));
		drag->setPixmap(px);
		
		if (drag->exec(Qt::MoveAction) == Qt::MoveAction )
		{
			// OK
		}
		else
		{
			// FAIL ( never runs )
		}
	}
	
	QTabBar::mousePressEvent(event);
}

void TabBar::dragEnterEvent(QDragEnterEvent *event)
{
	if (!(event->mouseButtons() & Qt::LeftButton))
	{
		event->ignore();
		return;
	}
	
	if (event->mimeData()->hasFormat("application/stildcpp-drag-tab"))
	{
		//int droptab = findTabUnder(event->pos());
		if (event->source() == this)
		{
			// Always run
			event->setDropAction(Qt::MoveAction);
			event->accept();
		} 
		else 
		{
			// not interesting. Must me deleted
			event->acceptProposedAction();
		}
	}
	else 
	{
		event->ignore();
	}
}

void TabBar::dragMoveEvent(QDragMoveEvent *event)
{
	if (!(event->mouseButtons() & Qt::LeftButton))
	{
		event->ignore();
		return;
	}
	
	if (event->mimeData()->hasFormat("application/stildcpp-drag-tab"))
	{
		if (event->source() == this)
		{
			// always run continiously
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else
		{
			//pretendent to remove
			event->acceptProposedAction();
		}
	}
	else
	{
		event->ignore();
	}
}

void TabBar::dropEvent(QDropEvent *event)
{
	int droptab = findTabUnder(event->pos());
	// Why droptab NEVER sets to -1 ???
	
	if (event->mimeData()->hasFormat("application/stildcpp-drag-tab"))
	{
		QByteArray itemData = event->mimeData()->data("application/stildcpp-drag-tab");
		int dragtab = itemData.toInt();
		
		// swap tabs
		if (dragtab != droptab)
		{
			TabWidget * w = qobject_cast<TabWidget*>(parent());
			int idx1 = qMin(dragtab, droptab);
			int idx2 = qMax(dragtab, droptab);

			QString name1, name2;
			QIcon icon1, icon2;
			QWidget *tab1, *tab2;
			
			if (idx1 != -1)
			{
				name1 = tabText(idx1);
				icon1 = tabIcon(idx1);
				tab1 = w->widget(idx1);
			}
			
			name2 = tabText(idx2);
			icon2 = tabIcon(idx2);
			tab2 = w->widget(idx2);
			
			w->removeTab(idx2);
			if (idx1 != -1) w->removeTab(idx1);
			w->insertTab(idx1, tab2, icon2, name2);
			if (idx1 != -1) w->insertTab(idx2, tab1, icon1, name1);
		}
		
		if (event->source() == this)
		{
			// always run
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else
		{
			// pretendent to delete
			event->acceptProposedAction();
		}
	}
	else
	{
		event->ignore();
	}
}

void TabBar::contextMenuEvent(QContextMenuEvent *event)
{
	event->accept();
	emit contextMenu(event, findTabUnder(event->pos()));
}

void TabBar::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	int newIndex = currentIndex() - numSteps;

	while (newIndex < 0) newIndex += count();
	
	newIndex = newIndex % count();
	setCurrentIndex(newIndex);	
	
	event->accept();
}
