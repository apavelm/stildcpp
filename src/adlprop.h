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

#ifndef __ADLPROP_H__
#define __ADLPROP_H__

#include "mdi_c.h"
#include "config.h"
#include "defs.h"
#include "stilutils.h"

#include "ui_adl_prop.h"

#include "client/ADLSearch.h"

#include <QtGui>
#include <QtCore>

using namespace std;
using namespace dcpp;

class ADLPropertiesDialog : public QDialog, private Ui::dlg_adl_prop
{
	Q_OBJECT
public:
	ADLPropertiesDialog(QWidget *parent, ADLSearch * _search);
	~ADLPropertiesDialog();
private:
	ADLSearch * src;
private slots:
	void slotAccept();
};

#endif
