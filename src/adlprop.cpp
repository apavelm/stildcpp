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
 
#include "adlprop.h"

ADLPropertiesDialog::ADLPropertiesDialog(QWidget *parent, ADLSearch * _search) : QDialog(parent), src(_search)
{
	setupUi(this);
	
	// Translate dialog
	setWindowTitle(StilUtils::TstrtoQ(T_("ADLSearch Properties")));

	edt_string->setText(StilUtils::TstrtoQ(Text::toT(_search->searchString)));
	edt_string->setFocus();

	cmb_searchtype->addItem(StilUtils::TstrtoQ(T_("Filename")));
	cmb_searchtype->addItem(StilUtils::TstrtoQ(T_("Directory")));
	cmb_searchtype->addItem(StilUtils::TstrtoQ(T_("Full Path")));
	cmb_searchtype->setCurrentIndex(_search->sourceType);

	edt_minsize->setText(StilUtils::TstrtoQ((_search->minFileSize > 0) ? Text::toT(Util::toString(_search->minFileSize)) : Util::emptyStringT));
	edt_maxsize->setText(StilUtils::TstrtoQ((_search->maxFileSize > 0) ? Text::toT(Util::toString(_search->maxFileSize)) : Util::emptyStringT));

	cmb_sizetype->addItem(StilUtils::TstrtoQ(T_("B")));
	cmb_sizetype->addItem(StilUtils::TstrtoQ(T_("KiB")));
	cmb_sizetype->addItem(StilUtils::TstrtoQ(T_("MiB")));
	cmb_sizetype->addItem(StilUtils::TstrtoQ(T_("GiB")));
	cmb_sizetype->setCurrentIndex(_search->typeFileSize);

	edt_destdir->setText(StilUtils::TstrtoQ(Text::toT(_search->destDir)));

	cb_enabled->setText(StilUtils::TstrtoQ(T_("Enabled")));
	Qt::CheckState s = ( _search->isActive ? Qt::Checked : Qt::Unchecked);
	cb_enabled->setChecked(s);

	cb_dlmatches->setText(StilUtils::TstrtoQ(T_("Download Matches")));
	s = ( _search->isAutoQueue ? Qt::Checked : Qt::Unchecked);
	cb_dlmatches->setChecked(s);
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAccept()) );
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()) );
}

ADLPropertiesDialog::~ADLPropertiesDialog()
{
}

void ADLPropertiesDialog::slotAccept()
{
	src->searchString = Text::fromT(StilUtils::QtoTstr(edt_string->text()));

	src->sourceType = (ADLSearch::SourceType)cmb_searchtype->currentIndex();

	tstring minFileSize = StilUtils::QtoTstr(edt_minsize->text());
	src->minFileSize = minFileSize.empty() ? -1 : Util::toInt64(Text::fromT(minFileSize));
	
	tstring maxFileSize = StilUtils::QtoTstr((edt_maxsize->text()));
	src->maxFileSize = maxFileSize.empty() ? -1 : Util::toInt64(Text::fromT(maxFileSize));
	
	src->typeFileSize = (ADLSearch::SizeType)cmb_sizetype->currentIndex();
	src->destDir = Text::fromT(StilUtils::QtoTstr(edt_destdir->text()));
	
	src->isActive = cb_enabled->isChecked();
	src->isAutoQueue = cb_dlmatches->isChecked();

	accept();
}

