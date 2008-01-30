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

#include "mdi_c.h"

MdiChild::MdiChild(QWidget *parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	type = StilUtils::WIN_TYPE_NONE;
	idText="";
	prnt = (TabWidget*) parent;
}

MdiChild::~MdiChild()
{
	prnt =0;
}

void MdiChild::setTabText(const QString &txt)
{
	int idx = prnt->indexOf(this);
	prnt->setTextChange(idx, txt);
}

void MdiChild::slot_setTabText(const QString &txt)
{
	setTabText(txt);
}

void MdiChild::setTabTextColor(QColor & c)
{
	int idx = prnt->indexOf(this);
	prnt->setTextColor(idx, c);
}

void MdiChild::slot_setTabTextColor(QColor & c)
{
	setTabTextColor(c);
}

void MdiChild::setTabToolTip(const QString &txt)
{
	int idx = prnt->indexOf(this);
	prnt->setTabToolTip(idx, txt);
}

void MdiChild::slot_setTabToolTip(const QString &txt)
{
	setTabToolTip(txt);
}

void MdiChild::setTabIcon(const QIcon & icn)
{
	int idx = prnt->indexOf(this);
	prnt->setTabIcon(idx, icn);
}

void MdiChild::slot_setTabIcon(const QIcon & icn)
{
	setTabIcon(icn);
}

//
