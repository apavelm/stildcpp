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

#ifndef __MYPROGRESS_H__
#define __MYPROGRESS_H__

#include <QtGui>
#include <QtCore>

#include "../stilutils.h"

class MyProgressBar : public QWidget
{
	Q_OBJECT
public:
	enum Direction { TopToBottom, BottomToTop };

	MyProgressBar(QWidget *parent = 0);
	~MyProgressBar();

	const int64_t minimum() { return _minimum; }
	const int64_t maximum() { return _maximum; }
	const int64_t value() { return _value; }

	virtual QString text() const;
	void setTextVisible(bool visible);
	bool isTextVisible() const { return _textVisible; }

	Qt::Alignment alignment() const { return _alignment; }
	void setAlignment(Qt::Alignment alignment);

	QSize sizeHint() const;
	QSize minimumSizeHint() const;

	Qt::Orientation orientation() const { return _orientation; }

	void setInvertedAppearance(bool invert) { _invertedAppearance = invert; update(); }
	bool invertedAppearance() { return _invertedAppearance; }
	void setTextDirection(MyProgressBar::Direction textDirection) { _textDirection = textDirection; update(); }
	MyProgressBar::Direction textDirection() { return _textDirection; }
	
	void resetLayoutItemMargins();
	bool repaintRequired() const;

	void setFormat(const QString &format);
	QString format() const { return _format; }
	
	void SetBarColor(QPalette::ColorRole role, const QColor & color );

public slots:
	void reset();
	void setRange(int64_t minimum, int64_t maximum);
	void setMinimum(int64_t minimum) { _minimum = minimum; }
	void setMaximum(int64_t maximum) { _maximum = maximum; }
	void setValue(int64_t value);
	void setOrientation(Qt::Orientation);

signals:
	void valueChanged(int64_t value);

protected:
	bool event(QEvent *e);
	void paintEvent(QPaintEvent *);
	void initStyleOption(QStyleOptionProgressBar *option) const;
private:
	int64_t _minimum;
	int64_t _maximum;
	int64_t _value;
	QPalette plt;
	Qt::Alignment _alignment;
	uint _textVisible : 1;
	int64_t _lastPaintedValue;
	Qt::Orientation _orientation;
	bool _invertedAppearance;
	MyProgressBar::Direction _textDirection;
	QString _format;
};

#endif
