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

#ifndef __MDI_SYSLOG_H__
#define __MDI_SYSLOG_H__

#include "config.h"
#include "stilutils.h"
#include "mdi_c.h"

#include <QtGui>
#include <QtCore>

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/LogManager.h"
#include "client/Util.h"
//

#include "ui_syslog.h"

using namespace std;
using namespace dcpp;

class SysLogWindow : public MdiChild, private Ui::mdi_SysLog, private LogManagerListener
{
	Q_OBJECT
public:
	SysLogWindow(QWidget *parent = 0);
	~SysLogWindow();
	
	enum Status {
		STATUS_STATUS,
		STATUS_LAST
	};
	
private:
	void addLine(time_t t, const tstring& msg);
	
	// LogManagerListener
	virtual void on(Message, time_t tm, const string& message) throw();
private slots:
	void slotSpeak(time_t, tstring);
signals:
	void sigSpeak(time_t, tstring);
};

#endif // __MDI_SYSLOG_H__
