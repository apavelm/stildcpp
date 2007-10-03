#include "config.h"
#include "preferencesdialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	categoryList->header()->hide();

	initCategoryList();
	initLookNFeelPage();
	
	categoryList->setCurrentItem(categoryList->topLevelItem(0));
	show();	
}

/*

void PreferencesDialog::on_fontButton_clicked() {
	ASSERT;
	bool ok;
	QFont newfont = QFontDialog::getFont(&ok, fontButton->font(), this);
	if (ok) {
		d->config->setFont(newfont);
		setFontString(newfont);
	}
}

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

void PreferencesDialog::on_closeButton_clicked() 
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
		it0->setText(0,tr("Look and Feel"));
		it0->setIcon(0,QIcon(":/images/icon_error.png"));
	QTreeWidgetItem *it1 = new QTreeWidgetItem(categoryList);
		it1->setText(0,tr("User"));
		it1->setIcon(0,QIcon(":/images/icon_mail.png"));
	QTreeWidgetItem *it2 = new QTreeWidgetItem(categoryList);
		it2->setText(0,tr("Connection"));
		it2->setIcon(0,QIcon(":/images/icon.png"));
		
		
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



