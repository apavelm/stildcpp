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

#include "indexing.h"

using namespace std;
using namespace dcpp;

HashDlg::HashDlg(QWidget *parent, bool aAutoClose) : QDialog(parent), autoClose(aAutoClose)
{
	setupUi(this);
	//setAttribute(Qt::WA_DeleteOnClose, true);
	//setWindowIcon(QIcon(":/images/pref_icon.png"));
	
	string tmp;
	startTime = GET_TICK();
	HashManager::getInstance()->getStats(tmp, startBytes, startFiles);
	HashManager::getInstance()->setPriority(Thread::NORMAL);
	updateStats("", 0, 0, 0);

	TimerManager::getInstance()->addListener(this);
}

HashDlg::~HashDlg()
{
	HashManager::getInstance()->setPriority(Thread::IDLE);
	TimerManager::getInstance()->removeListener(this);
}

void HashDlg::updateStats(string file, int64_t bytes, size_t files, uint32_t tick)
{
	if (bytes > startBytes) startBytes = bytes;
	if (files > startFiles) startFiles = files;

	double diff = tick - startTime;
	if (diff < 1000 || files == 0 || bytes == 0)
	{
		lbl_stat1->setText("-.-- B/s, " + StilUtils::fmtBytes(bytes) + " left");
		lbl_stat1->setText("-:--:-- left");
		progress->setValue(0);
	}
	else
	{
		double speedStat = (((double)(startBytes - bytes)) * 1000) / diff;

		lbl_stat1->setText( StilUtils::fmtBytes((int64_t)speedStat) + "/s, " + StilUtils::fmtBytes(bytes) + " left") ;
		if (speedStat == 0)
			lbl_stat1->setText("-:--:-- left");
		else
		{
			double ss = (double)bytes / speedStat;
			lbl_stat1->setText( string(Util::formatSeconds((int64_t)ss) + " left").c_str() );
		}
	}

	if (files == 0)
		lbl_Filename->setText( tr("Done") );
	else
		lbl_Filename->setText( file.c_str() );

	if (startFiles == 0 || startBytes == 0)
	{
		progress->setValue(0);
	}
	else
	{
		double progres = ((0.5 * (double)(startFiles - files)/(double)startFiles) + (0.5 * (double)(startBytes - bytes)/(double)startBytes));
		char buf[24];
		snprintf(buf, sizeof(buf), "%.0lf%%", progres * 100);
		progress->setValue(floor(progres));
		//gtk_progress_bar_set_text(GTK_PROGRESS_BAR(getWidget("progressbar")), buf);
	}
}

void HashDlg::on(TimerManagerListener::Second, uint32_t tics) throw()
{
	string file;
	int64_t bytes = 0;
	size_t files = 0;

	HashManager::getInstance()->getStats(file, bytes, files);

	//typedef Func4<Hash, string, int64_t, size_t, uint32_t> F4;
	//F4 *func = new F4(this, &Hash::updateStats_gui, file, bytes, files, GET_TICK());
	//WulforManager::get()->dispatchGuiFunc(func);
}
