/***************************************************************************
 *   Copyright (C) 2007, 2008 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *   Copyright (C) 2007, 2008 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)                  *
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

#include "TextWindow.h"

using namespace std;
using namespace dcpp;

static const size_t MAX_TEXT_LEN = 64*1024;

TextWindow::TextWindow(QWidget *parent, const string& fileName) : MdiChild(parent)
{
	setupUi(this);
	type = StilUtils::WIN_TYPE_TEXT_WINDOW;
	idText = StilUtils::TstrtoQ(Text::toT(fileName));

	QFont f = textEdit->font();
	f.setPointSize(APPSETTING(i_SYSLOGFONTSIZE));
	textEdit->setFont(f);

	/*
	QString tmp(StilUtils::TstrtoQ(Text::toT(fileName)));
	QFile file(tmp);
	file.open(QFile::ReadOnly | QFile::Text);
	*/

	try {
//		textEdit->setPlainText(StilUtils::TstrtoQ(Text::toT(Text::toDOS(File(fileName, File::READ, File::OPEN).read(MAX_TEXT_LEN)))));
		textEdit->setPlainText(StilUtils::TstrtoQ(Text::toT(File(fileName, File::READ, File::OPEN).read(MAX_TEXT_LEN))));
	} catch(const FileException& e)
	{
		textEdit->setPlainText(StilUtils::TstrtoQ(Text::toT(e.getError())));
	}

//	QTextStream in(&file);

//	textEdit->setPlainText(StilUtils::TstrtoQ(Text::toT(Text::toDOS(Text::fromT(StilUtils::QtoTstr(in.read(MAX_TEXT_LEN)))))));
//	textEdit->setPlainText(in.read(MAX_TEXT_LEN));
}
