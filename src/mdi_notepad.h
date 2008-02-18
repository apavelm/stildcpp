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


#ifndef __MDI_NOTEPAD_H__
#define __MDI_NOTEPAD_H__

#include "config.h"
#include "stilutils.h"
#include "mdi_c.h"

#include <QTextStream>

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/Util.h"
//

#include "ui_notepad.h"

class NotePad : public MdiChild, private Ui::mdiNotePad
{
	Q_OBJECT
public:
	NotePad(QWidget *parent);
	~NotePad();
	void save();
private:
	QString curFile;
};

#endif // __MDI_NOTEPAD_H__
