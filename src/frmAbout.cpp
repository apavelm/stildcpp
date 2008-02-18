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

#include "frmAbout.h"

//

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	VerLabel->setText(APPVER);
	VerLabel_2->setText(VERSIONSTRING);
	label_4->setText(APPNAME);
	thxlabel->setText(SPECIALTHX);
	#ifndef _WIN32
	label_2->setPixmap(QPixmap(":/images/icon_linux.png"));
	#else
	label_2->setPixmap(QPixmap(":/images/icon.png"));
	#endif
	label_3->setPixmap(QPixmap(":/images/DCPlusPlus.png"));

	connect(Button, SIGNAL(clicked()), this, SLOT(accept()));
	show();
}

//
