#include "hub_win.h"

HubWindow::HubWindow(QWidget *parent, const QString &hubaddr) : MdiChild(parent)
{	
	setupUi(this);
	type = 1;
	idText  = hubaddr;
	setWindowTitle(tr("Hub: ")+hubaddr);
	editor->clear();
	setupeditor();
	connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(send_msg()));
	connect(SendBtn,SIGNAL(pressed()),this,SLOT(send_msg()));
}

void HubWindow::setupeditor()
{
	QFont font;
	font.setFamily("Tahoma");
	font.setFixedPitch(true);
	font.setPointSize(10);

	editor->setFont(font);

	highlighter = new Highlighter(editor->document());
}

void HubWindow::send_msg()
{
//	QFile file("/home/irq/stildcpp/src/main.cpp");
//	if (file.open(QFile::ReadOnly | QFile::Text))
//	editor->setPlainText(file.readAll());
}

// HubWindow
