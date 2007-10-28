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

#include "config.h"
#include "preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowIcon(QIcon(":/images/pref_icon.png"));

	categoryList->header()->hide();

	initCategoryList();
	initLookNFeelPage();
	
	categoryList->setCurrentItem(categoryList->topLevelItem(0));
	show();	
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

void PreferencesDialog::accept() 
{
	// accept settings
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
		it2->setText(0,tr("Download"));
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
		it3->setText(0,tr("Share"));
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

void PreferencesDialog::initLookNFeelPage() 
{
	chkHiddenStart->setChecked(false);
//	connect(autoConnectCheck, SIGNAL(toggled(bool)), d->config, SLOT(setAutoconnect(bool)));
}

void PreferencesDialog::on_categoryList_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *)
{
	if (!c) return;
	widgetStack->setCurrentIndex(c->data(0, Qt::UserRole).toInt());
	currentLabel->setText("<h3>" + c->text(0) + "</h3>");
}



