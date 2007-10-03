#ifndef __QUICKCONNECTDLG_H__
#define __QUICKCONNECTDLG_H__

#include <QDialog>

#include "ui_quickconnect.h"

class TQuickConnectDialog : public QDialog, private Ui::quickConnectwin
{
private:
	QString &adr;
	int port;
private slots:
	void on_ok();
	void onPortChange(int);
	void onAdrChange(QString &);
public:
	TQuickConnectDialog(QWidget *);
signals:
	void on_Connect(QString &, int);
};

#endif // __QUICKCONNECTDLG_H__
