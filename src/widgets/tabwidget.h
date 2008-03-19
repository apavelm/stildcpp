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
 
#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtCore>
#include <QtGui>

#include "tabbar.h"

#include "ui_tabbuttonwidget.h"

class TabButtonWidget : public QWidget, private Ui::uiTabButtonWidget
{
	Q_OBJECT
public:
	TabButtonWidget(QWidget * parent) : QWidget(parent) { setupUi(this); }
	~TabButtonWidget() {}
	QToolButton * closeButton() { return closeButton_; }
	QToolButton * downButton() { return downButton_; }
};

class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	TabWidget(QWidget *parent = 0);
	~TabWidget();
	
	int addTab(QWidget * w) { return QTabWidget::addTab(w, QString("unnamed")); }

	void setTabText(QWidget *, const QString &);
	const QString tabText(int) const;
	void setTabToolTip(QWidget *, const QString &);
	const QString tabToolTip(int) const;
	void setTabIcon(QWidget *, const QIcon &);
	const QIcon tabIcon(int) const;
	void setTabTextColor(QWidget *, const QColor &);
	const QColor tabTextColor(int) const;
	
	
	void setCloseIcon(const QIcon &);

	void setTabBarShown(bool shown);     // default shown
	void setTabButtonsShown(bool shown); // default shown

signals:
	void mouseDoubleClickTab(QWidget *tab);
	void currentChanged(QWidget *selected);
	void closeButtonClicked();
	void aboutToShowMenu(QMenu *);

	// context menu on the blank space will have tab==-1
	void tabContextMenu(int tab, QPoint pos, QContextMenuEvent *event);

private slots:
	void mouseDoubleClickTab(int tab);
	void tab_currentChanged(int tab);
	void tab_contextMenu(QContextMenuEvent *event, int tab);
	void menu_aboutToShow();
	void menu_triggered(QAction *act);
	
	void tabup() { setTabPosition(QTabWidget::North); }
	void tabdown() { setTabPosition(QTabWidget::South); }
	void tableft() { setTabPosition(QTabWidget::West); }
	void tabright() { setTabPosition(QTabWidget::East); }

private:
	QTabBar *tabBar_;
	TabButtonWidget * buttons_;
	QMenu *menu_;
public slots:
	void slotCloseTab();
	void slotCloseTab(int n);
	void slotCloseAllTab();
	void slotCloseOtherTab();
};

#endif
