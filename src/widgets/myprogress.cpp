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

#include "myprogress.h"

bool MyProgressBar::repaintRequired() const
{
	if (_value == _lastPaintedValue) return false;

	int64_t valueDifference = qAbs(_value - _lastPaintedValue);

	// Check if the text needs to be repainted
	if (_value == _minimum || _value == _maximum) return true;
	if (_textVisible) 
	{
		if ((_format.contains(QLatin1String("%v")))) return true;
		if ((_format.contains(QLatin1String("%z")))) return true;
		if ((_format.contains(QLatin1String("%p")) && valueDifference >= qAbs((_maximum - _minimum) / 100))) return true;
	}

	// Check if the bar needs to be repainted
	QStyleOptionProgressBarV2 opt;
	initStyleOption(&opt);
	int cw = style()->pixelMetric(QStyle::PM_ProgressBarChunkWidth, &opt, this);
	QRect groove  = style()->subElementRect(QStyle::SE_ProgressBarGroove, &opt, this);
	return (valueDifference * groove.width() > cw * (_maximum - _minimum));
}

void MyProgressBar::initStyleOption(QStyleOptionProgressBar *option) const
{
	if (!option) return;
	option->initFrom(this);

	if (_orientation == Qt::Horizontal) option->state |= QStyle::State_Horizontal;
	option->minimum = 0;
	option->maximum = 100;
	if (_maximum - _minimum != 0 || _value == -1 )
		option->progress = qAbs( 100 * (_value - _minimum) / (_maximum - _minimum) );
	else option->progress = 0;
	option->textAlignment = _alignment;
	option->textVisible = _textVisible;
	option->text = text();
	option->palette = plt;
	option->type = QStyleOption::SO_ProgressBar;

	if (QStyleOptionProgressBarV2 *optionV2 = qstyleoption_cast<QStyleOptionProgressBarV2 *>(option)) 
	{
		optionV2->orientation = _orientation;  // ### Qt 5: use State_Horizontal instead
		optionV2->invertedAppearance = _invertedAppearance;
		optionV2->bottomToTop = (_textDirection == MyProgressBar::BottomToTop);
	}
}

void MyProgressBar::resetLayoutItemMargins()
{
	QStyleOptionProgressBar option;
	initStyleOption(&option);
	updateGeometry();
}

void MyProgressBar::SetBarColor(QPalette::ColorRole role, const QColor & color )
{
	plt.setColor(role, color);
}

MyProgressBar::MyProgressBar(QWidget *parent) : QWidget(parent)
{
	_minimum = 0;
	_maximum = 100;
	_value = -1;
	_alignment = Qt::AlignLeft;
	_textVisible = true;
	_lastPaintedValue = -1;
	_orientation = Qt::Horizontal;
	_invertedAppearance = false;
	_textDirection = MyProgressBar::TopToBottom;
	_format = QLatin1String("%p%");
	plt = palette();

	QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::Fixed);
	if (_orientation == Qt::Vertical) sp.transpose();
	setSizePolicy(sp);
	setAttribute(Qt::WA_WState_OwnSizePolicy, false);
	resetLayoutItemMargins();
}

MyProgressBar::~MyProgressBar()
{}

void MyProgressBar::reset()
{
	_value = _minimum - 1;
	if (_minimum == INT_MIN) _value = INT_MIN;
	repaint();
}

void MyProgressBar::setValue(int64_t value)
{
	if (_value == value || ((value > _maximum || value < _minimum) && _maximum != 0)) return;
	_value = value;
	emit valueChanged(value);
	if (repaintRequired()) repaint();
}

void MyProgressBar::setRange(int64_t minimum, int64_t maximum)
{
	_minimum = qMin(minimum, maximum);
	_maximum = qMax(minimum, maximum);
}

void MyProgressBar::setTextVisible(bool visible)
{
	if (_textVisible != visible) 
	{
		_textVisible = visible;
		repaint();
	}
}

void MyProgressBar::setAlignment(Qt::Alignment alignment)
{
	if (_alignment != alignment) 
	{
		_alignment = alignment;
		repaint();
	}
}

void MyProgressBar::paintEvent(QPaintEvent *)
{
	QStylePainter paint(this);
	QStyleOptionProgressBarV2 opt;
	initStyleOption(&opt);
	paint.drawControl(QStyle::CE_ProgressBar, opt);
	_lastPaintedValue = _value;
}

QSize MyProgressBar::sizeHint() const
{
	ensurePolished();
	QFontMetrics fm = fontMetrics();
	QStyleOptionProgressBarV2 opt;
	initStyleOption(&opt);
	int cw = style()->pixelMetric(QStyle::PM_ProgressBarChunkWidth, &opt, this);
	QSize size = QSize(cw * 7 + fm.width(QLatin1Char('0')) * 4, fm.height() + 2);
	if (opt.orientation == Qt::Vertical) size.transpose();
	return style()->sizeFromContents(QStyle::CT_ProgressBar, &opt, size, this);
}

QSize MyProgressBar::minimumSizeHint() const
{
	QSize size;
	if (orientation() == Qt::Horizontal) size = QSize(sizeHint().width(), fontMetrics().height() + 2);
	else size = QSize(fontMetrics().height() + 2, sizeHint().height());
	return size;
}

QString MyProgressBar::text() const
{
	if (_maximum == 0 || _value < _minimum || (_value == INT_MIN && _minimum == INT_MIN)) return QString();

	int64_t totalSteps = _maximum - _minimum;

	QString result = _format;
	result.replace(QLatin1String("%m"), QString::fromLatin1("%1").arg(totalSteps));
	result.replace(QLatin1String("%z"), QString::fromLatin1("%1").arg(StilUtils::fmtBytes(_value)));
	result.replace(QLatin1String("%v"), QString::fromLatin1("%1").arg(_value));

// If max and min are equal and we get this far, it means that the
// progress bar has one step and that we are on that step. Return
// 100% here in order to avoid division by zero further down.

	if (totalSteps == 0) 
	{
		result.replace(QLatin1String("%p"), QString::fromLatin1("%1").arg(100));
		return result;
	}

	int progress = int(((qreal(_value) - qreal(_minimum)) * 100.0) / totalSteps);
	result.replace(QLatin1String("%p"), QString::fromLatin1("%1").arg(progress));
	return result;
}

void MyProgressBar::setOrientation(Qt::Orientation orientation)
{
	if (_orientation == orientation) return;
	_orientation = orientation;
	if (!testAttribute(Qt::WA_WState_OwnSizePolicy))
	{
		QSizePolicy sp = sizePolicy();
		sp.transpose();
		setSizePolicy(sp);
		setAttribute(Qt::WA_WState_OwnSizePolicy, false);
	}
	resetLayoutItemMargins();
	update();
	updateGeometry();
}

bool MyProgressBar::event(QEvent *e)
{
	if (e->type() == QEvent::StyleChange)
	resetLayoutItemMargins();
	return QWidget::event(e);
}

void MyProgressBar::setFormat(const QString &format)
{
	if (_format != format)
	{
		_format = format;
		update();
	}
}
