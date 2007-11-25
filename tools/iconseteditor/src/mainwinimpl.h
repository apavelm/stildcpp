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

#ifndef __MAINWINIMPL_H__
#define __MAINWINIMPL_H__

#include <QDialog>
#include <QtGui>
#include "../../src/zstream.h"
#include "../../src/widgets/tabwidget.h"

#include "compose_dlg.h"

#include "ui_mainwin.h"

class MainWindowImpl : public QMainWindow, public Ui::MainWindow 
{
	Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindowImpl();
private:
	TabWidget * m_tabwin;
private slots:
	void slotCurrentTabChanged(int index);
	void funcNewIconSet();
	void funcFromDir();
};

#endif // __MAINWINIMPL_H__
