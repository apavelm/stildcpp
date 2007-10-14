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
#include "mainwindowimpl.h"
#include "frmAbout.h"
#include "mdi_c.h"
#include "defs.h"

//

//

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	
	QSplashScreen *splash = new QSplashScreen(QPixmap(":/images/splash.png"));
	splash->show();
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop; splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);
	MainWindowImpl win;
	
	
	splash->showMessage(QObject::tr("Loading Hash Tree..."), topRight, Qt::white);
//	try {
//		std::string module = File(Text::fromT(app.getModuleFileName()), File::READ, File::OPEN).read();
//		TigerTree tth(TigerTree::calcBlockSize(module.size(), 1));
//		tth.update(module.data(), module.size());
//		tth.finalize();
//		WinUtil::tth = Text::toT(tth.getRoot().toBase32());
//	} catch(const FileException&) {
//		dcdebug("Failed reading exe\n");
//	}
	

	splash->showMessage(QObject::tr("Creating tray icon..."), topRight, Qt::white);
//	establishConnections();
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
		return 1;
	}
//	win.show();
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) ); 
	splash->finish(&win);
	delete splash;
	return app.exec();
}
