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

#include "SearchWin.h"


SearchWindow::SearchWindow(QWidget *parent, const QString &what) : MdiChild(parent)
{	
	setupUi(this);
	type = 3;
	idText  = what;
	setWindowTitle(tr("Search for: ")+what);
	connect(searchButton, SIGNAL(pressed()), this, SLOT(SearchBtn()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchBtn()));
}

void SearchWindow::SearchBtn()
{	
	searchButton->setDefault(true);
	if (searchCombo->currentIndex()>=0)
	{
		idText  = searchCombo->itemText(searchCombo->currentIndex());	
		setTabText(tr("Search for: ")+idText);
		//SEARCH
	}
}
