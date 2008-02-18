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

#include <QTabWidget>
#include <QMouseEvent>
#include <QTabBar>
#include <QMenu>
#include <QApplication>
#include <QToolButton>

class TabWidget : public QTabWidget
{
	Q_OBJECT
public:
	TabWidget(QWidget *parent);
	~TabWidget();
	void setOpt(int k);
	int getOpt() const { return tb; }
	void setTextChange(int, const QString &);
	void setTextColor(int, QColor &);
	void setTabToolTip(int, const QString &);
	void setTabIcon(int, const QIcon &);
	
protected:
	bool eventFilter(QObject *obj, QEvent *event);
private:
	int tb;
	bool swapTabs(int index1, int index2);
	QToolButton *crossButton;
	QMenu *menu;
	void setCrossButton(bool activate);
private slots:
	void tabdown();
	void tabup();
public slots:
	void slotCloseTab();
	void slotCloseTab(int n);
	void slotCloseOtherTab();
	void slotCloseAllTab();
	void slotTextChange(int, const QString &);
	void slotTextColor(int, QColor &);
	void slotTabToolTip(int, const QString &);
	void slotTabIcon(int, const QIcon &);
};

#endif
