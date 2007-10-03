#include "mdi_c.h"

MdiChild::MdiChild(QWidget *parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	type=0;
	idText=tr("");
}

MdiChild::~MdiChild()
{
	emit actionReleased(action);
}

PMWindow::PMWindow(QWidget *parent, const QString &nick) : MdiChild(parent)
{	
	setupUi(this);
	type = 2;
	idText  = nick;
	setWindowTitle(tr("Private Chat with: ")+nick);
}

HubWindow::HubWindow(QWidget *parent, const QString &hubaddr) : MdiChild(parent)
{	
	setupUi(this);
	type = 1;
	idText  = hubaddr;
	setWindowTitle(tr("Hub: ")+hubaddr);
}

SearchWindow::SearchWindow(QWidget *parent, const QString &what) : MdiChild(parent)
{	
	setupUi(this);
	type = 3;
	idText  = what;
	setWindowTitle(tr("Search for: ")+what);
	connect(searchButton, SIGNAL(pressed()), this, SLOT(SearchBtn()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(SearchBtn()));
}

void SearchWindow::SearchBtn()
{	
	searchButton->setDefault(true);
	if (searchCombo->currentIndex()>=0)
	{
		idText  = searchCombo->itemText(searchCombo->currentIndex());	
		action->setText(tr("Search for: ")+idText);
		setWindowTitle(tr("Search for: ")+idText);		
		//SEARCH
	}
}

FavoriteHubListWindow::FavoriteHubListWindow(QWidget *parent) : MdiChild(parent)
{	
	setupUi(this);
	type = 4;
	idText  = tr("");
	setWindowTitle(tr("Favorite Hubs"));
}

//
