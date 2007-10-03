/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Tue Oct 2 23:49:47 2007
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QColumnView>
#include <QtGui/QDockWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionOpen_filelist;
    QAction *actionOpen_own_filelist;
    QAction *actionRefresh_own_filelist;
    QAction *actionOpen_Downloads_Folder;
    QAction *actionQuick_Connect;
    QAction *actionFollow_last_redirect;
    QAction *actionReconnect;
    QAction *actionSettings;
    QAction *actionGet_TTH_for_file;
    QAction *actionPublic_Hubs;
    QAction *actionFavorite_Hubs;
    QAction *actionFavorite_Users;
    QAction *actionIgnored_Users;
    QAction *actionSearch;
    QAction *actionADL_Search;
    QAction *actionSearch_Spy;
    QAction *actionNotepad;
    QAction *actionIndexing_Progress;
    QAction *actionToolBar;
    QAction *actionStatusBar;
    QAction *actionTransfers;
    QAction *actionDownload_Queue;
    QAction *actionFinished_downloads;
    QAction *actionFinished_Uploads;
    QAction *actionHomepage;
    QAction *actionDonate;
    QAction *aboutqtact;
    QAction *actionTabBar;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuFiles;
    QMenu *menuView;
    QMenu *menuTransfers;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *WindowToolBar;
    QDockWidget *TransDock;
    QWidget *dockWidgetContents;
    QVBoxLayout *vboxLayout;
    QColumnView *columnView;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
    if (MainWindow->objectName().isEmpty())
        MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    MainWindow->resize(598, 388);
    MainWindow->setUnifiedTitleAndToolBarOnMac(false);
    actionExit = new QAction(MainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionOpen_filelist = new QAction(MainWindow);
    actionOpen_filelist->setObjectName(QString::fromUtf8("actionOpen_filelist"));
    actionOpen_own_filelist = new QAction(MainWindow);
    actionOpen_own_filelist->setObjectName(QString::fromUtf8("actionOpen_own_filelist"));
    actionRefresh_own_filelist = new QAction(MainWindow);
    actionRefresh_own_filelist->setObjectName(QString::fromUtf8("actionRefresh_own_filelist"));
    actionOpen_Downloads_Folder = new QAction(MainWindow);
    actionOpen_Downloads_Folder->setObjectName(QString::fromUtf8("actionOpen_Downloads_Folder"));
    actionQuick_Connect = new QAction(MainWindow);
    actionQuick_Connect->setObjectName(QString::fromUtf8("actionQuick_Connect"));
    actionFollow_last_redirect = new QAction(MainWindow);
    actionFollow_last_redirect->setObjectName(QString::fromUtf8("actionFollow_last_redirect"));
    actionReconnect = new QAction(MainWindow);
    actionReconnect->setObjectName(QString::fromUtf8("actionReconnect"));
    actionSettings = new QAction(MainWindow);
    actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
    actionGet_TTH_for_file = new QAction(MainWindow);
    actionGet_TTH_for_file->setObjectName(QString::fromUtf8("actionGet_TTH_for_file"));
    actionPublic_Hubs = new QAction(MainWindow);
    actionPublic_Hubs->setObjectName(QString::fromUtf8("actionPublic_Hubs"));
    actionFavorite_Hubs = new QAction(MainWindow);
    actionFavorite_Hubs->setObjectName(QString::fromUtf8("actionFavorite_Hubs"));
    actionFavorite_Users = new QAction(MainWindow);
    actionFavorite_Users->setObjectName(QString::fromUtf8("actionFavorite_Users"));
    actionIgnored_Users = new QAction(MainWindow);
    actionIgnored_Users->setObjectName(QString::fromUtf8("actionIgnored_Users"));
    actionSearch = new QAction(MainWindow);
    actionSearch->setObjectName(QString::fromUtf8("actionSearch"));
    actionADL_Search = new QAction(MainWindow);
    actionADL_Search->setObjectName(QString::fromUtf8("actionADL_Search"));
    actionSearch_Spy = new QAction(MainWindow);
    actionSearch_Spy->setObjectName(QString::fromUtf8("actionSearch_Spy"));
    actionNotepad = new QAction(MainWindow);
    actionNotepad->setObjectName(QString::fromUtf8("actionNotepad"));
    actionIndexing_Progress = new QAction(MainWindow);
    actionIndexing_Progress->setObjectName(QString::fromUtf8("actionIndexing_Progress"));
    actionToolBar = new QAction(MainWindow);
    actionToolBar->setObjectName(QString::fromUtf8("actionToolBar"));
    actionToolBar->setCheckable(true);
    actionToolBar->setChecked(true);
    actionStatusBar = new QAction(MainWindow);
    actionStatusBar->setObjectName(QString::fromUtf8("actionStatusBar"));
    actionStatusBar->setCheckable(true);
    actionStatusBar->setChecked(true);
    actionTransfers = new QAction(MainWindow);
    actionTransfers->setObjectName(QString::fromUtf8("actionTransfers"));
    actionTransfers->setCheckable(true);
    actionTransfers->setChecked(true);
    actionDownload_Queue = new QAction(MainWindow);
    actionDownload_Queue->setObjectName(QString::fromUtf8("actionDownload_Queue"));
    actionFinished_downloads = new QAction(MainWindow);
    actionFinished_downloads->setObjectName(QString::fromUtf8("actionFinished_downloads"));
    actionFinished_Uploads = new QAction(MainWindow);
    actionFinished_Uploads->setObjectName(QString::fromUtf8("actionFinished_Uploads"));
    actionHomepage = new QAction(MainWindow);
    actionHomepage->setObjectName(QString::fromUtf8("actionHomepage"));
    actionDonate = new QAction(MainWindow);
    actionDonate->setObjectName(QString::fromUtf8("actionDonate"));
    aboutqtact = new QAction(MainWindow);
    aboutqtact->setObjectName(QString::fromUtf8("aboutqtact"));
    actionTabBar = new QAction(MainWindow);
    actionTabBar->setObjectName(QString::fromUtf8("actionTabBar"));
    actionTabBar->setCheckable(true);
    actionTabBar->setChecked(true);
    centralwidget = new QWidget(MainWindow);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    MainWindow->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindow);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 598, 30));
    menuFiles = new QMenu(menubar);
    menuFiles->setObjectName(QString::fromUtf8("menuFiles"));
    menuView = new QMenu(menubar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuTransfers = new QMenu(menubar);
    menuTransfers->setObjectName(QString::fromUtf8("menuTransfers"));
    menuWindow = new QMenu(menubar);
    menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    MainWindow->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindow);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    statusbar->setSizeGripEnabled(false);
    MainWindow->setStatusBar(statusbar);
    WindowToolBar = new QToolBar(MainWindow);
    WindowToolBar->setObjectName(QString::fromUtf8("WindowToolBar"));
    QFont font;
    font.setPointSize(7);
    WindowToolBar->setFont(font);
    WindowToolBar->setAutoFillBackground(false);
    WindowToolBar->setAllowedAreas(Qt::BottomToolBarArea|Qt::NoToolBarArea|Qt::TopToolBarArea);
    WindowToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    WindowToolBar->setFloatable(false);
    MainWindow->addToolBar(Qt::BottomToolBarArea, WindowToolBar);
    TransDock = new QDockWidget(MainWindow);
    TransDock->setObjectName(QString::fromUtf8("TransDock"));
    TransDock->setFeatures(QDockWidget::DockWidgetMovable);
    TransDock->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::NoDockWidgetArea|Qt::TopDockWidgetArea);
    dockWidgetContents = new QWidget(TransDock);
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    vboxLayout = new QVBoxLayout(dockWidgetContents);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    columnView = new QColumnView(dockWidgetContents);
    columnView->setObjectName(QString::fromUtf8("columnView"));

    vboxLayout->addWidget(columnView);

    TransDock->setWidget(dockWidgetContents);
    MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), TransDock);
    toolBar = new QToolBar(MainWindow);
    toolBar->setObjectName(QString::fromUtf8("toolBar"));
    toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    menubar->addAction(menuFiles->menuAction());
    menubar->addAction(menuView->menuAction());
    menubar->addAction(menuTransfers->menuAction());
    menubar->addAction(menuWindow->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menuFiles->addAction(actionOpen_filelist);
    menuFiles->addAction(actionOpen_own_filelist);
    menuFiles->addAction(actionRefresh_own_filelist);
    menuFiles->addAction(actionOpen_Downloads_Folder);
    menuFiles->addSeparator();
    menuFiles->addAction(actionQuick_Connect);
    menuFiles->addAction(actionFollow_last_redirect);
    menuFiles->addAction(actionReconnect);
    menuFiles->addSeparator();
    menuFiles->addAction(actionSettings);
    menuFiles->addAction(actionGet_TTH_for_file);
    menuFiles->addSeparator();
    menuFiles->addAction(actionExit);
    menuView->addAction(actionPublic_Hubs);
    menuView->addAction(actionFavorite_Hubs);
    menuView->addSeparator();
    menuView->addAction(actionFavorite_Users);
    menuView->addAction(actionIgnored_Users);
    menuView->addSeparator();
    menuView->addAction(actionSearch);
    menuView->addAction(actionADL_Search);
    menuView->addAction(actionSearch_Spy);
    menuView->addSeparator();
    menuView->addAction(actionIndexing_Progress);
    menuView->addSeparator();
    menuView->addAction(actionToolBar);
    menuView->addAction(actionStatusBar);
    menuView->addAction(actionTransfers);
    menuView->addAction(actionTabBar);
    menuTransfers->addAction(actionDownload_Queue);
    menuTransfers->addAction(actionFinished_downloads);
    menuTransfers->addSeparator();
    menuTransfers->addAction(actionFinished_Uploads);
    menuHelp->addAction(actionAbout);
    menuHelp->addSeparator();
    menuHelp->addAction(actionDonate);
    menuHelp->addAction(actionHomepage);
    menuHelp->addSeparator();
    menuHelp->addAction(aboutqtact);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    actionExit->setStatusTip(QApplication::translate("MainWindow", "Quit from StilDC++ application", 0, QApplication::UnicodeUTF8));
    actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    actionOpen_filelist->setText(QApplication::translate("MainWindow", "Open filelist", 0, QApplication::UnicodeUTF8));
    actionOpen_own_filelist->setText(QApplication::translate("MainWindow", "Open own filelist", 0, QApplication::UnicodeUTF8));
    actionRefresh_own_filelist->setText(QApplication::translate("MainWindow", "Refresh own filelist", 0, QApplication::UnicodeUTF8));
    actionOpen_Downloads_Folder->setText(QApplication::translate("MainWindow", "Open Downloads Folder", 0, QApplication::UnicodeUTF8));
    actionQuick_Connect->setText(QApplication::translate("MainWindow", "Quick Connect", 0, QApplication::UnicodeUTF8));
    actionFollow_last_redirect->setText(QApplication::translate("MainWindow", "Follow last redirect", 0, QApplication::UnicodeUTF8));
    actionReconnect->setText(QApplication::translate("MainWindow", "Reconnect", 0, QApplication::UnicodeUTF8));
    actionSettings->setText(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
    actionGet_TTH_for_file->setText(QApplication::translate("MainWindow", "Get TTH for file...", 0, QApplication::UnicodeUTF8));
    actionPublic_Hubs->setText(QApplication::translate("MainWindow", "Public Hubs", 0, QApplication::UnicodeUTF8));
    actionFavorite_Hubs->setText(QApplication::translate("MainWindow", "Favorite Hubs", 0, QApplication::UnicodeUTF8));
    actionFavorite_Users->setText(QApplication::translate("MainWindow", "Favorite Users", 0, QApplication::UnicodeUTF8));
    actionIgnored_Users->setText(QApplication::translate("MainWindow", "Ignored Users", 0, QApplication::UnicodeUTF8));
    actionSearch->setText(QApplication::translate("MainWindow", "Search", 0, QApplication::UnicodeUTF8));
    actionADL_Search->setText(QApplication::translate("MainWindow", "ADL Search", 0, QApplication::UnicodeUTF8));
    actionSearch_Spy->setText(QApplication::translate("MainWindow", "Search Spy", 0, QApplication::UnicodeUTF8));
    actionNotepad->setText(QApplication::translate("MainWindow", "Notepad", 0, QApplication::UnicodeUTF8));
    actionIndexing_Progress->setText(QApplication::translate("MainWindow", "Indexing Progress", 0, QApplication::UnicodeUTF8));
    actionToolBar->setText(QApplication::translate("MainWindow", "ToolBar", 0, QApplication::UnicodeUTF8));
    actionToolBar->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
    actionStatusBar->setText(QApplication::translate("MainWindow", "StatusBar", 0, QApplication::UnicodeUTF8));
    actionStatusBar->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", 0, QApplication::UnicodeUTF8));
    actionTransfers->setText(QApplication::translate("MainWindow", "Transfers", 0, QApplication::UnicodeUTF8));
    actionTransfers->setShortcut(QApplication::translate("MainWindow", "Ctrl+3", 0, QApplication::UnicodeUTF8));
    actionDownload_Queue->setText(QApplication::translate("MainWindow", "Download Queue", 0, QApplication::UnicodeUTF8));
    actionFinished_downloads->setText(QApplication::translate("MainWindow", "Finished downloads", 0, QApplication::UnicodeUTF8));
    actionFinished_Uploads->setText(QApplication::translate("MainWindow", "Finished Uploads", 0, QApplication::UnicodeUTF8));
    actionHomepage->setText(QApplication::translate("MainWindow", "Homepage", 0, QApplication::UnicodeUTF8));
    actionDonate->setText(QApplication::translate("MainWindow", "Donate", 0, QApplication::UnicodeUTF8));
    aboutqtact->setText(QApplication::translate("MainWindow", "About Qt", 0, QApplication::UnicodeUTF8));
    actionTabBar->setText(QApplication::translate("MainWindow", "Window Bar", 0, QApplication::UnicodeUTF8));
    actionTabBar->setShortcut(QApplication::translate("MainWindow", "Ctrl+4", 0, QApplication::UnicodeUTF8));
    menuFiles->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
    menuTransfers->setTitle(QApplication::translate("MainWindow", "Transfers", 0, QApplication::UnicodeUTF8));
    menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    WindowToolBar->setWindowTitle(QApplication::translate("MainWindow", "WindowToolBar", 0, QApplication::UnicodeUTF8));
    TransDock->setWindowTitle(QApplication::translate("MainWindow", "Transfers", 0, QApplication::UnicodeUTF8));
    toolBar->setWindowTitle(QApplication::translate("MainWindow", "Main", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

#endif // UI_MAINWINDOW_H
