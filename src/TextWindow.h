/***************************************************************************
 *   Copyright (C) 2007, 2008 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *   Copyright (C) 2007, 2008 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)                  *
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


#ifndef __TEXTWINDOW_H__
#define __TEXTWINDOW_H__

#include "config.h"
#include "stilutils.h"
#include "mdi_c.h"

#include <QTextStream>

#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Util.h"
#include "client/File.h"
#include "client/Text.h"

#include "ui_TextWindow.h"

class TextWindow : public MdiChild, private Ui::mdiTextWindow
{
	Q_OBJECT
public:
	TextWindow(QWidget *parent, const string& fileName);
};

#endif // __TEXTWINDOW_H__
