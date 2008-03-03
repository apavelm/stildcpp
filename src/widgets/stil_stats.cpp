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


#include "stil_stats.h"

QMyStats::QMyStats(QWidget *parent) : QWidget(parent),
	lastTick(GET_TICK()),
	lastUp(Socket::getTotalUp()),
	lastDown(Socket::getTotalDown()),
	col_up ( SETTING(UPLOAD_BAR_COLOR)),
	col_down ( SETTING(DOWNLOAD_BAR_COLOR)),
	col_bg(Qt::black),
	col_axis(Qt::yellow),
	col_scale(Qt::darkGray)
{
	setMaxValue( (lastUp > lastDown ? lastUp : lastDown) );

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
	timer->start(1000);
}

QMyStats::~QMyStats()
{
	delete timer;
}

bool QMyStats::eachSecond()
{
	uint32_t tick = GET_TICK();
	uint32_t tdiff = tick - lastTick;
	if(tdiff == 0)
		return true;

	int64_t d = Socket::getTotalDown();
	int64_t ddiff = d - lastDown;
	int64_t u = Socket::getTotalUp();
	int64_t udiff = u - lastUp;
	
	// 334 = 668 /2; 2 pixel per 1 line minimum
	if ( dlist.count() >= 334) dlist.removeAt(0);
	dlist.append(ddiff);
	
	if ( uplist.count() >= 334) uplist.removeAt(0);
	uplist.append(udiff);
	
	lastTick = tick;
	lastUp = u;
	lastDown = d;
	
	if ( (ddiff > speed6) || (udiff > speed6) ) 
		{
			setMaxValue( (ddiff > udiff ? ddiff : udiff) );
		}

	return true;
}

void QMyStats::setMaxValue(int64_t value)
{
	speed6 = value;
	speed3 = value / 2;
	speed1 = speed3 / 3;
	speed2 = speed1 + speed1;
	speed4 = speed3 + speed1;
	speed5 = speed6 - speed1;
	update();
}

void QMyStats::paintEvent(QPaintEvent *)
{
	setUpdatesEnabled(false);
	paintBorder();
	paintScale();
	paintGraph();
	setUpdatesEnabled(true);
}

void QMyStats::paintBorder()
{
	QPainter painter(this);
	painter.setWindow(0, 0, 800, 600);
	painter.setRenderHint(QPainter::Antialiasing);
	
	painter.save();

	QLinearGradient linGrad(20, 50, 180, 250);
	linGrad.setColorAt(0, Qt::gray);
	linGrad.setColorAt(1, Qt::white);
	linGrad.setSpread(QGradient::ReflectSpread);
	painter.setBrush(linGrad);
	QRectF border(0, 0, 800, 600);
	painter.drawRoundRect(border, 5, 5);

	QLinearGradient linGrad1(20, 300, 457, 300);
	linGrad1.setColorAt(0, QColor(80,80,80));
	linGrad1.setColorAt(1, Qt::white);
	linGrad1.setSpread(QGradient::PadSpread);
	painter.setBrush(linGrad1);

	painter.setPen(QPen(col_bg, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	QRectF border1(20, 20, 760, 560);
	painter.drawRect(border1);

	painter.setBrush(Qt::black);
	QRectF panel(82, 43, 668, 517);
	painter.drawRect(panel);
	painter.restore();
	painter.end();
}


void QMyStats::paintGraph()
{
	QPainter painter(this);
	painter.setWindow(0, 0, 800, 600);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();
	// down traffic
	painter.setPen(QPen(col_down, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));	
	for (int i = 0; i < dlist.count()-1; i++ )
	{
		painter.drawLine(82 + (668*i)/dlist.count(), 551 - 498*dlist.at(i)/speed6 , 82 + (668*(i+1))/dlist.count() , 551 - 498*dlist.at(i+1)/speed6 );
	}
	painter.restore();
	painter.save();
	// up traffic
	painter.setPen(QPen(col_up, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i < uplist.count()-1; i++ )
	{
		painter.drawLine(82 + (668*i)/uplist.count(), 551 - 498*uplist.at(i)/speed6 , 82 + (668*(i+1))/uplist.count() , 551 - 498*uplist.at(i+1)/speed6 );
	}
	painter.restore();
	painter.end();
}


void QMyStats::paintScale()
{
	QPainter painter(this);
	painter.setWindow(0, 0, 800, 600);
	painter.setRenderHint(QPainter::Antialiasing);
	
	// draw vertical dot line
	painter.save();
	
	painter.setPen(QPen(col_scale, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	// Vertical dot-lines gray
	for (int i = 1; i <= 22; i++) painter.drawLine(82+i*30, 43, 82+i*30, 560);

	// Vertiaval dot-lines white color (main-lines)
	painter.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i <= 4; i++)
	{
	painter.drawLine(82+(i*75), 43, 82+(i*75), 560);
	painter.translate(75,0);
	}
	
	painter.restore();
	painter.save();

	// draw horizontal dot line
	painter.setPen(QPen(col_scale, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i <=6 ; i++) 
		{
			painter.drawLine(82, 53+(i*83), 750, 53+(i*83));
		}
	
	painter.setPen(QPen(col_axis, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	// draw zero-speed label
	painter.drawText(QRectF(50, 542, 20, 20), Qt::AlignRight, "0");
	// draw speed value
	QRectF a(22, 44, 58, 20);
	QRectF b(22, 125, 58, 20);
	QRectF c(22, 210, 58, 20);
	QRectF d(22, 293, 58, 20);
	QRectF e(22, 376, 58, 20);
	QRectF f(22, 459, 58, 20);
	
	QFont fnt;
	fnt.setPointSize(8);
	painter.setFont(fnt);
	
	painter.drawText(a, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed6)));
	painter.drawText(b, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed5)));
	painter.drawText(c, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed4)));
	painter.drawText(d, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed3)));
	painter.drawText(e, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed2)));
	painter.drawText(f, Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed1)));
	
	painter.restore();
	painter.end();
}

QSize QMyStats::minimumSizeHint() const
{
	return QSize(80, 60);
}

QSize QMyStats::sizeHint() const
{
	return QSize(800, 600);
}
