#ifndef __PM_WIN_H__
#define __PM_WIN_H__

#include <QFile>

#include "highlighter.h"
#include "mdi_c.h"

#include "ui_PMWindow.h"

class PMWindow : public MdiChild, private Ui::mdiPMwin
{
	Q_OBJECT
private:
	Highlighter *highlighter;
public:
	PMWindow(QWidget *parent, const QString &nick);
	void setupeditor();
private slots:
	void send_pm_msg();
};

// PRIVATE MESSAGE WINDOW

#endif // __PM_WIN_H__
