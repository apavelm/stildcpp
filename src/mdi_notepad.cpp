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

#include "mdi_notepad.h"

using namespace std;
using namespace dcpp;

NotePad::~NotePad()
{
	save();
}

NotePad::NotePad(QWidget *parent) : MdiChild(parent)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_NOTEPAD;
	idText = "Notepad";
	setTabText(tr("NotePad"));
	
	QFont f = textEdit->font();
	f.setPointSize(APPSETTING(i_NOTEPADFONTSIZE));
	textEdit->setFont(f);

	QString tmp(StilUtils::TstrtoQ(Text::toT(dcpp::Util::getNotepadFile())));

	QFile file(tmp);
	file.open(QFile::ReadOnly | QFile::Text);

	QTextStream in(&file);
	textEdit->setPlainText(in.readAll());
	
	curFile= tmp;
}

void NotePad::save()
{
	QFile file(curFile);
	file.open(QFile::WriteOnly | QFile::Text);

	QTextStream out(&file);
	out << textEdit->toPlainText();
}
