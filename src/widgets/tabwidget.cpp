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

#include "tabwidget.h"


TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
	setMouseTracking( true );
	tabBar()->installEventFilter(this);
	tb=0;
	setTabShape(QTabWidget::Rounded);
	setElideMode(Qt::ElideRight);
	
	// Context Menu
	menu = new QMenu(this);
	
	// Position Submenu
	QMenu *a = menu->addMenu(tr("Position"));
	connect(a->addAction(QIcon(":/images/tab_up.png"), tr("Tabs Position Up")), SIGNAL(triggered()), this, SLOT(tabup()) );
	connect(a->addAction(QIcon(":/images/tab_down.png"), tr("Tabs Position Down")), SIGNAL(triggered()), this, SLOT(tabdown()) );

	connect(menu->addAction(QIcon(":/images/cross.png"), tr("Close Current Tab")), SIGNAL(triggered()), this, SLOT(slotCloseTab()) );
	connect(menu->addAction(QIcon(":/images/cross2.png"), tr("Close Other Tabs")), SIGNAL(triggered()), this, SLOT(slotCloseOtherTab()) );
	connect(menu->addAction(QIcon(":/images/cross_black.png"), tr("Close All Tabs")), SIGNAL(triggered()), this, SLOT(slotCloseAllTab()) );

	
	// Corner Close-cross Button
	crossButton = new QToolButton(this);
	crossButton->setIcon( QIcon(":/images/cross.png") );
	connect(crossButton, SIGNAL(clicked()), this, SLOT(slotCloseTab()) );
	crossButton->setGeometry(0,0,32,32);
	this->setCornerWidget(crossButton);
}

TabWidget::~TabWidget()
{
	delete menu;
	delete crossButton;
}

void TabWidget::tabdown()
{
	setOpt(1);
}

void TabWidget::tabup()
{
	setOpt(0);
}


void TabWidget::setOpt(int k)
{
	tb=k;
	if (k==0) setTabPosition(QTabWidget::North);
		else setTabPosition(QTabWidget::South);
}

bool TabWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (obj==tabBar())
	{
		if (event->type() == QEvent::MouseButtonRelease )
		{
		qApp->restoreOverrideCursor();
		}
		
		else 
		
		if (event->type() == QEvent::MouseButtonPress )
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
			int tmp2=-1;
			for (int i=0; i<tabBar()->count(); i++)
			{
				if ( tabBar()->tabRect(i).contains( mouseEvent->pos() ) )
				{
				tmp2 = i;
				break;
				}
			}
			if ( mouseEvent->button() == Qt::LeftButton )
				qApp->setOverrideCursor( Qt::ClosedHandCursor );
			if ( mouseEvent->button() == Qt::MidButton )
				if (tmp2>-1) slotCloseTab(tmp2);
			if ( mouseEvent->button() == Qt::RightButton )
				menu->exec(mouseEvent->globalPos());
		}
		
		else 
		
		if (event->type() == QEvent::MouseMove )
		{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		for (int i=0; i<tabBar()->count(); i++)
		{
			if ( tabBar()->tabRect(i).contains( mouseEvent->pos() ) )
			{
				if ( swapTabs(i, currentIndex()) )
				{
					setCurrentWidget(widget(i));
					update();
					int x;
					if ( !tabBar()->tabRect(i).contains( mouseEvent->pos() ) )
					{
						if ( tabBar()->tabRect(currentIndex()).x() < tabBar()->tabRect(i).x() )
							x = tabBar()->tabRect(i).x();
						else
							x = tabBar()->tabRect(i).x()+(tabBar()->tabRect(i).width()-(qAbs(tabBar()->tabRect(i).width()-tabBar()->tabRect(currentIndex()).width())));
						QPoint point =  QPoint(x, mouseEvent->pos().y() );
						point =  widget(i)->mapToGlobal( point );
						QCursor::setPos ( point.x(), QCursor::pos().y() );
					}
				break;
				}
			}
		}
		}
	}
	return QTabWidget::eventFilter( obj, event);
}

bool TabWidget::swapTabs(int index1, int index2)
{
	if (index1==index2) return false;
	int t1 = qMin(index1,index2);
	int t2 = qMax(index1,index2);
	
	index1=t1;
	index2=t2;

	QString name1 = tabBar()->tabText(index1);
	QString name2 = tabBar()->tabText(index2);

	QWidget *tab1 = widget(index1);
	QWidget *tab2 = widget(index2);

	removeTab(index2);
	removeTab(index1);

	insertTab(index1,tab2,name2);
	insertTab(index2,tab1,name1);
	return true;
}

void TabWidget::slotCloseTab(int n)
{
	if ( this->widget(n)) 
	{
		if ( !(this->widget(n)->close()) ) return;
			else delete this->widget(n);
	}
}

void TabWidget::slotCloseTab()
{
	slotCloseTab(currentIndex());
}

void TabWidget::slotCloseAllTab()
{
	if (count()==0) return;
	for (int i=count(); i>0; i--) slotCloseTab(0);
}

void TabWidget::slotCloseOtherTab()
{
	if (count()==1) return;
	for (int i=count(); i>0; i--) 
		if ( currentIndex()!=0 )
			slotCloseTab(0);
		else
			slotCloseTab(1);
}

void TabWidget::setCrossButton(bool activate)
{
	if (!activate) crossButton->hide();
	else crossButton->show();
}

void TabWidget::setTextChange(int index, const QString & txt)
{
	if ( (index<0)||(index>=tabBar()->count())||(txt.isEmpty()) ) return;
	int ix = currentIndex();
	QWidget *tab1 = widget(index);

	removeTab(index);
	insertTab(index,tab1,txt);
	setCurrentIndex(ix);
}

void TabWidget::slotTextChange(int index, const QString & txt)
{
	setTextChange(index, txt);
}

void TabWidget::setTextColor(int index, QColor & c)
{
	tabBar()->setTabTextColor(index, c);
}

void TabWidget::setTabToolTip(int index, const QString & txt)
{
	tabBar()->setTabToolTip(index, txt);
}
	
void TabWidget::slotTextColor(int index, QColor & c)
{
	setTextColor(index, c);
}

void TabWidget::slotTabToolTip(int index, const QString & txt)
{
	setTabToolTip(index, txt);
}

void TabWidget::setTabIcon(int index, const QIcon & icn)
{
	tabBar()->setTabIcon(index, icn);
}

void TabWidget::slotTabIcon(int index, const QIcon & icn)
{
	setTabIcon(index, icn);
}

//
