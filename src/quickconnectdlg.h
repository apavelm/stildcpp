#ifndef __QUICKCONNECTDLG_H__
#define __QUICKCONNECTDLG_H__

#include <QDialog>

#include "ui_quickconnect.h"

class TQuickConnectDialog : public QDialog, private Ui::quickConnectwin
{
	Q_OBJECT
private slots:
	void okFunc();

public:
	TQuickConnectDialog(QWidget *);

signals:
	void con_pressed(QString, int);
};

#endif // __QUICKCONNECTDLG_H__
