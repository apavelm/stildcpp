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
	col_up(SETTING(UPLOAD_BAR_COLOR)),
	col_down(SETTING(DOWNLOAD_BAR_COLOR)),
	col_bg(Qt::black),
	col_axis(Qt::yellow),
	col_scale(Qt::darkGray)
{
	initValue();
	initSecond();
}

QMyStats::~QMyStats()
{
	delete timer;
}

void QMyStats::initSecond()
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(eachSecond()));
	timer->start(1000);
}

bool QMyStats::eachSecond()
{
	uint32_t tick = GET_TICK();
	uint32_t tdiff = tick - lastTick;
	if(tdiff == 0)
		return true;

	uint32_t scrollms = (tdiff + scrollTick)*PIX_PER_SEC;
	uint32_t scroll = scrollms / 1000;

	if(scroll == 0)
		return true;

	scrollTick = scrollms - (scroll * 1000);

//	SmartWin::Point clientSize = getClientAreaSize();
//	RECT rect = { twidth, 0, clientSize.x, clientSize.y };
	//::ScrollWindow(handle(), -((int)scroll), 0, &rect, &rect);

	int64_t d = Socket::getTotalDown();
	int64_t ddiff = d - lastDown;
	int64_t u = Socket::getTotalUp();
	int64_t udiff = u - lastUp;

qDebug() << "SOCKET: down_total: " << d << "up_total: " << u << "ddiff: " <<ddiff << "udiff: " << udiff;

	addTick(ddiff, tdiff, down, downAvg, scroll);
	addTick(udiff, tdiff, up, upAvg, scroll);

	int64_t mspeed = 0;
	StatIter i;
	for(i = down.begin(); i != down.end(); ++i) {
		if(mspeed < i->speed)
			mspeed = i->speed;
	}
	for(i = up.begin(); i != up.end(); ++i) {
		if(mspeed < i->speed)
			mspeed = i->speed;
	}
	if(mspeed > max || ((max * 3 / 4) > mspeed) ) {
		max = mspeed;
		update(); // ??? 
	}

	lastTick = tick;
	lastUp = u;
	lastDown = d;
	return true;
}


void QMyStats::addTick(int64_t bdiff, int64_t tdiff, StatList& lst, AvgList& avg, int scroll)
{
	while((int)lst.size() > ((width / PIX_PER_SEC) + 1) ) {
		lst.pop_back();
	}
	while(avg.size() > AVG_SIZE ) {
		avg.pop_back();
	}
	int64_t bspeed = bdiff * (int64_t)1000 / tdiff;
	avg.push_front(bspeed);

	bspeed = 0;

	for(AvgIter ai = avg.begin(); ai != avg.end(); ++ai) {
		bspeed += *ai;
	}

	bspeed /= avg.size();
	lst.push_front(Stat(scroll, bspeed));
}

void QMyStats::initValue()
{
    start_point = 0;
    zoom_point = ((1024/2)+1)*20000/22050;
	speed1 = 1;
	speed2 = 2;
	speed3 = 3;
	speed4 = 4;
	speed5 = 5;
	range_freq = 5;
    typeFFT = 0;
    lockFFT = false;

}

void QMyStats::setZoomPoint(int N)
{
	zoom_point = ((N/2)+1)*20000/22050;
}

void QMyStats::paintEvent(QPaintEvent *)
{
	paintBorder();
	//paintFreq();
	paintScale();
	//paintControl();
}


void QMyStats::paintBorder()
{
	QPainter painter(this);
	painter.setWindow(0, 0, 800, 600);
	painter.setRenderHint(QPainter::Antialiasing);

	QLinearGradient linGrad(20, 50, 180, 250);
	linGrad.setColorAt(0, Qt::gray);
	linGrad.setColorAt(1, Qt::white);
	linGrad.setSpread(QGradient::ReflectSpread);
	painter.setBrush(linGrad);
	QRectF border(0, 0, 800, 600);
	painter.drawRoundRect(border, 5, 5);

	QLinearGradient linGrad1(20, 180, 57, 180);
	linGrad1.setColorAt(0, Qt::white);
	linGrad1.setColorAt(1, QColor(80,80,80));
	linGrad1.setSpread(QGradient::PadSpread);
	painter.setBrush(linGrad1);

	painter.setPen(QPen(col_bg, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	QRectF border1(20, 20, 760, 560);
	painter.drawRect(border1);

	painter.setBrush(Qt::black);
	QRectF panel(82, 43, 668, 517);
	painter.drawRect(panel);
}


void QMyStats::paintFreq()
{
    if (wavFile.isEmpty())
        return;
    else
    {
        QPainter painter(this);
        painter.setWindow(0, 0, 800, 360);
        painter.setRenderHint(QPainter::Antialiasing);

        QLinearGradient linGrad(400, 320, 400, 40);
        linGrad.setColorAt(0, Qt::green);
        linGrad.setColorAt(1, Qt::red);
        linGrad.setSpread(QGradient::PadSpread);
        painter.setBrush(linGrad);

        painter.setPen(QPen(linGrad, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        // draw the frequency value
        // X axis is 700 pt. - Y axis is 360 pt.
        // 1024/2 pt : 22050 Hz = x : 20000 Hz -> x = 464 ?????
        // DC freq (0 kz) not draw
        // typeFFT = 0 linear 1 logaritmic

        if (typeFFT == 0)
        {
            for (int i = 1; i <= zoom_point-1; i++)
            {
                painter.drawLine(100+(i*600/zoom_point), 260, 100+(i*600/zoom_point), (260-195*(magnitudeLin[i+start_point]/max_val_lin)));
            }
        }
        if (typeFFT == 1)
        {
            for (int i = 1; i <= zoom_point-1; i++)
            {
                painter.drawLine(100+(i*600/zoom_point), 260, 100+(i*600/zoom_point), (260-195*(magnitudeLog[i+start_point]/max_val_log)));
            }
        }
    }


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

	// draw orizzontal dot line
	painter.setPen(QPen(col_scale, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	for (int i = 0; i <=6 ; i++) painter.drawLine(82, 53+(i*83), 750, 53+(i*83));
	
	painter.setPen(QPen(col_axis, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	// draw speed value-consts
	painter.drawText(QRectF(50, 540, 20, 20), Qt::AlignCenter, "0");
	// draw speed value	
	QRectF a(30, 50, 40, 20);
	QRectF b(30, 130, 40, 20);
	QRectF c(30, 210, 40, 20);
	QRectF d(30, 295, 40, 20);
	QRectF e(30, 370, 40, 20);
	QRectF f(30, 450, 40, 20);
	float s1 = speed1 / 1024;
	float s2 = speed2 / 1024;
	float s3 = speed3 / 1024;
	float s4 = speed4 / 1024;
	float s5 = speed5 / 1024;
	float s6 = speed6 / 1024;
	QString fr1 = QString("%1").arg(s1, 0, 'f', 2);
	QString fr2 = QString("%1").arg(s2 ,0, 'f', 2);
	QString fr3 = QString("%1").arg(s3, 0, 'f', 2);
	QString fr4 = QString("%1").arg(s4, 0, 'f', 2);
	QString fr5 = QString("%1").arg(s5, 0, 'f', 2);
	QString fr6 = QString("%1").arg(s6, 0, 'f', 2);
	painter.drawText(a, Qt::AlignCenter, fr1);
	painter.drawText(b, Qt::AlignCenter, fr2);
	painter.drawText(c, Qt::AlignCenter, fr3);
	painter.drawText(d, Qt::AlignCenter, fr4);
	painter.drawText(e, Qt::AlignCenter, fr5);
	painter.drawText(f, Qt::AlignCenter, fr6);

/*
    // draw label
    painter.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    QRectF m(60, 150, 30, 20);
    painter.drawText(m, Qt::AlignCenter, "Magn");
*/

}

void QMyStats::paintControl()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 800, 360);
    painter.setRenderHint(QPainter::Antialiasing);
    // select color lin-log
/*    
    painter.setBrush(colLin);
    painter.drawEllipse(QRect(100, 300, 10, 10));
    painter.setBrush(colLog);
    painter.drawEllipse(QRect(145, 300, 10, 10));
    // select color point number
    painter.setBrush(col512);
    painter.drawEllipse(QRect(705, 60, 10, 10));
    painter.setBrush(col1024);
    painter.drawEllipse(QRect(705, 80, 10, 10));
    painter.setBrush(col2048);
    painter.drawEllipse(QRect(705, 100, 10, 10));
    painter.setBrush(col4096);
    painter.drawEllipse(QRect(705, 120, 10, 10));
    painter.setBrush(colLock);
    painter.drawEllipse(QRect(705, 140, 10, 10));
    // draw label lin-log
    painter.setPen(QPen(Qt::white));
    QFont labFont("Arial", 8, QFont::Normal);
    painter.setFont(labFont);
    QRectF lin(115, 300, 20, 10);
    painter.drawText(lin, Qt::AlignCenter, "Lin");
    QRectF log(160, 300, 20, 10);
    painter.drawText(log, Qt::AlignCenter, "Log");
    // draw label point number
    QFont numFont("Arial", 7, QFont::Normal);
    painter.setFont(numFont);
    QRectF p512(720, 60, 20, 10);
    painter.drawText(p512, Qt::AlignLeft, "512");
    QRectF p1024(720, 80, 20, 10);
    painter.drawText(p1024, Qt::AlignLeft, "1024");
    QRectF p2048(720, 100, 20, 10);
    painter.drawText(p2048, Qt::AlignLeft, "2048");
    QRectF p4096(720, 120, 20, 10);
    painter.drawText(p4096, Qt::AlignLeft, "4096");
    QRectF pLock(720, 140, 20, 10);
    painter.drawText(pLock, Qt::AlignLeft, "Lock");
    // draw file folder
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    QRectF folder(712, 172, 30, 15);
    painter.drawRoundRect(folder, 10, 10);
    painter.setPen(QPen(Qt::white));
    painter.setBrush(Qt::black);
    QRectF folder1(710, 170, 30, 15);
    painter.drawRoundRect(folder1, 10, 10);
    painter.setPen(QPen(Qt::yellow));
    QFont textFont("Arial", 8, QFont::Normal);
    painter.setFont(textFont);
    painter.drawText(folder1, Qt::AlignCenter, "FILE");
*/    
   
}

QSize QMyStats::minimumSizeHint() const
{
	return QSize(80, 60);
}

QSize QMyStats::sizeHint() const
{
	return QSize(800, 600);
}
