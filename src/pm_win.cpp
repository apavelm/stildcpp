#include "pm_win.h"

PMWindow::PMWindow(QWidget *parent, const QString &nick) : MdiChild(parent)
{
	setupUi(this);
	type = 2;
	idText  = nick;
	setWindowTitle(tr("Private Chat with: ")+nick);
	editor->clear();
	setupeditor();
	connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(send_pm_msg()));
	connect(SendBtn,SIGNAL(pressed()),this,SLOT(send_pm_msg()));
}

void PMWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void PMWindow::send_pm_msg()
{
//	QFile file("/home/irq/stildcpp/src/main.cpp");
//	if (file.open(QFile::ReadOnly | QFile::Text))
//	editor->setPlainText(file.readAll());
}

// PRIVATE MESSAGE WINDOW
