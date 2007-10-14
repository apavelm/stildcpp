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

#include "mainwindowimpl.h"
using namespace std;
//
MainWindowImpl::~MainWindowImpl()
{
	thrdGetTTh.stop();
	delete closeAct;
	delete closeAllAct;
	delete tileAct;
	delete cascadeAct;
	delete nextAct;
	delete previousAct;
	delete separatorAct;
	delete minimizeAction;
	delete maximizeAction;
	delete restoreAction;
	delete quitAction;
	delete trayIcon;
	delete trayIconMenu;
	delete shareStatusLbl;		
	delete mdiArea;
	delete windowMapper;
	thrdGetTTh.wait();
}

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	setupUi(this);
	
	mdiArea = new QMdiArea;
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),this, SLOT(updateWindowMenu()));
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget *)),this, SLOT(setActiveSubWindow(QWidget *)));
	
	createActions();
	createToolBars();
	createTrayIcon();

	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));	
	
	setToolTip(tr("StilDC++"));
	setWindowTitle(tr("StilDC++"));
	
	shareStatusLbl = new QLabel;
	setShareSize(tr("Total shared: 0b"));
	statusBar()->addPermanentWidget(shareStatusLbl);
	
	setIcon(0);
	setWindowIcon(QIcon(":/images/icon.png"));
	trayIcon->show();

	show(); //insert "if" startHidden
}

void MainWindowImpl::setVisible(bool visible)
{
	minimizeAction->setEnabled(visible);
	maximizeAction->setEnabled(!isMaximized());
	restoreAction->setEnabled(isMaximized() || !visible);
	QWidget::setVisible(visible);
}

void MainWindowImpl::closeEvent(QCloseEvent *event)
{
	mdiArea->closeAllSubWindows();
	if (trayIcon->isVisible()) {
	QMessageBox::information(this, tr("Systray"),tr("The program will keep running in the system tray. To terminate the program, choose <b>Quit</b> in the context menu of the system tray entry."));
	hide();
	event->ignore();
	}
}

void MainWindowImpl::setToolTip(const QString & title)
{
	trayIcon->setToolTip(title);
}

void MainWindowImpl::setIcon(int index)
{
	QIcon icon = QIcon(":/images/icon.png");
	switch (index) {
		case 1: icon = QIcon(":/images/icon_mail.png"); break;
		case 2:	icon = QIcon(":/images/icon_error.png"); break;
		default: ;
	};
	trayIcon->setIcon(icon);
}

void MainWindowImpl::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
		case QSystemTrayIcon::Trigger:
		{	// Left Mouse Button
			if (this->isHidden()) this->show(); else this->hide();
		}; break;
//		case QSystemTrayIcon::DoubleClick:
//			showMessage("Title","Text",0,2000);
	case QSystemTrayIcon::MiddleClick:
			statusMessage("Ready"); break;
	default:
	;
					}
}

void MainWindowImpl::showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint)
{
// Show message in Tray Icon
QSystemTrayIcon::MessageIcon icon;
// icon = { NoIcon, Information, Warning, Critical }
	switch (type) {
		case 0: icon = QSystemTrayIcon::NoIcon; break;
		case 1: icon = QSystemTrayIcon::Information; break;
		case 2: icon = QSystemTrayIcon::Warning; break;
		case 3: icon = QSystemTrayIcon::Critical; break;
		default: icon = QSystemTrayIcon::Information;
	}
	trayIcon->showMessage(title, message, icon, millisecondsTimeoutHint);
}

void MainWindowImpl::messageClicked()
{
// OnClick on InformationMessages appeared in TrayIcon
	QMessageBox::information(0, tr("Systray"), tr("Sorry, I already gave what help I could.\nMaybe you should try asking a human?"));
}

void MainWindowImpl::createActions()
{	
	minimizeAction = new QAction(tr("Mi&nimize"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

	maximizeAction = new QAction(tr("Ma&ximize"), this);
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

	restoreAction = new QAction(tr("&Restore"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	
	
	closeAct = new QAction(tr("Cl&ose"), this);
	closeAct->setShortcut(tr("Ctrl+F4"));
	closeAct->setStatusTip(tr("Close the active window"));
	connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

	closeAllAct = new QAction(tr("Close &All"), this);
	closeAllAct->setStatusTip(tr("Close all the windows"));
	connect(closeAllAct, SIGNAL(triggered()),mdiArea, SLOT(closeAllSubWindows()));

	tileAct = new QAction(tr("&Tile"), this);
	tileAct->setStatusTip(tr("Tile the windows"));
	connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

	cascadeAct = new QAction(tr("&Cascade"), this);
	cascadeAct->setStatusTip(tr("Cascade the windows"));
	connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

	nextAct = new QAction(tr("Ne&xt"), this);
	nextAct->setStatusTip(tr("Move the focus to the next window"));
	connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

	previousAct = new QAction(tr("Pre&vious"), this);
	previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

	separatorAct = new QAction(this);
	separatorAct->setSeparator(true);
	

	connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));	
	connect(aboutqtact, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(actionTransfers, SIGNAL(triggered()), this, SLOT(transcheck()));
	connect(actionStatusBar, SIGNAL(triggered()), this, SLOT(statusbarcheck()));
	connect(actionToolBar, SIGNAL(triggered()), this, SLOT(toolbarcheck()));
	connect(actionTabBar, SIGNAL(triggered()), this, SLOT(winbarcheck()));
	
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(PreferencesFunc()));
		
	connect(menuWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
	connect(actionDonate, SIGNAL(triggered()), this, SLOT(DonateFunc()));
	connect(actionHomepage, SIGNAL(triggered()), this, SLOT(HomepageFunc()));
	connect(actionSearch, SIGNAL(triggered()), this, SLOT(SearchFunc()));
	connect(actionGet_TTH_for_file, SIGNAL(triggered()), this, SLOT(GetTTHFunc()));
	
	connect(actionFavorite_Hubs, SIGNAL(triggered()), this, SLOT(FavHubListFunc()));
	connect(actionQuick_Connect, SIGNAL(triggered()), this, SLOT(fQuickConFunc()));
	
	connect(&thrdGetTTh, SIGNAL(ready()), this, SLOT(show_tthFunc()));
	
}

void MainWindowImpl::createTrayIcon()
{
	trayIconMenu = new QMenu(this);	
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}

void MainWindowImpl::createToolBars()
{
	toolBar->addAction(actionExit);
}

void MainWindowImpl::statusMessage(const QString & mes)
{
	statusbar->showMessage(mes);
}

void MainWindowImpl::About()
{
	new AboutDialog(this);
}

void MainWindowImpl::PreferencesFunc()
{
	new PreferencesDialog(this);	
}

void MainWindowImpl::updateWindowMenu()
{
	menuWindow->clear();
	menuWindow->addAction(closeAct);
	menuWindow->addAction(closeAllAct);
	menuWindow->addSeparator();
	menuWindow->addAction(tileAct);
	menuWindow->addAction(cascadeAct);
	menuWindow->addSeparator();
	menuWindow->addAction(nextAct);
	menuWindow->addAction(previousAct);
	menuWindow->addSeparator();	
	
	bool hasMdiChild = (activeMdiChild() != 0);
	closeAct->setEnabled(hasMdiChild);
	closeAllAct->setEnabled(hasMdiChild);
	tileAct->setEnabled(hasMdiChild);
	cascadeAct->setEnabled(hasMdiChild);
	nextAct->setEnabled(hasMdiChild);
	previousAct->setEnabled(hasMdiChild);
	separatorAct->setVisible(hasMdiChild);
	
	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();

	for (int i = 0; i < windows.size(); ++i) 
	{
		MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());				
		child->action->setChecked(child == activeMdiChild());
		menuWindow->addAction(child->action);
		windowMapper->setMapping(child->action, windows.at(i));
	}
}

MdiChild *MainWindowImpl::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}


QMdiSubWindow *MainWindowImpl::findMdiChild(const int id, QString & idtext, QAction *act)
{
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) 
	{
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (mdiChild->type == id) 
		{
			if (mdiChild->idText == idtext)
			{
				if (act!=NULL)
				{
					if (mdiChild->action==act) return window;
				} else return window;
			}
		}
	}
	return NULL;
}

QMdiSubWindow *MainWindowImpl::findMdiChild(const int id, QString & idtext)
{
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) 
	{
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (mdiChild->type == id) 
		{
			if (mdiChild->idText == idtext) return window;
		}
	}
	return NULL;
}

QMdiSubWindow *MainWindowImpl::findMdiChild(const int id)
{
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) 
	{
		MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
		if (mdiChild->type == id) return window;		
	}
	return NULL;
}

void MainWindowImpl::setActiveSubWindow(QWidget *window)
{
	if (!window) return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindowImpl::DonateFunc()
{
	PMWindow *child = new PMWindow(this,tr("Vasya"));
	mdiArea->addSubWindow(child);

		child->action  = menuWindow->addAction(tr("PM: ")+child->idText);
		child->action->setCheckable(true);
		connect(child->action, SIGNAL(triggered()), windowMapper, SLOT(map()));		
		connect(child, SIGNAL(actionReleased(QAction *)), this, SLOT(slotclosemdi(QAction *)));
		WindowToolBar->addAction(child->action);
	
	child->show();
}

void MainWindowImpl::HomepageFunc()
{
	QUrl url = QUrl(tr("http://code.google.com/p/stildcpp/"));
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::OpenHub(QString &adr, int port)
{
	HubWindow *child = new HubWindow(this,adr);
	mdiArea->addSubWindow(child);
	child->action  = menuWindow->addAction(tr("HUB: ")+child->idText);
	
		child->action->setCheckable(true);
		connect(child->action, SIGNAL(triggered()), windowMapper, SLOT(map()));		
		connect(child, SIGNAL(actionReleased(QAction *)), this, SLOT(slotclosemdi(QAction *)));
		WindowToolBar->addAction(child->action);
	
	child->show();
}

void MainWindowImpl::SearchFunc()
{
	SearchWindow *child = new SearchWindow(this,tr(""));
	mdiArea->addSubWindow(child);
	child->action  = menuWindow->addAction(tr("Search: ")+child->idText);
	
		child->action->setCheckable(true);
		connect(child->action, SIGNAL(triggered()), windowMapper, SLOT(map()));		
		connect(child, SIGNAL(actionReleased(QAction *)), this, SLOT(slotclosemdi(QAction *)));
		WindowToolBar->addAction(child->action);
	
	child->show();
}

void MainWindowImpl::FavHubListFunc()
{
	QMdiSubWindow *w = findMdiChild(4);
	if (w!=NULL) setActiveSubWindow(w); else
	{
	FavoriteHubListWindow *child = new FavoriteHubListWindow(this);
	mdiArea->addSubWindow(child);
	child->action  = menuWindow->addAction(tr("Fav_Hubs"));
	
		child->action->setCheckable(true);
		connect(child->action, SIGNAL(triggered()), windowMapper, SLOT(map()));		
		connect(child, SIGNAL(actionReleased(QAction *)), this, SLOT(slotclosemdi(QAction *)));
		WindowToolBar->addAction(child->action);	
	child->show();
	}
}

void MainWindowImpl::qcdconFunc(QString adr, int port)
{
	OpenHub(adr,port);
}

void MainWindowImpl::fQuickConFunc()
{
	TQuickConnectDialog *qcd = new TQuickConnectDialog(this);
	connect(qcd,SIGNAL(con_pressed(QString, int)),this,SLOT(qcdconFunc(QString, int)));
}

void MainWindowImpl::transcheck()
{
	if (actionTransfers->isChecked()) TransDock->setVisible(true); else TransDock->setVisible(false);
}

void MainWindowImpl::statusbarcheck()
{
	if (actionStatusBar->isChecked()) statusbar->setVisible(true); else statusbar->setVisible(false);
}

void MainWindowImpl::winbarcheck()
{
	if (actionTabBar->isChecked()) WindowToolBar->setVisible(true); else WindowToolBar->setVisible(false);
}

void MainWindowImpl::toolbarcheck()
{
	if (actionToolBar->isChecked()) toolBar->setVisible(true); else toolBar->setVisible(false);
}

void MainWindowImpl::setShareSize(const QString &sz)
{
	shareStatusLbl->setText(sz);
}

void MainWindowImpl::slotclosemdi(QAction *act)
{
	WindowToolBar->removeAction(act);
}

QString ThreadGetTTH::getA()
{
	return a;
}

void ThreadGetTTH::setA(QString s)
{
	a=s;
}

QString ThreadGetTTH::getB()
{
	return b;
}

QString ThreadGetTTH::getC()
{
	return c;
}

void ThreadGetTTH::stop()
{
	 _stp = true;
}

void ThreadGetTTH::run()
{	
	_stp = false;
	b="";
	c="";
	if (!a.isEmpty()) {
		using namespace dcpp;
		AutoArray<char> buf(512*1024);

		try {
			QFile f(a);
			QFileInfo fi(f);
			TigerTree tth(TigerTree::calcBlockSize(f.size(), 1));

			f.open(QIODevice::ReadOnly);
			if (fi.size() > 0) {
				qint64 n = 512*1024;
				while( (n = f.read(buf, n)) > 0) {
					if (_stp) break;
					tth.update(buf, n);
					n = 512*1024;
					if (_stp) break;
				}
			} else tth.update("", 0);
			tth.finalize();

			b = tr(tth.getRoot().toBase32().c_str());
			
			c = "magnet:?xt=urn:tree:tiger:" + b + "&xl=" + QString::number(fi.size()) + "&dn=" + (Util::encodeURI(fi.fileName().toLocal8Bit().data())).c_str();
			f.close();
			if (_stp) return;
		} catch(...) { }
	using namespace std;
	emit ready();
	}
	return ;
}

void MainWindowImpl::GetTTHFunc()
{	
	actionGet_TTH_for_file->setEnabled(false);
	QString selectedFilter=tr("");
	QFileDialog::Options options;
	//options |= QFileDialog::DontUseNativeDialog;	
	thrdGetTTh.setA( QFileDialog::getOpenFileName(this, tr("Select File"),"", tr("All Files (*)"), &selectedFilter, options) );
	
	thrdGetTTh.start(QThread::LowPriority);
}

void MainWindowImpl::show_tthFunc()
{
	new TthDialog(this,thrdGetTTh.getA(),thrdGetTTh.getB(),thrdGetTTh.getC());
//	QMessageBox::information(this, tr("Tiger Tree Hash"),tr("File: ")+thrdGetTTh.getA()+tr("\nTTH: ")+thrdGetTTh.getB()+tr("\n")+thrdGetTTh.getC());
	actionGet_TTH_for_file->setEnabled(true);
}


//
