#ifndef FRMABOUT_H
#define FRMABOUT_H
//

#include <QDialog>

#include "ui_about.h"

class AboutDialog : public QDialog, private Ui::formAbout
{
public:
	AboutDialog(QWidget *);
};
#endif




