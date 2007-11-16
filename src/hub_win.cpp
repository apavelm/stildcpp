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

#include "hub_win.h"

using namespace std;
using namespace dcpp;

HubWindow::~HubWindow()
{
	FavoriteManager::getInstance()->removeListener(this);
	client->removeListener(this);
	client->disconnect(true);
	ClientManager::getInstance()->putClient(client);
}

HubWindow::HubWindow(QWidget *parent, const dcpp::tstring& url) : MdiChild(parent)
{	
	setupUi(this);
	type = 1;
	idText = StilUtils::TstrtoQ(url);
	
	// UserList in LeftSide
	/*
	if (_option_left_side) 
		{
			QWidget *w1 = splitter->widget(0);
			splitter->addWidget(w1);
		}
	*/
	
	
	setWindowTitle(tr("Hub: ")+StilUtils::TstrtoQ(url));
	editor->clear();
	setupeditor();
	
	client = ClientManager::getInstance()->getClient(dcpp::Text::fromT(url));
	client->addListener(this);
	client->connect();
	connect(SendBtn, SIGNAL(clicked()), this, SLOT( sendFunc() ) );
	
	FavoriteManager::getInstance()->addListener(this);
	
}

#include "textutil.h" //DELTE ME AFTER DEBUGING FUNCTION BELOW
void HubWindow::sendFunc()
{
	Iconset *ic = new Iconset();
	ic->load("/home/irq/stildcpp/src/images/emotions/iconset");
	ic->addToFactory();
	
	QString txt = "Testing smile :'( sequence $) ;) just do it! :) =) http://www.google.com rifjriofj/me gfgergergij";
	txt = TextUtil::plain2rich(txt);
	txt = TextUtil::linkify(txt);
	txt = TextUtil::emoticonify(txt);
	txt = TextUtil::legacyFormat(txt);
	txt = "["+editor->formatTimeStamp(QDateTime::currentDateTime())+"] " + txt;
	editor->appendText(txt);
}

void HubWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	//highlighter = new Highlighter(editor->document());
}

// HubWindow
