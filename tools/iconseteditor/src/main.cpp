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

#include <QApplication>
#include <QtGui>
#include <QSplashScreen>
#include <QTranslator>
#include <QLocale>
#include <QDir>
#include "mainwinimpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	
	QSplashScreen *splash;
	Qt::Alignment alignBC = Qt::AlignBottom | Qt::AlignHCenter;
	
	splash = new QSplashScreen(QPixmap(":/images/splash_editor.png"));
	splash->show();
	splash->showMessage(QObject::tr("Starting..."), alignBC, Qt::black);

	
	MainWindowImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	splash->finish(&win);
	delete splash;
	return app.exec();
}
