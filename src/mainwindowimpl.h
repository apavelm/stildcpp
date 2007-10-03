#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
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
#include "ui_mainwindow.h"

#include "config.h"

#include "frmAbout.h"
#include "preferencesdialog.h"
#include "mdi_c.h"
#include "quickconnectdlg.h"

//

class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	~MainWindowImpl();
	
	void setVisible(bool visible);
	
protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void setIcon(int index);
	void setToolTip(const QString & title);
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void showMessage(const QString & title, const QString & message, int type, int millisecondsTimeoutHint = 10000);
	void messageClicked();
	void statusMessage(const QString & mes);
	void slotclosemdi(QAction *act);
	void About();
	void DonateFunc();
	void HomepageFunc();
	void SearchFunc();
	void PreferencesFunc();
	void FavHubListFunc();
	void QuickConFunc();
	void OpenHub(QString &adr);
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
	void setShareSize(const QString &sz);


	QAction *minimizeAction;
	QAction *maximizeAction;
	QAction *restoreAction;
	QAction *quitAction;
	
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
	

};

#endif

