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
	delete showhide;
	delete trayIcon;
	delete trayIconMenu;
	delete shareStatusLbl;
	delete m_tabwin;
	thrdGetTTh.wait();
}

void MainWindowImpl::initMain()
{
	setupUi(this);
	
	m_tabwin = new TabWidget( this );
	connect(m_tabwin, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );	
	setCentralWidget(m_tabwin);
	
	createActions();
	createToolBars();
	createTrayIcon();

	connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));	
	
	setToolTip(tr(APPLICATIONNAME));
	setWindowTitle(tr(APPLICATIONNAME));
	
	shareStatusLbl = new QLabel;
	setShareSize(tr("Total shared: 0b"));
	statusBar()->addPermanentWidget(shareStatusLbl);
	
	trayIcon->setIcon(QIcon(":/images/icon.png"));
	
	setWindowIcon(QIcon(":/images/icon.png"));
	
	if (p_opt[AppSettings::s_USETRAY]) 
		trayIcon->show();

	if (!p_opt[AppSettings::s_STARTHIDDEN])
		this->show();
	else
		if (!p_opt[AppSettings::s_USETRAY]) this->show();
}

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	initMain();
}

MainWindowImpl::MainWindowImpl(int *a, QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	p_opt = a;
	initMain();
}

void MainWindowImpl::closeEvent(QCloseEvent *event)
{
	if (p_opt[AppSettings::s_HIDEONCLOSE])
	{
		if (trayIcon->isVisible()) 
			{
				hide();
				event->ignore();
			}
	} 
	else 
		{
			if (p_opt[AppSettings::s_PROMPTONCLOSE])
			{
				QMessageBox::StandardButton reply;
				reply = QMessageBox::question(this, tr("StilDC++"), tr("Do you realy want to exit?"), QMessageBox::Yes | QMessageBox::No);
				if (reply == QMessageBox::Yes)
				{
					event->accept();
				}
				else event->ignore();
			} else
			{
				event->accept();
			}
		}
}

void MainWindowImpl::setToolTip(const QString & title)
{
	trayIcon->setToolTip(title);
}

void MainWindowImpl::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) 
	{
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
	showhide = new QAction(tr("Show / Hide"), this);
	showhide->setStatusTip(tr("Show / Hide application window"));
	showhide->setToolTip(tr("Show / Hide application window"));
	connect(showhide, SIGNAL(triggered()), this, SLOT(showhideFunc()));
	
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	
	connect(actionOpen_filelist, SIGNAL(triggered()), this, SLOT(openfilelistFunc()));
	connect(actionOpen_own_filelist, SIGNAL(triggered()), this, SLOT(openownfilelistFunc()));
	connect(actionOpen_Downloads_Folder, SIGNAL(triggered()), this, SLOT(OpenDownloadsFolderFunc()));
	connect(actionRefresh_own_filelist, SIGNAL(triggered()), this, SLOT(RefreshOwnFileListFunc()));

	connect(actionAbout, SIGNAL(triggered()), this, SLOT(About()));	
	connect(aboutqtact, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(actionTransfers, SIGNAL(triggered()), this, SLOT(transcheck()));
	connect(actionStatusBar, SIGNAL(triggered()), this, SLOT(statusbarcheck()));
	connect(actionToolBar, SIGNAL(triggered()), this, SLOT(toolbarcheck()));	
	
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(PreferencesFunc()));

	connect(actionDonate, SIGNAL(triggered()), this, SLOT(DonateFunc()));
	connect(actionHomepage, SIGNAL(triggered()), this, SLOT(HomepageFunc()));
	connect(actionSearch, SIGNAL(triggered()), this, SLOT(SearchFunc()));
	connect(actionGet_TTH_for_file, SIGNAL(triggered()), this, SLOT(GetTTHFunc()));
	
	connect(actionFavorite_Hubs, SIGNAL(triggered()), this, SLOT(FavHubListFunc()));
	connect(actionQuick_Connect, SIGNAL(triggered()), this, SLOT(fQuickConFunc()));
	
	connect(&thrdGetTTh, SIGNAL(ready()), this, SLOT(show_tthFunc()));
	
}

void MainWindowImpl::showhideFunc()
{
	if (this->isHidden()) this->show(); else this->hide();
}

void MainWindowImpl::createTrayIcon()
{	
	trayIconMenu = new QMenu(this);	
	trayIconMenu->addAction(showhide);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionExit);

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

void MainWindowImpl::DonateFunc()
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new PMWindow(this,tr("Vasya"))),"PM"));
}

void MainWindowImpl::HomepageFunc()
{
	QUrl url = QUrl(tr(APPHOMEPAGE));
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::OpenHub(QString &adr, int port)
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new HubWindow(this,tr("adress"))),"Hub"));
}

void MainWindowImpl::SearchFunc()
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new SearchWindow(this,tr(""))),"search"));
}

void MainWindowImpl::FavHubListFunc()
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new FavoriteHubListWindow(this)),"Fav_Hub"));
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

void MainWindowImpl::toolbarcheck()
{
	if (actionToolBar->isChecked()) toolBar->setVisible(true); else toolBar->setVisible(false);
}

void MainWindowImpl::setShareSize(const QString &sz)
{
	shareStatusLbl->setText(sz);
}

const QString & ThreadGetTTH::getA()
{
	return a;
}

void ThreadGetTTH::setA(QString s)
{
	a=s;
}

const QString & ThreadGetTTH::getB()
{
	return b;
}

const QString & ThreadGetTTH::getC()
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
	actionGet_TTH_for_file->setEnabled(true);
}

void MainWindowImpl::openfilelistFunc()
{
	// Open Custom FileList
	
	QString selectedFilter=tr("");
	QFileDialog::Options options;
	//options |= QFileDialog::DontUseNativeDialog;
	QString fn = QFileDialog::getOpenFileName(this, tr("Select File"),"", tr("All Files (*)"), &selectedFilter, options);
	if (!fn.isEmpty()) OpenList(fn);
}

void MainWindowImpl::openownfilelistFunc()
{
	// Open Own FileList
	OpenList(tr(""));
}

void MainWindowImpl::OpenList(const QString &filename)
{
	// Function to open filelists
}

void MainWindowImpl::RefreshOwnFileListFunc()
{
	// Refreshes (rehash) own filelist
}

void MainWindowImpl::OpenDownloadsFolderFunc()
{
	QUrl url = QUrl("/home/").toLocalFile(); // FIXME : Change to downloadDIR variable.
	QDesktopServices::openUrl(url);
}

void MainWindowImpl::slotCurrentTabChanged(int index)
{
	// Insert Code Here 
}

//
