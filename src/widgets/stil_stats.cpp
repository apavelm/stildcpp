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

const int MAX_ELEMENTS = 500;
const int BORDER_MARGIN = 20;
const int PANEL_MARGIN = 20;
const int LEFT_PANEL_MARGIN = 70;
const int FNT_SIZE = 8;
const int HORIZONTAL_GRIP_SNAP = 50;
const int HORIZONTAL_GRIP_MAIN_SNAP = 125;

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
	setBackgroundRole(QPalette::Window);
	setAutoFillBackground(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	dlist.clear();
	uplist.clear();
	speed[0] = 0;
	d_p = new QPointF[MAX_ELEMENTS+2];
	u_p = new QPointF[MAX_ELEMENTS+2];
	int64_t a = findMax(dlist);
	int64_t b = findMax(uplist);
	setMaxValue( (a > b ? a : b) );

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
	timer->start(1000);
}

QMyStats::~QMyStats()
{
	dlist.clear();
	uplist.clear();
	delete d_p;
	delete u_p;
	delete timer;
}

void QMyStats::refreshPixmap()
{
	pixmap = QPixmap(size());
	pixmap.fill(this, 0, 0);

	QPainter painter(&pixmap);
	painter.initFrom(this);
	painter.setRenderHint(QPainter::Antialiasing);
	paintBorder(&painter);
	paintScale(&painter);
	paintGraph(&painter);
	update();
}

int64_t QMyStats::findMax(const QList<int64_t> & lst)
{
	QList<int64_t>::const_iterator i;
	int64_t mx = 0;
	for (i = lst.begin(); i!= lst.end(); ++i)
		mx = qMax(mx, (*i));
	return mx;
}

void QMyStats::eachSecond()
{
	uint32_t tick = GET_TICK();
	uint32_t tdiff = tick - lastTick;
	if(tdiff == 0) return;

	int64_t d = Socket::getTotalDown();
	int64_t ddiff = d - lastDown;
	int64_t u = Socket::getTotalUp();
	int64_t udiff = u - lastUp;
	
	if ( dlist.count() >= MAX_ELEMENTS) 
		{
			int64_t tmp = dlist.at(0);
			dlist.removeAt(0);
			if (tmp == speed[6])
			{
				int64_t a = findMax(dlist);
				int64_t b = findMax(uplist);
				setMaxValue( (a > b ? a : b) );
			}
		}
	dlist.append(ddiff);
	
	if ( uplist.count() >= MAX_ELEMENTS)
		{
			int64_t tmp = uplist.at(0);
			uplist.removeAt(0);
			if (tmp == speed[6])
			{
				int64_t a = findMax(dlist);
				int64_t b = findMax(uplist);
				setMaxValue( (a > b ? a : b) );
			}
		}
	uplist.append(udiff);
	
	lastTick = tick;
	lastUp = u;
	lastDown = d;
	
	if ( (ddiff > speed[6]) || (udiff > speed[6]) ) 
		{
			setMaxValue( (ddiff > udiff ? ddiff : udiff) );
		}
	calcCoord();
	refreshPixmap();
}

void QMyStats::setMaxValue(int64_t value)
{
	if (value < 6) speed[6] = 6; 
	else
		speed[6] = value;
	speed[3] = speed[6] / 2;
	speed[1] = speed[3] / 3;
	speed[2] = speed[1] + speed[1];
	speed[4] = speed[3] + speed[1];
	speed[5] = speed[6] - speed[1];
}

void QMyStats::paintEvent(QPaintEvent *)
{
	QStylePainter painter(this);
	painter.drawPixmap(0, 0, pixmap);
}

void QMyStats::paintBorder(QPainter *painter)
{
	painter->save();
	QLinearGradient linGrad(20, 50, 180, 250);
	linGrad.setColorAt(0, Qt::gray);
	linGrad.setColorAt(1, Qt::white);
	linGrad.setSpread(QGradient::ReflectSpread);
	painter->setBrush(linGrad);
	QRectF border(0, 0, size().width(), size().height());
	painter->drawRoundRect(border, 5, 5);
	
	painter->restore();
	painter->save();

	QLinearGradient linGrad1(BORDER_MARGIN, size().height() /2 , size().width()-BORDER_MARGIN, size().height() /2 );
	linGrad1.setColorAt(0, QColor(80,80,80));
	linGrad1.setColorAt(1, Qt::white);
	linGrad1.setSpread(QGradient::PadSpread);
	painter->setBrush(linGrad1);

	painter->setPen(QPen(col_bg, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	QRectF border1(BORDER_MARGIN, BORDER_MARGIN, size().width()-2*BORDER_MARGIN, size().height()-2*BORDER_MARGIN);
	painter->drawRect(border1);

	painter->setBrush(Qt::black);
	QRectF panel(BORDER_MARGIN + LEFT_PANEL_MARGIN , BORDER_MARGIN + PANEL_MARGIN, size().width() - LEFT_PANEL_MARGIN - PANEL_MARGIN - 2*BORDER_MARGIN, size().height()-2*PANEL_MARGIN - 2*BORDER_MARGIN);
	painter->drawRect(panel);
	painter->restore();
}

void QMyStats::calcCoord()
{
	qreal t_x, t_y;
	int d_p_c = dlist.count();
	int u_p_c = uplist.count();
	int left_start = BORDER_MARGIN + LEFT_PANEL_MARGIN + 1;
	int up_start = size().height()- PANEL_MARGIN - BORDER_MARGIN;
	int panel_width = size().width() - 2*BORDER_MARGIN - PANEL_MARGIN - LEFT_PANEL_MARGIN;
	int panel_height = size().height() - 2*PANEL_MARGIN - 2*BORDER_MARGIN;
	d_p[0] = QPointF(left_start, up_start);
	u_p[0] = d_p[0];
	qreal prop1, prop2;
	// DOWN-LINE
	for (int i = 0; i < d_p_c; i++)
	{
		prop1 = qreal(dlist.at(i))/qreal(speed[6]);
		t_x = left_start + panel_width*(i+1)/d_p_c;
		t_y = up_start - panel_height*prop1;
		d_p[i+1] = QPointF(t_x, t_y);
	}
	// UP-LINE
	for (int i = 0; i < u_p_c; i++)
	{
		prop2 = qreal(uplist.at(i))/qreal(speed[6]);
		t_x = left_start + panel_width*(i+1)/u_p_c;
		t_y = up_start - panel_height*prop2;
		u_p[i+1] = QPointF(t_x, t_y);
	}
}

void QMyStats::paintGraph(QPainter *painter)
{
	painter->save();
	// down traffic
	painter->setPen(QPen(col_down, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawPolyline(d_p, dlist.count() + 1);
	
	painter->restore();
	painter->save();
	
	// up traffic
	painter->setPen(QPen(col_up, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawPolyline(u_p, uplist.count() + 1);
	
	painter->restore();
}


void QMyStats::paintScale(QPainter *painter)
{
	painter->save();
	// draw horizontal dot lines
	qreal wh = (size().height() - 2*BORDER_MARGIN - 2*PANEL_MARGIN) / 6;
	painter->setPen(QPen(col_scale, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i <=6 ; i++) 
		{
			QLineF ql(BORDER_MARGIN + LEFT_PANEL_MARGIN,
					BORDER_MARGIN + PANEL_MARGIN + i*wh,
					size().width() - PANEL_MARGIN - BORDER_MARGIN,
					BORDER_MARGIN + PANEL_MARGIN + i*wh);
			painter->drawLine(ql);
		}
	painter->restore();
	
	
	painter->save();
	// draw vertical dot lines
	painter->setPen(QPen(col_scale, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	int vc = (size().width() - 2*BORDER_MARGIN - LEFT_PANEL_MARGIN - PANEL_MARGIN) / HORIZONTAL_GRIP_SNAP;
	for (int i = 1; i <= vc; i++) painter->drawLine(BORDER_MARGIN + LEFT_PANEL_MARGIN + i*HORIZONTAL_GRIP_SNAP, BORDER_MARGIN + PANEL_MARGIN, BORDER_MARGIN + LEFT_PANEL_MARGIN +i*HORIZONTAL_GRIP_SNAP, size().height() - 2*BORDER_MARGIN);

	// Vertiaval dot-lines white color (main-lines)
	painter->setPen(QPen(Qt::white, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i <= 4; i++)
	{
	painter->drawLine(BORDER_MARGIN + LEFT_PANEL_MARGIN +(i*HORIZONTAL_GRIP_MAIN_SNAP), BORDER_MARGIN + PANEL_MARGIN, BORDER_MARGIN + LEFT_PANEL_MARGIN +(i*HORIZONTAL_GRIP_MAIN_SNAP), size().height() - 2*BORDER_MARGIN);
	painter->translate(HORIZONTAL_GRIP_MAIN_SNAP,0);
	}
	painter->restore();


	painter->setPen(QPen(col_axis, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	QFont fnt;
	fnt.setPointSize(FNT_SIZE);
	painter->setFont(fnt);
	// draw speed value
	QRectF _R[7];
	for (int i=6; i>=0; i--)
	{
		_R[i] = QRectF(BORDER_MARGIN + 2, BORDER_MARGIN + PANEL_MARGIN +3 - (FNT_SIZE + 1) + (6-i)*wh, LEFT_PANEL_MARGIN - 5, FNT_SIZE*2);
		painter->drawText(_R[i], Qt::AlignRight, QString::fromStdString(Util::formatBytes(speed[i])));
	}
}


void QMyStats::resizeEvent(QResizeEvent *)
{
	refreshPixmap();
}

QSize QMyStats::minimumSizeHint() const
{
	return QSize(80, 60);
}

QSize QMyStats::sizeHint() const
{
	return QSize(800, 600);
}
