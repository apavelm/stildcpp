#include "quickconnectdlg.h"

TQuickConnectDialog::TQuickConnectDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	AdrLabel->setText(tr("Input Hub Address below:"));
	IconLabel->setPixmap(QPixmap(":/images/QuickCon.png"));
	adrline->setText(tr(""));
	spinBox->setValue(411);
	
	connect(this,SIGNAL(accepted()),this,SLOT(okFunc()));
	
	show();
}

void TQuickConnectDialog::okFunc()
{
	QString a=adrline->text();
	emit con_pressed(a,spinBox->value());
}
