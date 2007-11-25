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

#ifndef __ZSTREAM_H__
#define __ZSTREAM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <QtCore>

namespace zstream {


#define CHUNK 524288

struct z_header
{
	char filename[40];
	long int	offset;
	size_t	size;
};

#define Z_FILE_NOT_FOUND 16

typedef QList<z_header> HeaderList;

	// Internal functions
	int def(FILE *source, FILE *dest, int level);
	int readZfile(FILE *source, const z_header &head, FILE *dest);
	void zerr(int ret);
	// User Functions
	int compressdir(const QString &dir, const QString &filename);
	int compressdir(QString &dir, QString &filename);
	HeaderList * GetZList(const QString &filename);
	HeaderList * GetZList(QString &filename);
	int unpack2dir(const QString &filename, const QString &dir);
	int unpack2dir(QString &filename, QString &dir);
	QByteArray getFileData(const QString &archive_filename, const QString &filename);
	QByteArray getFileData(QString &archive_filename, QString &filename);

}; // of namespace

#endif // __ZSTREAM_H__
