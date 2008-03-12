/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
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
	initDownloadsPreviewPage();
	initDownloadsQueuePage();	
	initSharingPage();
	initMessagesPage();
	initLNFPage();
	
	initLNFTabsPage();
	
	categoryList->setCurrentItem(categoryList->topLevelItem(0));
	show();
}

void PreferencesDialog::on_categoryList_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	widgetStack->setCurrentIndex(c->data(0, Qt::UserRole).toInt());
	currentLabel->setText("<h3>" + c->text(0) + "</h3>");
}

void PreferencesDialog::on_okBtn_clicked() 
{
	accept();
	close();
}

void PreferencesDialog::initCategoryList() 
{
	QTreeWidgetItem *it0 = new QTreeWidgetItem(categoryList);
		it0->setText(0,tr("Personal information"));
		it0->setIcon(0,QIcon(":/images/icon_error.png"));
	QTreeWidgetItem *it1 = new QTreeWidgetItem(categoryList);
		it1->setText(0,tr("Connection settings"));
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
					QTreeWidgetItem *it5d = new QTreeWidgetItem(it5);
						it5d->setText(0,tr("Tabs and Windows"));
						it5d->setIcon(0,QIcon(":/images/icon.png"));
		
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
	connections->clear();
	for(StringIter i = dcpp::SettingsManager::connectionSpeeds.begin(); i != dcpp::SettingsManager::connectionSpeeds.end(); ++i, ++j) 
	{
		connections->addItem(StilUtils::TstrtoQ(Text::toT(*i)));
		if(selected == 0 && SETTING(UPLOAD_SPEED) == *i) {
			selected = j; 
		}
	}
	connections->setCurrentIndex(selected);
	
	// Adding charset encoding list to combobox
	QString t;
	int sel = -1;
	cmb_Encoding->clear();
	QList<QByteArray> lst = QTextCodec::availableCodecs();
	for(int i=0;i< lst.size(); i++) 
	{
		t = lst.at(i);
		cmb_Encoding->addItem(t);
		if(sel == -1 && APPSTRING(s_DEFCHARSET) == t) sel = i;
	}
	if (sel == -1) sel = 0;
	cmb_Encoding->setCurrentIndex(sel);
}

void PreferencesDialog::applyGeneralPage()
{
	SettingsManager::getInstance()->set(SettingsManager::NICK, dcpp::Text::fromT(StilUtils::QtoTstr(NickEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::EMAIL, dcpp::Text::fromT(StilUtils::QtoTstr(EmailEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::DESCRIPTION, dcpp::Text::fromT(StilUtils::QtoTstr(DescEdit->text()) ) );
	SettingsManager::getInstance()->set(SettingsManager::UPLOAD_SPEED, dcpp::Text::fromT(StilUtils::QtoTstr(connections->currentText()) ) );
	SETAPPSTRING(s_DEFCHARSET, cmb_Encoding->currentText());
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
	new ConfPubHubDlg(this);
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
		/*QTreeWidgetItem *fItem = */new QTreeWidgetItem(FavDLDir_tree, lst);
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
	if (dcpp::FavoriteManager::getInstance()->removeFavoriteDir( Text::fromT( StilUtils::QtoTstr( it->text(1) ) ) ))
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
		QDir ddd(directory);
		QString text = QInputDialog::getText(this, tr("Add Favorite Directory"), tr("Folder Name:"), QLineEdit::Normal, ddd.dirName(), &ok);
		if (ok && !text.isEmpty())
		if	(dcpp::FavoriteManager::getInstance()->addFavoriteDir( dcpp::Text::fromT( StilUtils::QtoTstr(directory) ) , dcpp::Text::fromT( StilUtils::QtoTstr(text) ) ) )
		{
			QStringList lt;
			lt << text << directory;
			/*QTreeWidgetItem *fItm = */new QTreeWidgetItem(FavDLDir_tree, lt);
		}
	}
}

void PreferencesDialog::applyDownloadsFavPage()
{
	
}

void PreferencesDialog::initDownloadsPreviewPage()
{
	
}

void PreferencesDialog::applyDownloadsPreviewPage()
{
	
}

int download_queue_list_checked[] = {
	dcpp::SettingsManager::PRIO_LOWEST,
	dcpp::SettingsManager::AUTODROP_ALL,
	dcpp::SettingsManager::AUTODROP_FILELISTS,
	dcpp::SettingsManager::AUTODROP_DISCONNECT,
	dcpp::SettingsManager::AUTO_SEARCH,
	dcpp::SettingsManager::AUTO_SEARCH_AUTO_MATCH,
	dcpp::SettingsManager::SKIP_ZERO_BYTE,
	dcpp::SettingsManager::DONT_DL_ALREADY_SHARED,
	dcpp::SettingsManager::DONT_DL_ALREADY_QUEUED,
	dcpp::SettingsManager::ANTI_FRAG
};

const char* download_queue_list_names[] = {
	N_("Set lowest prio for newly added files larger than Low prio size"),
	N_("Autodrop slow sources for all queue items (except filelists)"),
	N_("Remove slow filelists"),
	N_("Don't remove the source when autodropping, only disconnect"),
	N_("Automatically search for alternative download locations"),
	N_("Automatically match queue for auto search hits"),
	N_("Skip zero-byte files"),
	N_("Don't download files already in share"),
	N_("Don't download files already in the queue"),
	N_("Use antifragmentation method for downloads")
};

void PreferencesDialog::initDownloadsQueuePage()
{
	group_autopriority->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Auto priority settings"))));
	lbl_dlqueue_highest->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Highest prio max size"))));
	lbl_dlqueue_high->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("High prio max size"))));
	lbl_dlqueue_normal->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Normal prio max size"))));
	lbl_dlqueue_low->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Low prio max size"))));
	
	group_autodrop->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Autodrop settings"))));
	edt_dlqueue_drop->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Drop sources below"))));
	edt_dlqueue_minelapsed->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Min elapsed"))));
	edt_dlqueue_minsources->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Min sources online"))));
	edt_dlqueue_checkevery->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Check every"))));
	edt_dlqueue_maxinactivity->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Max inactivity"))));
	edt_dlqueue_minfilesize->setText(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Min filesize"))));
	
	group_otherqueue->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Other queue options"))));
	
	edt_dlqueue_highest->setText(QString::number(SETTING(PRIO_HIGHEST_SIZE)));
	edt_dlqueue_high->setText(QString::number(SETTING(PRIO_HIGH_SIZE)));
	edt_dlqueue_normal->setText(QString::number(SETTING(PRIO_NORMAL_SIZE)));
	edt_dlqueue_low->setText(QString::number(SETTING(PRIO_LOW_SIZE)));
	edt_dlqueue_drop->setText(QString::number(SETTING(AUTODROP_SPEED)));
	edt_dlqueue_minelapsed->setText(QString::number(SETTING(AUTODROP_ELAPSED)));
	edt_dlqueue_minsources->setText(QString::number(SETTING(AUTODROP_MINSOURCES)));
	edt_dlqueue_checkevery->setText(QString::number(SETTING(AUTODROP_INTERVAL)));
	edt_dlqueue_maxinactivity->setText(QString::number(SETTING(AUTODROP_INACTIVITY)));
	edt_dlqueue_minfilesize->setText(QString::number(SETTING(AUTODROP_FILESIZE)));
	
	//list_queueoptions
	QListWidgetItem * w;
	Qt::CheckState chk;
	for (int i = 0; i < 10 /* Just counted */; i++)
	{
		w = new QListWidgetItem(list_queueoptions);
		w->setText(StilUtils::TstrtoQ(dcpp::Text::toT(download_queue_list_names[i])) );
		chk = (dcpp::SettingsManager::getInstance()->getBool(static_cast<dcpp::SettingsManager::IntSetting>(download_queue_list_checked[i])) ? Qt::Checked : Qt::Unchecked);
		w->setCheckState(chk);
	}
}

void PreferencesDialog::applyDownloadsQueuePage()
{
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::PRIO_HIGHEST_SIZE, edt_dlqueue_highest->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::PRIO_HIGH_SIZE, edt_dlqueue_high->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::PRIO_NORMAL_SIZE, edt_dlqueue_normal->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::PRIO_LOW_SIZE, edt_dlqueue_low->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_SPEED, edt_dlqueue_drop->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_ELAPSED, edt_dlqueue_minelapsed->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_MINSOURCES, edt_dlqueue_minsources->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_INTERVAL, edt_dlqueue_checkevery->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_INACTIVITY, edt_dlqueue_maxinactivity->text().toInt());
	dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_FILESIZE, edt_dlqueue_minfilesize->text().toInt());
	
	//list_queueoptions
	QListWidgetItem * w;
	Qt::CheckState chk;
	bool b;
	for (int i = 0; i < 10 /* Just counted */; i++)
	{
		w = list_queueoptions->item(i);
		chk = w->checkState();
		b = chk == Qt::Checked ? true : false;
		dcpp::SettingsManager::getInstance()->set(static_cast<dcpp::SettingsManager::IntSetting>(download_queue_list_checked[i]), static_cast<int>(b) );
	}
	
	if(SETTING(AUTODROP_INTERVAL) < 1)
		dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_INTERVAL, 1);
	if(SETTING(AUTODROP_ELAPSED) < 1)
		dcpp::SettingsManager::getInstance()->set(dcpp::SettingsManager::AUTODROP_ELAPSED, 1);
}

void PreferencesDialog::initSharingPage()
{
	connect(RenameShare_btn, SIGNAL(clicked()), this, SLOT(SharingPageRename()));
	connect(RemoveShare_btn, SIGNAL(clicked()), this, SLOT(SharingPageRemove()));
	connect(AddShare_btn, SIGNAL(clicked()), this, SLOT(SharingPageAdd()));
	connect(check_SharedHidden, SIGNAL(stateChanged(int)), this, SLOT(SharingPageHidden(int)));
	
	StringPairList dirs = ShareManager::getInstance()->getDirectories();
	for(StringPairIter j = dirs.begin(); j != dirs.end(); j++) 
	{
		QStringList row;
		row << StilUtils::TstrtoQ(Text::toT(j->first)) << StilUtils::TstrtoQ(Text::toT(j->second)) << StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize(j->second));
		/*QTreeWidgetItem *fItm = */new QTreeWidgetItem(SharedDir_tree, row);
	}
	
	lbl_TotalSize->setText(tr("Total Size: ")+StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize()));
	
	extraSlots_spin->setValue( SETTING(MIN_UPLOAD_SPEED) );
	UploadSlots_spin->setValue( SETTING(SLOTS) );
	check_SharedHidden->setChecked( SETTING(SHARE_HIDDEN) ); 
	
}

void PreferencesDialog::SharingPageRename()
{
	QTreeWidgetItem *it = SharedDir_tree->currentItem();
	if (it)
	{
		bool ok;
		QString text = QInputDialog::getText(this, tr("Rename Shared Directory"), tr("New Folder Name:"), QLineEdit::Normal, it->text(0) , &ok);
		if (ok && !text.isEmpty()) 
		{
			ShareManager::getInstance()->renameDirectory( Text::fromT(StilUtils::QtoTstr(it->text(0)) ), Text::fromT(StilUtils::QtoTstr(text) ) );
			it->setText(0, text);
		}
	ShareManager::getInstance()->setDirty();
	}
}

void PreferencesDialog::SharingPageRemove()
{
	QTreeWidgetItem *it = SharedDir_tree->currentItem();
	if (it) 
	{
		ShareManager::getInstance()->removeDirectory(Text::fromT(StilUtils::QtoTstr( it->text(1) ))) ;
		delete it;
	}
	ShareManager::getInstance()->setDirty();
	lbl_TotalSize->setText(StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize()));
}

void PreferencesDialog::SharingPageAdd()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	//options |= QFileDialog::DontUseNativeDialog;
	QString directory;
	directory.clear();
	directory = QFileDialog::getExistingDirectory(this, tr("Select Folder"), "", options);
	if (!directory.isEmpty()) 
	{
		bool ok;
		QDir ddd(directory);
		QString text = QInputDialog::getText(this, tr("Add Shared Directory"), tr("Folder Name:"), QLineEdit::Normal, ddd.dirName(), &ok);
		if (ok && !text.isEmpty())
		{
			ShareManager::getInstance()->addDirectory(Text::fromT(StilUtils::QtoTstr(directory) ), Text::fromT(StilUtils::QtoTstr(text)));
			QStringList lt;
			lt << text << directory << StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize(Text::fromT(StilUtils::QtoTstr(directory))));
			/*QTreeWidgetItem *fItm = */new QTreeWidgetItem(SharedDir_tree, lt);
			
			ShareManager::getInstance()->setDirty();
			lbl_TotalSize->setText(StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize() ));
			
			MainWindowImpl::getInstance()->ShowHashDlg(true);
		}
	}
}

void PreferencesDialog::SharingPageHidden(int /*state*/)
{
	// Save the checkbox state so that ShareManager knows to include/exclude hidden files
//	Item i = items[1]; // The checkbox. Explicit index used - bad!
//	SettingsManager::getInstance()->set((SettingsManager::IntSetting)i.setting, state);

	// Refresh the share. This is a blocking refresh. Might cause problems?
	// Hopefully people won't click the checkbox enough for it to be an issue. :-)
	ShareManager::getInstance()->setDirty();
	ShareManager::getInstance()->refresh(true, false, true);

	// Clear the GUI list, for insertion of updated shares
	SharedDir_tree->clear();
	StringPairList dirs = ShareManager::getInstance()->getDirectories();
	for(StringPairIter j = dirs.begin(); j != dirs.end(); j++)
	{
		QStringList row;
		row << StilUtils::TstrtoQ(Text::toT(j->first)) << StilUtils::TstrtoQ(Text::toT(j->second)) << StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize(j->second));
		/*QTreeWidgetItem *fItm = */new QTreeWidgetItem(SharedDir_tree, row);
	}

	// Display the new total share size
	lbl_TotalSize->setText(tr("Total Size: ")+StilUtils::fmtBytes(ShareManager::getInstance()->getShareSize()));
}

void PreferencesDialog::applySharingPage()
{
	SettingsManager::getInstance()->set(SettingsManager::MIN_UPLOAD_SPEED, extraSlots_spin->value() );
	SettingsManager::getInstance()->set(SettingsManager::SLOTS, UploadSlots_spin->value() );
	
	SettingsManager::getInstance()->set(SettingsManager::SHARE_HIDDEN, check_SharedHidden->isChecked() );

	if(SETTING(SLOTS) < 1)
		SettingsManager::getInstance()->set(SettingsManager::SLOTS, 1);
	ShareManager::getInstance()->refresh();
}

void PreferencesDialog::initMessagesPage()
{
	connect(btn_edt_timestampfmt, SIGNAL(clicked()), this, SLOT(MessagesPageHelp()));
	connect(btn_edit_icp_path, SIGNAL(clicked()), this, SLOT(MessagesPageBrowse()));
	chk_smiles->setChecked( APPSETTING(i_SHOWSMILES) );
	chk_timestamp->setChecked( BOOLSETTING(TIME_STAMPS) );
	chk_joins->setChecked( BOOLSETTING(SHOW_JOINS) );
	chk_joins_fav->setChecked( BOOLSETTING(FAV_SHOW_JOINS) );
	edt_timestampfmt->setText(StilUtils::TstrtoQ(Text::toT(SETTING(TIME_STAMPS_FORMAT))));
	edit_icp_path->setText(APPSTRING(s_ICONSETPATH));
}

void PreferencesDialog::MessagesPageBrowse()
{
	QString icp;
	icp.clear();
	icp = QFileDialog::getOpenFileName(this, tr("Select Folder"), "", tr("IconSet Pack (*.icp)"));
	if (!icp.isEmpty()) edit_icp_path->setText( icp );
}

void PreferencesDialog::MessagesPageHelp()
{
QMessageBox::information(this, tr("Timestamps Help"), tr(
"\n- %a - Abbreviated weekday name\n \
- %A - Full weakday name\n \
- %b - Abbreviated month name\n \
- %B - Full month name\n \
- %c - Date and time represention appropriate to locale\n \
- %d - Day of month as decimal number (01-31)\n \
- %H - Hour in 24-hour format (00-23)\n \
- %I - Hour in 12-hour format (00-12)\n \
- %j - Day of year as decimal number (001-366)\n \
- %m - Month as decimal format (00-12)\n \
- %M - Minute as decimal format (00-59)\n \
- %p - Current locale AM/PM indicator for 12-hour clock\n \
- %S - Second as decimal number (00-59)\n \
- %U - Week of the year as decimal number. With Sunday as first day of week (00-53)\n \
- %w - Weekday as decimal number (0-6, Sunday is 0)\n \
- %W - Week of the year as decimal number. With Monday as first day of week (00-53)\n \
- %x - Day representation for current locale\n \
- %X - Time representation for current locale\n \
- %y - Year without century as decimal number (00-99)\n \
- %Y - Year with century as decimal number\n \
\nDefault: %H:%M"
));
}

void PreferencesDialog::applyMessagesPage()
{
	SETAPPSETTING(i_SHOWSMILES, chk_smiles->isChecked());

	SettingsManager::getInstance()->set(SettingsManager::TIME_STAMPS, chk_timestamp->isChecked() );
	SettingsManager::getInstance()->set(SettingsManager::SHOW_JOINS, chk_joins->isChecked() );
	SettingsManager::getInstance()->set(SettingsManager::FAV_SHOW_JOINS, chk_joins_fav->isChecked() );

	SettingsManager::getInstance()->set(SettingsManager::TIME_STAMPS_FORMAT, dcpp::Text::fromT(StilUtils::QtoTstr(edt_timestampfmt->text()) ) );
	SETAPPSTRING(s_ICONSETPATH, edit_icp_path->text());
}

void PreferencesDialog::initLNFPage()
{
	chk_swpUserList->setChecked( APPSETTING(i_HUBLEFTSIDE) );
}

void PreferencesDialog::applyLNFPage()
{
	SETAPPSETTING(i_HUBLEFTSIDE, chk_swpUserList->isChecked());
}

int lnf_tabs_autoopen_list_checked[] = {
	dcpp::SettingsManager::OPEN_SYSTEM_LOG,
	dcpp::SettingsManager::OPEN_FAVORITE_USERS,
	dcpp::SettingsManager::OPEN_QUEUE,
	dcpp::SettingsManager::OPEN_FINISHED_DOWNLOADS,
	dcpp::SettingsManager::OPEN_WAITING_USERS,
	dcpp::SettingsManager::OPEN_FINISHED_UPLOADS,
	dcpp::SettingsManager::OPEN_SEARCH_SPY,
	dcpp::SettingsManager::OPEN_NETWORK_STATISTICS,
	dcpp::SettingsManager::OPEN_NOTEPAD,
	dcpp::SettingsManager::OPEN_PUBLIC,
	dcpp::SettingsManager::OPEN_FAVORITE_HUBS
};

const char* lnf_tabs_autoopen_list_names[] = {
	N_("System Log"),
	N_("Favorite Users"),
	N_("Download Queue"),
	N_("Finished Downloads"),
	N_("Waiting Users"),
	N_("Finished Uploads"),
	N_("Search Spy"),
	N_("Network Statistics"),
	N_("Notepad"),
	N_("Public Hubs"),
	N_("Favorite Hubs")
};

int lnf_tabs_winopt_list_checked[] = {
	dcpp::SettingsManager::POPUP_PMS,
	dcpp::SettingsManager::POPUP_HUB_PMS,
	dcpp::SettingsManager::POPUP_BOT_PMS,
	dcpp::SettingsManager::POPUNDER_FILELIST,
	dcpp::SettingsManager::POPUNDER_PM,
	dcpp::SettingsManager::JOIN_OPEN_NEW_WINDOW,
	dcpp::SettingsManager::IGNORE_HUB_PMS,
	dcpp::SettingsManager::IGNORE_BOT_PMS,
	dcpp::SettingsManager::TOGGLE_ACTIVE_WINDOW,
	dcpp::SettingsManager::PROMPT_PASSWORD
};

const char* lnf_tabs_winopt_list_names[] = {
	N_("Open private messages in their own window"),
	N_("Open private messages from bots in their own window"),
	N_("Open private messages from the hub in their own window"),
	N_("Open new file list windows in the background"),
	N_("Open new private message windows in the background"),
	N_("Open new window when using /join"),
	N_("Ignore private messages from the hub"),
	N_("Ignore private messages from bots"),
	N_("Toggle window when selecting an active tab"),
	N_("Popup box to input password for hubs")
};

int lnf_tabs_confirmdlg_list_checked[] = {
	dcpp::SettingsManager::CONFIRM_EXIT,
	dcpp::SettingsManager::CONFIRM_HUB_REMOVAL,
	dcpp::SettingsManager::CONFIRM_ITEM_REMOVAL
};

const char* lnf_tabs_confirmdlg_list_names[] = {
	N_("Confirm application exit"),
	N_("Confirm favorite hub removal"),
	N_("Confirm item removal in download queue")
};

void PreferencesDialog::initLNFTabsPage()
{
	
	group_autoopen->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Auto-open at startup"))));
	group_winoptions->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Window options"))));
	group_confdlgoptions->setTitle(StilUtils::TstrtoQ(dcpp::Text::toT(N_("Confirm dialog options"))));
	
	QListWidgetItem * w;
	Qt::CheckState chk;
	for (int i = 0; i < 11 /* Just counted */; i++)
	{
		w = new QListWidgetItem(list_autoopen);
		w->setText(StilUtils::TstrtoQ(dcpp::Text::toT(lnf_tabs_autoopen_list_names[i])) );
		chk = (dcpp::SettingsManager::getInstance()->getBool((dcpp::SettingsManager::IntSetting)lnf_tabs_autoopen_list_checked[i]) ? Qt::Checked : Qt::Unchecked);
		w->setCheckState(chk);
	}
	
	for (int i = 0; i < 10 /* Just counted */; i++)
	{
		w = new QListWidgetItem(list_winoptions);
		w->setText(StilUtils::TstrtoQ(dcpp::Text::toT(lnf_tabs_winopt_list_names[i])) );
		chk = (dcpp::SettingsManager::getInstance()->getBool((dcpp::SettingsManager::IntSetting)lnf_tabs_winopt_list_checked[i]) ? Qt::Checked : Qt::Unchecked);
		w->setCheckState(chk);
	}

	for (int i = 0; i < 3 /* Just counted */; i++)
	{
		w = new QListWidgetItem(list_confdlgoptions);
		w->setText(StilUtils::TstrtoQ(dcpp::Text::toT(lnf_tabs_confirmdlg_list_names[i])) );
		chk = (dcpp::SettingsManager::getInstance()->getBool((dcpp::SettingsManager::IntSetting)lnf_tabs_confirmdlg_list_checked[i]) ? Qt::Checked : Qt::Unchecked);
		w->setCheckState(chk);
	}
	
}

void PreferencesDialog::applyLNFTabsPage()
{
	QListWidgetItem * w;
	Qt::CheckState chk;
	bool b;
	for (int i = 0; i < 11 /* Just counted */; i++)
	{
		w = list_autoopen->item(i);
		chk = w->checkState();
		b = chk == Qt::Checked ? true : false;
		dcpp::SettingsManager::getInstance()->set((dcpp::SettingsManager::IntSetting)lnf_tabs_autoopen_list_checked[i], static_cast<int>(b) );
	}
	
	for (int i = 0; i < 10 /* Just counted */; i++)
	{
		w = list_winoptions->item(i);
		chk = w->checkState();
		b = chk == Qt::Checked ? true : false;
		dcpp::SettingsManager::getInstance()->set((dcpp::SettingsManager::IntSetting)lnf_tabs_winopt_list_checked[i], static_cast<int>(b) );
	}
	
	for (int i = 0; i < 3 /* Just counted */; i++)
	{
		w = list_confdlgoptions->item(i);
		chk = w->checkState();
		b = chk == Qt::Checked ? true : false;
		dcpp::SettingsManager::getInstance()->set((dcpp::SettingsManager::IntSetting)lnf_tabs_confirmdlg_list_checked[i], static_cast<int>(b) );
	}
}

void PreferencesDialog::accept()
{
		applyGeneralPage();
		applyConnectionPage();
		applyDownloadsPage();
		applyDownloadsFavPage();
		applyDownloadsPreviewPage();
		applyDownloadsQueuePage();
		applySharingPage();
		applyMessagesPage();
		applyLNFPage();
		
		applyLNFTabsPage();
		

	AppSettings::AppSettingsMgr::getInstance()->save();
	dcpp::SettingsManager::getInstance()->save();
}
