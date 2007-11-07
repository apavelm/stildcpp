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

#include "preferencesdialog.h"

using namespace std;
using namespace dcpp;

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowIcon(QIcon(":/images/pref_icon.png"));

	categoryList->header()->hide();

	initCategoryList();
	initGeneralPage();
	initConnectionPage();
	initDownloadsPage();
	initDownloadsFavPage();
	
	categoryList->setCurrentItem(categoryList->topLevelItem(0));
	show();
}

void PreferencesDialog::on_categoryList_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	widgetStack->setCurrentIndex(c->data(0, Qt::UserRole).toInt());
	currentLabel->setText("<h3>" + c->text(0) + "</h3>");
}

/*

void PreferencesDialog::setFontString(const QFont &newfont) {
	QString font = newfont.family();
	if (newfont.bold())
		font += QString(", %1").arg(tr("bold"));
	if (newfont.italic())
		font += QString(", %1").arg(tr("italic"));
	font += QString(", %1").arg(newfont.pointSize());
	fontLine->setText(font);
	fontLine->setFont(newfont);
}


*/

void PreferencesDialog::on_okBtn_clicked() 
{
	accept();
	close();
}

void PreferencesDialog::initCategoryList() 
{
	QTreeWidgetItem *it0 = new QTreeWidgetItem(categoryList);
		it0->setText(0,tr("General"));
		it0->setIcon(0,QIcon(":/images/icon_error.png"));
	QTreeWidgetItem *it1 = new QTreeWidgetItem(categoryList);
		it1->setText(0,tr("Connection Settings"));
		it1->setIcon(0,QIcon(":/images/icon_mail.png"));
	QTreeWidgetItem *it2 = new QTreeWidgetItem(categoryList);
		it2->setText(0,tr("Downloads"));
		it2->setIcon(0,QIcon(":/images/icon.png"));
				
					QTreeWidgetItem *it2a = new QTreeWidgetItem(it2);
						it2a->setText(0,tr("Favorites"));
						it2a->setIcon(0,QIcon(":/images/icon_error.png"));
					QTreeWidgetItem *it2b = new QTreeWidgetItem(it2);
						it2b->setText(0,tr("Preview"));
						it2b->setIcon(0,QIcon(":/images/icon_mail.png"));
					QTreeWidgetItem *it2c = new QTreeWidgetItem(it2);
						it2c->setText(0,tr("Queue"));
						it2c->setIcon(0,QIcon(":/images/icon.png"));
								
	QTreeWidgetItem *it3 = new QTreeWidgetItem(categoryList);
		it3->setText(0,tr("Sharing"));
		it3->setIcon(0,QIcon(":/images/icon_error.png"));
	QTreeWidgetItem *it4 = new QTreeWidgetItem(categoryList);
		it4->setText(0,tr("Messages"));
		it4->setIcon(0,QIcon(":/images/icon_mail.png"));
	QTreeWidgetItem *it5 = new QTreeWidgetItem(categoryList);
		it5->setText(0,tr("Look and Feel"));
		it5->setIcon(0,QIcon(":/images/icon.png"));
		
					QTreeWidgetItem *it5a = new QTreeWidgetItem(it5);
						it5a->setText(0,tr("Colors and Fonts"));
						it5a->setIcon(0,QIcon(":/images/pref_colors_n_fonts.png"));
					QTreeWidgetItem *it5b = new QTreeWidgetItem(it5);
						it5b->setText(0,tr("Popup Messages"));
						it5b->setIcon(0,QIcon(":/images/icon_mail.png"));
					QTreeWidgetItem *it5c = new QTreeWidgetItem(it5);
						it5c->setText(0,tr("Sounds"));
						it5c->setIcon(0,QIcon(":/images/pref_sounds.png"));
		
	QTreeWidgetItem *it6 = new QTreeWidgetItem(categoryList);
		it6->setText(0,tr("Additional"));
		it6->setIcon(0,QIcon(":/images/icon_error.png"));
		
					QTreeWidgetItem *it6a = new QTreeWidgetItem(it6);
						it6a->setText(0,tr("Logs"));
						it6a->setIcon(0,QIcon(":/images/icon_mail.png"));
					QTreeWidgetItem *it6b = new QTreeWidgetItem(it6);
						it6b->setText(0,tr("User Commands"));
						it6b->setIcon(0,QIcon(":/images/icon.png"));		
					QTreeWidgetItem *it6c = new QTreeWidgetItem(it6);
						it6c->setText(0,tr("Misc"));
						it6c->setIcon(0,QIcon(":/images/icon_error.png"));		
		
	categoryList->setAlternatingRowColors(true);

	// Make item-index relations
	for (int i = 0, index = 0; i < categoryList->topLevelItemCount(); i++, index++) {
		QTreeWidgetItem *item = categoryList->topLevelItem(i);
		item->setExpanded(true);
		item->setData(0, Qt::UserRole, index);
		for (int j = 0; j < item->childCount(); j++)
			item->child(j)->setData(0, Qt::UserRole, ++index);
	}	
}

void PreferencesDialog::initGeneralPage() 
{
	NickEdit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(NICK))));
	EmailEdit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(EMAIL))));
	DescEdit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(DESCRIPTION))));
	
	int selected = 0, j = 0;
	for(StringIter i = dcpp::SettingsManager::connectionSpeeds.begin(); i != dcpp::SettingsManager::connectionSpeeds.end(); ++i, ++j) 
	{
		connections->addItem(QString::fromStdString(*i));
		if(selected == 0 && SETTING(UPLOAD_SPEED) == *i) {
			selected = j; 
		}
	}
	connections->setCurrentIndex(selected);
}

void PreferencesDialog::applyGeneralPage()
{
	SettingsManager::getInstance()->set(SettingsManager::NICK, dcpp::Text::fromT(StilUtils::QtoTstr(NickEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::EMAIL, dcpp::Text::fromT(StilUtils::QtoTstr(EmailEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::DESCRIPTION, dcpp::Text::fromT(StilUtils::QtoTstr(DescEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::UPLOAD_SPEED, dcpp::Text::fromT(StilUtils::QtoTstr(connections->currentText()) ) );
}

void PreferencesDialog::initConnectionPage() 
{
	connect(radio_ICS_active, SIGNAL(clicked()), this, SLOT(ConnectionPageUpdate()));
	connect(radio_ICS_manual, SIGNAL(clicked()), this, SLOT(ConnectionPageUpdate()));
	connect(radio_ICS_passive, SIGNAL(clicked()), this, SLOT(ConnectionPageUpdate()));
	connect(radio_OCS_direct, SIGNAL(clicked()), this, SLOT(ConnectionPageUpdate()));
	connect(radio_OCS_socks, SIGNAL(clicked()), this, SLOT(ConnectionPageUpdate()));
	
	switch(SETTING(INCOMING_CONNECTIONS)) {
		case SettingsManager::INCOMING_DIRECT: radio_ICS_active->setChecked(true); break;
		//case SettingsManager::INCOMING_FIREWALL_UPNP: radio_ICS_upnp->setChecked(true); break; // not used Win firewall
		case SettingsManager::INCOMING_FIREWALL_NAT: radio_ICS_manual->setChecked(true); break;
		case SettingsManager::INCOMING_FIREWALL_PASSIVE: radio_ICS_passive->setChecked(true); break;
		default: radio_ICS_active->setChecked(true); break;
	}

	switch(SETTING(OUTGOING_CONNECTIONS)) {
		case SettingsManager::OUTGOING_DIRECT: radio_OCS_direct->setChecked(true); break;
		case SettingsManager::OUTGOING_SOCKS5: radio_OCS_socks->setChecked(true); break;
		default: radio_OCS_direct->setChecked(true); break;
	}
	
	check_notoverride->setChecked(SETTING(NO_IP_OVERRIDE));
	check_SocksResolve->setChecked(SETTING(SOCKS_RESOLVE));
	check_UpdateIPstartup->setChecked(APPSETTING(i_UPDATEIP));
	
	TCPport_Edit->setText( QString("%1").arg(SETTING(TCP_PORT)) );
	UDPport_Edit->setText( QString("%1").arg(SETTING(UDP_PORT)) );
	TLSport_Edit->setText( QString("%1").arg(SETTING(TLS_PORT)) );
	SocksPort_Edit->setText( QString("%1").arg(SETTING(SOCKS_PORT)) );
	WANIP_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(EXTERNAL_IP))));
	SocksIP_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(SOCKS_SERVER))));
	SocksUsername_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(SOCKS_USER))));
	SocksPassword_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(SOCKS_PASSWORD))));
	
	ConnectionPageUpdate();
}

void PreferencesDialog::ConnectionPageUpdate()
{
	bool v = true;
	if (radio_ICS_passive->isChecked()) v = false; else v = true;
		TCPport_Edit->setEnabled(v);
		UDPport_Edit->setEnabled(v);
		TLSport_Edit->setEnabled(v);
		WANIP_Edit->setEnabled(v);
		GetIPbtn->setEnabled(v);
		check_UpdateIPstartup->setEnabled(v);
		check_notoverride->setEnabled(v);
	bool p = true;
	if (radio_OCS_socks->isChecked()) p = true; else p = false;
		SocksIP_Edit->setEnabled(p);
		SocksUsername_Edit->setEnabled(p);
		SocksPort_Edit->setEnabled(p);
		SocksPassword_Edit->setEnabled(p);
		check_SocksResolve->setEnabled(p);
}

void PreferencesDialog::applyConnectionPage()
{
	int val =0;
	if (radio_ICS_active->isChecked()) val = dcpp::SettingsManager::INCOMING_DIRECT;
	if (radio_ICS_manual->isChecked()) val = dcpp::SettingsManager::INCOMING_FIREWALL_NAT;
	if (radio_ICS_passive->isChecked()) val = dcpp::SettingsManager::INCOMING_FIREWALL_PASSIVE;
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::INCOMING_CONNECTIONS, val);
	val = 0;
	if (radio_OCS_direct->isChecked()) val = dcpp::SettingsManager::OUTGOING_DIRECT;
	if (radio_OCS_socks->isChecked()) val = dcpp::SettingsManager::OUTGOING_SOCKS5;
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::OUTGOING_CONNECTIONS, val);

	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::NO_IP_OVERRIDE, check_notoverride->isChecked());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::SOCKS_RESOLVE, check_SocksResolve->isChecked());
	
	SETAPPSETTING(i_UPDATEIP, check_UpdateIPstartup->isChecked());
	
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::EXTERNAL_IP, dcpp::Text::fromT(StilUtils::QtoTstr(WANIP_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::SOCKS_SERVER, dcpp::Text::fromT(StilUtils::QtoTstr(SocksIP_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::SOCKS_USER, dcpp::Text::fromT(StilUtils::QtoTstr(SocksUsername_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::SOCKS_PASSWORD, dcpp::Text::fromT(StilUtils::QtoTstr(SocksPassword_Edit->text()) ) );
	
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::TCP_PORT, TCPport_Edit->text().toInt() );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::UDP_PORT, UDPport_Edit->text().toInt() );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::TLS_PORT, TLSport_Edit->text().toInt() );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::SOCKS_PORT, SocksPort_Edit->text().toInt() );

}

void PreferencesDialog::initDownloadsPage() 
{
	connect(DLPath_btn, SIGNAL(clicked()), this, SLOT(DownloadsPageBrowse1()));
	connect(unfinishedDL_btn, SIGNAL(clicked()), this, SLOT(DownloadsPageBrowse2()));
	connect(ConfPubHubLists_btn, SIGNAL(clicked()), this, SLOT(DownloadsPageConfPublic()));
	
	DefDL_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOAD_DIRECTORY))));
	unfinishedDL_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(TEMP_DOWNLOAD_DIRECTORY))));
	httpproxy_Edit->setText(StilUtils::TstrtoQ(Text::toT(SETTING(HTTP_PROXY))));
	lim1_spin->setValue( SETTING(DOWNLOAD_SLOTS) );
	lim2_spin->setValue( SETTING(MAX_DOWNLOAD_SPEED) );
}

void PreferencesDialog::DownloadsPageBrowse1()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	//options |= QFileDialog::DontUseNativeDialog;
	QString directory;
	directory.clear();
	directory = QFileDialog::getExistingDirectory(this, tr("Select Folder"), StilUtils::TstrtoQ(Text::toT(SETTING(DOWNLOAD_DIRECTORY))), options);
	if (!directory.isEmpty()) DefDL_Edit->setText(directory);
}

void PreferencesDialog::DownloadsPageBrowse2()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	//options |= QFileDialog::DontUseNativeDialog;
	QString directory;
	directory.clear();
	directory = QFileDialog::getExistingDirectory(this, tr("Select Folder"), StilUtils::TstrtoQ(Text::toT(SETTING(TEMP_DOWNLOAD_DIRECTORY))), options);
	if (!directory.isEmpty()) unfinishedDL_Edit->setText(directory);
}

void PreferencesDialog::DownloadsPageConfPublic()
{
	// NOT IMPLEMENTED
}

void PreferencesDialog::applyDownloadsPage()
{
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::DOWNLOAD_DIRECTORY, dcpp::Text::fromT(StilUtils::QtoTstr(DefDL_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::TEMP_DOWNLOAD_DIRECTORY, dcpp::Text::fromT(StilUtils::QtoTstr(unfinishedDL_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::HTTP_PROXY, dcpp::Text::fromT(StilUtils::QtoTstr(httpproxy_Edit->text()) ) );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::DOWNLOAD_SLOTS, lim1_spin->value() );
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::MAX_DOWNLOAD_SPEED, lim2_spin->value() );
}

void PreferencesDialog::initDownloadsFavPage()
{
	connect(RenameFavFolder_Btn, SIGNAL(clicked()), this, SLOT(DownloadsFavPageRename()));
	connect(RemoveFavFolder_btn, SIGNAL(clicked()), this, SLOT(DownloadsFavPageRemove()));
	connect(favFolderAdd_btn, SIGNAL(clicked()), this, SLOT(DownloadsFavPageAdd()));
	
	StringPairList dirs = dcpp::FavoriteManager::getInstance()->getFavoriteDirs();
	for(StringPairIter j = dirs.begin(); j != dirs.end(); j++) 
	{
		QStringList lst;
		lst << StilUtils::TstrtoQ(Text::toT(j->second)) << StilUtils::TstrtoQ(Text::toT(j->first));
		QTreeWidgetItem *fItem = new QTreeWidgetItem(FavDLDir_tree, lst);
	}
	
	FavDLDir_tree->sortByColumn(0,Qt::AscendingOrder);
}

void PreferencesDialog::DownloadsFavPageRename()
{
	QTreeWidgetItem *it = FavDLDir_tree->currentItem();
	if (it)
	{
		bool ok;
		QString text = QInputDialog::getText(this, tr("Rename Favorite Directory"), tr("New Folder Name:"), QLineEdit::Normal, it->text(0) , &ok);
		if (ok && !text.isEmpty()) 
		if (dcpp::FavoriteManager::getInstance()->renameFavoriteDir( Text::fromT( StilUtils::QtoTstr( it->text(0) ) ), Text::fromT(StilUtils::QtoTstr(text))))
		{
			it->setText(0, text);
		}
	}
}

void PreferencesDialog::DownloadsFavPageRemove()
{
	QTreeWidgetItem *it = FavDLDir_tree->currentItem();
	if (it) 
	if (dcpp::FavoriteManager::getInstance()->removeFavoriteDir( Text::fromT( StilUtils::QtoTstr( it->text(0) ) ) ))
		{
			delete it;
		}
}

void PreferencesDialog::DownloadsFavPageAdd()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	//options |= QFileDialog::DontUseNativeDialog;
	QString directory;
	directory.clear();
	directory = QFileDialog::getExistingDirectory(this, tr("Select Folder"), "", options);
	if (!directory.isEmpty()) 
	{
		bool ok;
		QString text = QInputDialog::getText(this, tr("Add Favorite Directory"), tr("Folder Name:"), QLineEdit::Normal, directory, &ok);
		if (ok && !text.isEmpty())
		if	(dcpp::FavoriteManager::getInstance()->addFavoriteDir( dcpp::Text::fromT( StilUtils::QtoTstr(directory) ) , dcpp::Text::fromT( StilUtils::QtoTstr(text) ) ) )
		{
			QStringList lt;
			lt << text << directory;
			QTreeWidgetItem *fItm = new QTreeWidgetItem(FavDLDir_tree, lt);
		}
	}
}

void PreferencesDialog::accept() 
{
	// insert below wrapper 'interface options to settings'
		applyGeneralPage();
		applyConnectionPage();
		applyDownloadsPage();
	//
	AppSettings::AppSettingsMgr::getInstance()->save();
	dcpp::SettingsManager::getInstance()->save();
}
