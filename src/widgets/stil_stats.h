/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
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

#ifndef _STIL_STATS_H
#define _STIL_STATS_H

#include "../client/stdinc.h"
#include "../client/DCPlusPlus.h"
#include "../client/Socket.h"
#include "../client/TimerManager.h"
#include "../client/SettingsManager.h"

#include <QtGui>

using namespace std;
using namespace dcpp;

class QMyStats : public QWidget
{
	Q_OBJECT
public:
	QMyStats(QWidget *parent = 0);
	~QMyStats();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
private:
	QTimer * timer;
	void initSecond();
	
	enum { PIX_PER_SEC = 2 }; // Pixels per second
	enum { LINE_HEIGHT = 10 };
	enum { AVG_SIZE = 5 };
	
	struct Stat {
		Stat() : scroll(0), speed(0) { }
		Stat(uint32_t aScroll, int64_t aSpeed) : scroll(aScroll), speed(aSpeed) { }
		uint32_t scroll;
		int64_t speed;
	};
	typedef QList<Stat> StatList;
	typedef StatList::iterator StatIter;
	typedef QList<int64_t> AvgList;
	typedef AvgList::iterator AvgIter;
	StatList up;
	StatList down;
	AvgList upAvg;
	AvgList downAvg;

	long width;
	long height;
	long twidth;
	uint32_t lastTick;
	uint32_t scrollTick;
	int64_t lastUp;
	int64_t lastDown;
	int64_t max;
	
	void addTick(int64_t bdiff, int64_t tdiff, StatList& lst, AvgList& avg, int scroll);
	
private slots:
	bool eachSecond();

protected:
	void paintEvent(QPaintEvent *);
	void paintBorder();
	void setZoomPoint(int);
	void paintFreq();
	void paintScale();
	void paintControl();
	void initValue();
	
private:
	QColor col_up;
	QColor col_down;
	QColor col_bg;
	QColor col_axis;
	QColor col_scale;
	
	int64_t speed1;
	int64_t speed2;
	int64_t speed3;
	int64_t speed4;
	int64_t speed5;
	int64_t speed6;

	int zoom_point;
	int start_point;
	int end_point;
	int typeFFT;
	double *magnitudeLin;
	double *magnitudeLog;
	double min_val_lin;
	double max_val_lin;
	double min_val_log;
	double max_val_log;
	double range_freq;
	bool lockFFT;
	QString wavFile;

};

#endif
