/***************************************************************************
 *   Copyright (C) 2007 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail dot com (apavelm@gmail.com)                    *
 *   Copyright (C) 2007 by Yakov Suraev aka BigBiker                       *
 *   Mail: adminbsd on gmail dot com (adminbsd@gmail.com)                  *
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


#ifndef __INDEXING_H__
#define __INDEXING_H__


#include "config.h"
#include "stilutils.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/TimerManager.h"
#include "client/HashManager.h"
//

#include "ui_hashman.h"

class HashDlg: public QDialog, private Ui::dlgIndexing 
	,public TimerManagerListener
{
	Q_OBJECT
public:
	HashDlg(QWidget *parent, bool aAutoClose);
	~HashDlg();

	//int run() { return createDialog(IDD_HASH_PROGRESS); }
private:
	// GUI functions
	void updateStats(std::string file, int64_t bytes, size_t files, uint32_t tick);

	// Client callbacks
	virtual void on(TimerManagerListener::Second, uint32_t tics) throw();
	
	bool autoClose;
	int64_t startBytes;
	size_t startFiles;
	uint32_t startTime;
};



#endif // __INDEXING_H__
