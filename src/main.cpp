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
#include "mainwindowimpl.h"
#include "frmAbout.h"
#include "mdi_c.h"
#include "defs.h"
#include "config.h"
//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"

using namespace std;
using namespace dcpp;
//

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	
	AppSettings::AppSettingsMgr::newInstance();
	dcpp::startup(NULL,NULL);
	
	if (AppSettings::AppSettingsMgr::getInstance()->load()!=0) fprintf(stdout, "\nConfiguration file not found.\nUsing default values...\n");

	
	QSplashScreen *splash;
	Qt::Alignment alignBC = Qt::AlignBottom | Qt::AlignHCenter;
	
	if (APPSETTING(i_SHOWSPLASH))
	{
		splash = new QSplashScreen(QPixmap(":/images/splash.png"));
		splash->show();
		splash->showMessage(QObject::tr("Starting..."), alignBC, Qt::black);
	}
	
	// Translate qt
	QTranslator *qtTranslator = new QTranslator(&app);
	QString ql = QLocale::system().name();
	qtTranslator->load("qt_" + ql, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(qtTranslator);
	
	QTranslator *translator = new QTranslator(&app);
	translator->load(QString("stildcpp_") + ql, ":/lang/", QString(), ".qm");
	app.installTranslator(translator); 
	
	
	if (APPSETTING(i_USETRAY))
	{
		if (APPSETTING(i_SHOWSPLASH)) splash->showMessage(QObject::tr("Creating tray icon..."), alignBC, Qt::black);
		if (!QSystemTrayIcon::isSystemTrayAvailable()) 
			{
			QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
			fprintf(stdout,"\nSystem tray wasn't detected!\nIf You want to run StilDC++ without tray icon:\n change \"UseTray\" visriable to \"0\" in stildcpp.xml file, which located in \"YourHomeDir/.stildcpp/\"");
			return 1;
			}
	}
	
	MainWindowImpl win;

	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) ); 
	if (APPSETTING(i_SHOWSPLASH)) 
		{
			splash->finish(&win);
			delete splash;
		}
	return app.exec();
}
