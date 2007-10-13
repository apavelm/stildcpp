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
	type=0;
	idText=tr("");
}

MdiChild::~MdiChild()
{
	emit actionReleased(action);
}

PMWindow::PMWindow(QWidget *parent, const QString &nick) : MdiChild(parent)
{	
	setupUi(this);
	type = 2;
	idText  = nick;
	setWindowTitle(tr("Private Chat with: ")+nick);
}

HubWindow::HubWindow(QWidget *parent, const QString &hubaddr) : MdiChild(parent)
{	
	setupUi(this);
	type = 1;
	idText  = hubaddr;
	setWindowTitle(tr("Hub: ")+hubaddr);
	textEdit->clear();
	setupeditor();
}

void HubWindow::setupeditor()
{
	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);

	textEdit->setFont(font);

	highlighter = new Highlighter(textEdit->document());
}

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
		action->setText(tr("Search for: ")+idText);
		setWindowTitle(tr("Search for: ")+idText);		
		//SEARCH
	}
}

FavoriteHubListWindow::FavoriteHubListWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = 4;
	idText  = tr("");
	setWindowTitle(tr("Favorite Hubs"));
}

//
