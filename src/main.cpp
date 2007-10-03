#include <QApplication>
#include <QtGui>
#include <QSplashScreen>
#include "mainwindowimpl.h"
#include "frmAbout.h"
#include "mdi_c.h"
//

int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	
	QSplashScreen *splash = new QSplashScreen(QPixmap(":/images/splash.png"));
	splash->show();
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop; splash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);
	MainWindowImpl win;
//	splash->showMessage(QObject::tr("Loading Modules..."), topRight, Qt::white);
//	loadModules();
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
