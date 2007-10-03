#include "quickconnectdlg.h"

TQuickConnectDialog::TQuickConnectDialog(QWidget *parent) : QDialog(parent) 
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	AdrLabel->setText(tr("Input Hub Address below:"));
	IconLabel->setPixmap(QPixmap(":/images/QuickCon.png"));
	adrline->setText(tr(""));
	spinBox->setValue(411);
	port=411;
	adr=tr("");

	connect(CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(onPortChange(int)));
	connect(adrline, SIGNAL(returnPressed()), this, SLOT(on_ok()));
	connect(adrline, SIGNAL(textChanged()), this, SLOT(onAdrChange(QString &)));

	show();
}
void TQuickConnectDialog::on_ok()
{	
	emit on_Connect(adr,port);
	accept();
}

void TQuickConnectDialog::onPortChange(int n)
{
	port=n;
}

void TQuickConnectDialog::onAdrChange(QString &s)
{
	adr=s;
}

