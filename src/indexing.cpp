/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
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

HashDlg::HashDlg(QWidget *parent, bool aAutoClose) : QDialog(parent), autoClose(aAutoClose)
{
	setupUi(this);
	setWindowTitle(StilUtils::TstrtoQ(T_("Creating file index...")));
	lbl_title->setText(StilUtils::TstrtoQ(CT_("Please wait while DC++ indexes your files (they won't be shared until they've been indexed)...")));

	connect(inback_btn, SIGNAL(clicked()), this, SLOT(accept()) );

	string tmp;
	startTime = GET_TICK();
	HashManager::getInstance()->getStats(tmp, startBytes, startFiles);

	updateStats();

	HashManager::getInstance()->setPriority(Thread::NORMAL);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateStats()));
	timer->start(1000);
}

HashDlg::~HashDlg()
{
	HashManager::getInstance()->setPriority(Thread::IDLE);
	delete timer;
}

void HashDlg::updateStats()
{
	string file;
	int64_t bytes = 0;
	size_t files = 0;
	uint32_t tick = GET_TICK();

	HashManager::getInstance()->getStats(file, bytes, files);
	if(bytes > startBytes) startBytes = bytes;
	if(files > startFiles) startFiles = files;

	if(autoClose && files == 0) 
	{
		accept();
		return;
	}
	
	double diff = tick - startTime;
	if(diff < 1000 || files == 0 || bytes == 0) 
	{
		lbl_stat0->setText(StilUtils::TstrtoQ( str(TF_("-.-- files/h, %1% files left") % (uint32_t)files).c_str()) );
		lbl_stat1->setText(StilUtils::TstrtoQ( str(TF_("-.-- B/s, %1% left") % Text::toT(Util::formatBytes(bytes))).c_str()) );
		lbl_stat2->setText(StilUtils::TstrtoQ( CT_("-:--:-- left")) );
		progress->setValue(0);
	} else {
		double filestat = (((double)(startFiles - files)) * 60 * 60 * 1000) / diff;
		double speedStat = (((double)(startBytes - bytes)) * 1000) / diff;

		lbl_stat0->setText(StilUtils::TstrtoQ( str(TF_("%1% files/h, %2% files left") % filestat % (uint32_t)files).c_str()));
		lbl_stat1->setText(StilUtils::TstrtoQ( str(TF_("%1%/s, %2% left") % Text::toT(Util::formatBytes((int64_t)speedStat)) % Text::toT(Util::formatBytes(bytes))).c_str()));

		if(filestat == 0 || speedStat == 0) {
			lbl_stat2->setText(StilUtils::TstrtoQ( CT_("-:--:-- left")));
		} else {
			double fs = files * 60 * 60 / filestat;
			double ss = bytes / speedStat;

			lbl_stat2->setText(StilUtils::TstrtoQ( str(TF_("%1% left") % Text::toT(Util::formatSeconds((int64_t)(fs + ss) / 2))).c_str()));
		}
	}

	if(files == 0) {
		lbl_Filename->setText(StilUtils::TstrtoQ( CT_("Done")));
	} else {
		lbl_Filename->setText(StilUtils::TstrtoQ( Text::toT(file).c_str()));
	}

	if(startFiles == 0 || startBytes == 0) {
		progress->setValue(0);
	} else {
		progress->setValue((int)(10000 * ((0.5 * (startFiles - files)/startFiles) + 0.5 * (startBytes - bytes) / startBytes)));
	}
}
