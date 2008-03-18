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

#include "quickconnectdlg.h"

TQuickConnectDialog::TQuickConnectDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	AdrLabel->setText(tr("Input Hub Address below:"));
	IconLabel->setPixmap(QPixmap(":/images/actions/quick_connect.png"));
	
	#ifdef _DEBUG
		adrline->setText("localhost:4111");
	#else
		adrline->setText("");
	#endif
	
	connect(this,SIGNAL(accepted()),this,SLOT(okFunc()));
	
	show();
}

void TQuickConnectDialog::okFunc()
{
	emit con_pressed(adrline->text());
}
