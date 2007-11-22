/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwinimpl.h"

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f)  : QMainWindow(parent, f)
{
	setupUi(this);
	
	m_tabwin = new TabWidget( this );
	connect(m_tabwin, SIGNAL(currentChanged(int)), this, SLOT(slotCurrentTabChanged(int)) );
	setCentralWidget(m_tabwin);
	m_tabwin->setOpt(0);
	
	connect(actionIconSet, SIGNAL(triggered()), this, SLOT(funcNewIconSet()) );
	connect(actFromDir, SIGNAL(triggered()), this, SLOT(funcFromDir()) );
	
	toolBar->addAction(actionIconSet);
	toolBar->addAction(actExit);
	
}

MainWindowImpl::~MainWindowImpl()
{
	
}

void MainWindowImpl::slotCurrentTabChanged(int index)
{
	// Insert Code Here 
}

void MainWindowImpl::funcNewIconSet()
{
	m_tabwin->setCurrentIndex(m_tabwin->addTab((new composewin()),"Composing"));
}

void MainWindowImpl::funcFromDir()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	//if (!native->isChecked()) options |= QFileDialog::DontUseNativeDialog;
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Choose directory..."),
		"",
		options);
	if (!directory.isEmpty()) 
	{
		QFileDialog::Options options;
		//if (!native->isChecked()) options |= QFileDialog::DontUseNativeDialog;
		QString selectedFilter;
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Choose filename "),
			"",
			tr("ICP Files (*.icp)"),
			&selectedFilter,
			options);
		if (!fileName.isEmpty()) 
		{
			if ( (fileName.right(4)).toLower() !=".icp") fileName += ".icp";
			if (zstream::compressdir(directory,fileName)==Z_OK ) QMessageBox::information(this, "IconSet Collection Creator", "Done!");
		}
	}
}

