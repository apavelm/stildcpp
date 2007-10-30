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

#ifndef __HUB_WIN_H__
#define __HUB_WIN_H__

#include <QFile>
#include <QUrl>

#include "highlighter.h"
#include "mdi_c.h"

#include "ui_HUBWindow.h"

class HubWindow : public MdiChild, private Ui::mdiHUBwin
{
	Q_OBJECT
private:
	Highlighter *highlighter;
public:
	HubWindow(QWidget *parent, const QString &hubaddr);
	void setupeditor();
private slots:
	void send_msg();
};

#endif // __HUB_WIN_H__
