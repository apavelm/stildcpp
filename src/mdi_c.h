#ifndef __MDI_C_H__
#define __MDI_C_H__

#include <QDialog>
#include <QString>
#include <QAction>
#include <QWidget>


#include "ui_PMWindow.h"
#include "ui_HUBWindow.h"
#include "ui_search.h"
#include "ui_favhublist.h"

//

class MdiChild : public QDialog
{
	Q_OBJECT
public:
	MdiChild(QWidget *parent);
	~MdiChild();
	QAction *action;
	
	int type;
	/*  0 - MdiChild (NONE)
		1 - Hub
		2 - Private Chat
		3 - Search
		4 - Favorite Hub List
	*/
	QString idText;
signals:
	void actionReleased(QAction *action);
};

class PMWindow : public MdiChild, private Ui::mdiPMwin
{
	Q_OBJECT
public:
	PMWindow(QWidget *parent, const QString &nick);
};

class HubWindow : public MdiChild, private Ui::mdiHUBwin
{
	Q_OBJECT
public:
	HubWindow(QWidget *parent, const QString &hubaddr);
};

class SearchWindow : public MdiChild, private Ui::mdiSEARCHwin
{
	Q_OBJECT
private slots:
	void SearchBtn();
public:
	SearchWindow(QWidget *parent, const QString &what);
};

class FavoriteHubListWindow : public MdiChild, private Ui::mdiFAVHUBwin
{
	Q_OBJECT
public:
	FavoriteHubListWindow(QWidget *parent);
};


#endif // __MDI_C_H__
