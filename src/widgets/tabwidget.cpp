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

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
	tabBar_ = new TabBar(this);
	tabBar_->setElideMode(Qt::ElideRight);
	setTabBar( tabBar_ );
	buttons_ = new TabButtonWidget(this);

	int buttonwidth = qMax(tabBar()->style()->pixelMetric(QStyle::PM_TabBarScrollButtonWidth, 0, tabBar()), QApplication::globalStrut().width());
	buttons_->downButton()->setFixedWidth(buttonwidth);
	buttons_->downButton()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	buttons_->downButton()->setArrowType(Qt::DownArrow);
	buttons_->downButton()->setPopupMode(QToolButton::InstantPopup);
	
	menu_ = new QMenu(this);
	buttons_->downButton()->setMenu(menu_);
	
	connect(menu_, SIGNAL(aboutToShow()), SLOT(menu_aboutToShow()));
	connect(menu_, SIGNAL(triggered(QAction*)), SLOT(menu_triggered(QAction*)));

	buttons_->closeButton()->setFixedWidth(buttonwidth);
	buttons_->closeButton()->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
	buttons_->closeButton()->setText("x");

	connect( tabBar(), SIGNAL(mouseDoubleClickTab(int)), SLOT(mouseDoubleClickTab(int)));
	connect( tabBar(), SIGNAL( currentChanged(int)), SLOT(tab_currentChanged(int)));
	connect( tabBar(), SIGNAL( contextMenu(QContextMenuEvent*,int)), SLOT( tab_contextMenu(QContextMenuEvent*,int)));
	connect( buttons_->closeButton(), SIGNAL(clicked()), SIGNAL(closeButtonClicked()));
	connect(this, SIGNAL(closeButtonClicked()), this, SLOT(slotCloseTab()) );
	setCornerWidget(buttons_);
}

TabWidget::~TabWidget()
{
	delete menu_;
	delete buttons_;
}

void TabWidget::slotCloseTab(int n)
{
	if ( widget(n)) 
	{
		if ( !(widget(n)->close()) ) return;
			else delete widget(n);
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

void TabWidget::setCloseIcon(const QIcon& icon)
{
	buttons_->closeButton()->setIcon(icon);
	buttons_->closeButton()->setText("");
}

void TabWidget::mouseDoubleClickTab(int tab)
{
	emit mouseDoubleClickTab(widget(tab));
}

void TabWidget::tab_currentChanged(int tab)
{
	setCurrentIndex(tab);
	emit currentChanged(currentWidget());
}

void TabWidget::setTabText(QWidget* widget, const QString& label)
{
	int index = indexOf(widget);
	if (index != -1) tabBar()->setTabText(index, label);
} 

const QString TabWidget::tabText(int index) const
{
	return tabBar()->tabText(index);
}

void TabWidget::setTabToolTip(QWidget * widget, const QString & tooltip)
{
	int index = indexOf(widget);
	if (index != -1) tabBar()->setTabToolTip(index, tooltip);
}

const QString TabWidget::tabToolTip(int index) const
{
	return tabBar()->tabToolTip(index);
}

void TabWidget::setTabIcon(QWidget * widget, const QIcon & icon)
{
	int index = indexOf(widget);
	if (index != -1) tabBar()->setTabIcon(index, icon);
}

const QIcon TabWidget::tabIcon(int index) const
{
	return tabBar()->tabIcon(index);
}

void TabWidget::setTabTextColor(QWidget * widget, const QColor & color)
{
	int index = indexOf(widget);
	if (index != -1) tabBar()->setTabTextColor(index, color);
}

const QColor TabWidget::tabTextColor(int index) const
{
	return tabBar()->tabTextColor(index);
}

void TabWidget::menu_aboutToShow()
{
	menu_->clear();
	bool vis = false;
	for (int i = 0; i < tabBar()->count(); i++)
	{
		QRect r = tabBar()->tabRect(i);
		bool newvis = tabBar()->rect().contains(r);
		if (newvis != vis)
		{
			menu_->addSeparator();
			vis = newvis;
		}
		menu_->addAction(tabBar()->tabIcon(i), tabBar()->tabText(i))->setData(i+1);
	}
	
	menu_->addSeparator();
	QMenu * b = new QMenu();
	connect(b->addAction(/*QIcon(":/images/tabs/tabs_position_up.png"),*/ tr("Tabs Position Up")), SIGNAL(triggered()), this, SLOT(tabup()) );
	connect(b->addAction(/*QIcon(":/images/tabs/tabs_position_down.png"),*/ tr("Tabs Position Down")), SIGNAL(triggered()), this, SLOT(tabdown()) );
	connect(b->addAction(/*QIcon(":/images/tabs/tabs_position_up.png"),*/ tr("Tabs Position Left")), SIGNAL(triggered()), this, SLOT(tableft()) );
	connect(b->addAction(/*QIcon(":/images/tabs/tabs_position_down.png"),*/ tr("Tabs Position Right")), SIGNAL(triggered()), this, SLOT(tabright()) );
	QAction * y = menu_->addMenu(b);
	y->setText(tr("Position"));
	menu_->addSeparator();
	
	QMenu * a = new QMenu();
	connect(a->addAction(QIcon(":/images/tabs/close_tab.png"), tr("Close Current Tab")), SIGNAL(triggered()), this, SLOT(slotCloseTab()) );
	connect(a->addAction(QIcon(":/images/tabs/close_other_tabs.png"), tr("Close Other Tabs")), SIGNAL(triggered()), this, SLOT(slotCloseOtherTab()) );
	connect(a->addAction(QIcon(":/images/tabs/close_all_tabs.png"), tr("Close All Tabs")), SIGNAL(triggered()), this, SLOT(slotCloseAllTab()) );
	
	QAction * z = menu_->addMenu(a);
	z->setText(tr("Close..."));
	
	emit aboutToShowMenu(menu_);
}

void TabWidget::menu_triggered(QAction *act) {
	int idx = act->data().toInt();
	if (idx <= 0 || idx > tabBar()->count()) {
		// out of range
		// emit signal? 
	} else {
		setCurrentIndex(idx-1);
	}
}

void TabWidget::tab_contextMenu( QContextMenuEvent * event, int tab)
{
	emit tabContextMenu(tab, tabBar()->mapToGlobal(event->pos()), event);
}

/**
 * Show/hide the tab bar of this widget
 */
void TabWidget::setTabBarShown(bool shown) {
	if (shown && tabBar()->isHidden()) {
		tabBar()->show();
	} else if (!shown && !tabBar()->isHidden()) {
		tabBar()->hide();
	}
}

/**
 * Show/hide the menu and close buttons that appear next to the tab bar
 */
void TabWidget::setTabButtonsShown(bool shown) {
	if (shown && buttons_->downButton()->isHidden()) {
		buttons_->downButton()->show();
		buttons_->closeButton()->show();
	} else if (!shown && !buttons_->downButton()->isHidden()) {
		buttons_->downButton()->hide();
		buttons_->closeButton()->hide();
	}
}

//
