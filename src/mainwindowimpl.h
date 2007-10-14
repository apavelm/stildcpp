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

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include "ui_mainwindow.h"

#include "config.h"

#include "frmAbout.h"
#include "preferencesdialog.h"
#include "mdi_c.h"
#include "tthdial.h"
#include "quickconnectdlg.h"
#include "defs.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/MerkleTree.h"
#include "client/Util.h"
#include "client/Text.h"
#include "client/File.h"
//


class ThreadGetTTH : public QThread
{
	Q_OBJECT
public:
	void run();
	void stop();
	const QString & getA();
	void setA(QString);
	const QString & getB();
	const QString & getC();
private:
	volatile bool _stp;
	QString a,b,c;
signals:
	void ready();
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindowImpl();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void setToolTip(const QString & title);
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint = 10000);
	void messageClicked();
	void statusMessage(const QString & mes);
	void slotclosemdi(QAction *act);
	void About();
	void openfilelistFunc();
	void OpenDownloadsFolderFunc();
	void RefreshOwnFileListFunc();
	void openownfilelistFunc();
	void DonateFunc();
	void HomepageFunc();
	void SearchFunc();
	void qcdconFunc(QString , int);
	void show_tthFunc();
	void showhideFunc();
	void GetTTHFunc();
	void PreferencesFunc();
	void FavHubListFunc();
	void fQuickConFunc();
	void OpenHub(QString &adr, int port);
	void transcheck(); // if TransferWindow Checked in menu @View@
	void statusbarcheck(); // if StatusBar Checked in menu @View@
	void toolbarcheck(); // if ToolBar Checked in menu @View@
	void winbarcheck(); // if WinTabBar Checked in menu @View@
	
	void updateWindowMenu();
	void setActiveSubWindow(QWidget *window);

private:
	void createActions();
	void createTrayIcon();
	void createToolBars();
	void OpenList(const QString &);
	void setShareSize(const QString &sz);

	QAction *showhide;

	QAction *closeAct;
	QAction *closeAllAct;
	QAction *tileAct;
	QAction *cascadeAct;
	QAction *nextAct;
	QAction *previousAct;
	QAction *separatorAct;

	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	QLabel *shareStatusLbl;
	
	
	QMdiArea *mdiArea;
	QSignalMapper *windowMapper;
	
	MdiChild *activeMdiChild();
	QMdiSubWindow *findMdiChild(const int id, QString &idtext, QAction *act);
	QMdiSubWindow *findMdiChild(const int id, QString &idtext);
	QMdiSubWindow *findMdiChild(const int id);
	
	ThreadGetTTH thrdGetTTh;
	

};

#endif

