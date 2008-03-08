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
#include "../client/Util.h"

#include <QtGui>
#include <QtCore>

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
	QPixmap pixmap;
	QList<int64_t> dlist, uplist;
	QPointF *d_p, *u_p;

	uint32_t lastTick;
	int64_t lastUp;
	int64_t lastDown;
protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void paintBorder(QPainter *painter);
	void paintGraph(QPainter *painter);
	void paintScale(QPainter *painter);
	void setMaxValue(int64_t);
	void refreshPixmap();
	void calcCoord();
	int64_t findMax(const QList<int64_t> & lst);
	
	void timerEvent(QTimerEvent *event);
	
private:
	QColor col_up;
	QColor col_down;
	QColor col_bg;
	QColor col_axis;
	QColor col_scale;
	
	int64_t speed[7];
};

#endif
