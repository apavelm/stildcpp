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

#include "tthdial.h"

TthDialog::TthDialog(QWidget *parent,const QString &a,const QString &b,const QString &c) : QDialog(parent)
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	connect(OKBtn, SIGNAL(clicked()), this, SLOT(accept()));
	setWindowIcon(QIcon(":/images/magnet.png"));
	icolabel_file->setPixmap(QPixmap(":/images/tth_file.png"));
	icolabel_TTH->setPixmap(QPixmap(":/images/tth_tth.png"));
	icolabel_magnet->setPixmap(QPixmap(":/images/magnet.png"));
	connect(cp_file,SIGNAL(clicked()),filebox,SLOT(selectAll()));
	connect(cp_tth,SIGNAL(clicked()),tthbox,SLOT(selectAll()));
	connect(cp_magnet,SIGNAL(clicked()),magnetbox,SLOT(selectAll()));
	connect(cp_file,SIGNAL(clicked()),filebox,SLOT(copy()));
	connect(cp_tth,SIGNAL(clicked()),tthbox,SLOT(copy()));
	connect(cp_magnet,SIGNAL(clicked()),magnetbox,SLOT(copy()));
	filebox->setText(a);
	tthbox->setText(b);
	magnetbox->setText(c);
	show();
}

// TTH DIALOG
