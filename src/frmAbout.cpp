#include "frmAbout.h"

//

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);

	VerLabel->setText(tr("Version 0.1"));
	label_2->setPixmap(QPixmap(":/images/icon.png"));

	connect(Button, SIGNAL(clicked()), this, SLOT(accept()));
	show();
}

//
