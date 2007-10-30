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

#ifndef __PM_WIN_H__
#define __PM_WIN_H__

#include <QFile>

#include "highlighter.h"
#include "mdi_c.h"

#include "ui_PMWindow.h"

class PMWindow : public MdiChild, private Ui::mdiPMwin
{
	Q_OBJECT
private:
	Highlighter *highlighter;
public:
	PMWindow(QWidget *parent, const QString &nick);
	void setupeditor();
private slots:
	void send_pm_msg();
};

// PRIVATE MESSAGE WINDOW

#endif // __PM_WIN_H__
