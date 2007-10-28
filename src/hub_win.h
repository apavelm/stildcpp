#ifndef __HUB_WIN_H__
#define __HUB_WIN_H__

#include <QFile>

#include "highlighter.h"
#include "mdi_c.h"

#include "ui_HUBWindow.h"

class HubWindow : public MdiChild, private Ui::mdiHUBwin
{
	Q_OBJECT
private:
	Highlighter *highlighter;
public:
	HubWindow(QWidget *parent, const QString &hubaddr);
	void setupeditor();
private slots:
	void send_msg();
};

#endif // __HUB_WIN_H__
