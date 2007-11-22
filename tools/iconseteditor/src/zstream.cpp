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

#include "zstream.h"

namespace zstream
{

int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    char in[CHUNK];
    char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) 
	{
		deflateEnd(&strm);
		return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = (Bytef*)in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do 
	{
            strm.avail_out = CHUNK;
            strm.next_out = (Bytef*)out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
	    {
		deflateEnd(&strm);
		return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
	deflateEnd(&strm);
	return Z_OK;
}


int readZfile(FILE *source, const z_header &head, FILE *dest)
{
	int ret;
	unsigned long int have;
	z_stream strm;
	char out[CHUNK];
	char *in = (char*)malloc(head.size);

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK) return ret;

	/* decompress until deflate stream ends or end of file */
	fseek(source, head.offset, SEEK_SET);
	do 
	{
		strm.avail_in = fread(in, 1, head.size, source);
		if (ferror(source)) 
			{
			inflateEnd(&strm);
			return Z_ERRNO;
			}
		if (strm.avail_in == 0) break;
		strm.next_in = (Bytef*)in;

		/* run inflate() on input until output buffer not full */
		do 
		{
			strm.avail_out = CHUNK;
			strm.next_out = (Bytef*)out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) 
			{
				case Z_NEED_DICT: ret = Z_DATA_ERROR;	/* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR: inflateEnd(&strm);
			return ret;
			}
			
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) 
			{
				inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

	/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	inflateEnd(&strm);
	free(in);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void zerr(int ret)
{
	switch (ret) 
	{
	case Z_STREAM_ERROR: fprintf(stderr, "invalid compression level\n"); break;
	case Z_DATA_ERROR: fprintf(stderr, "invalid or incomplete deflate data\n"); break;
	case Z_MEM_ERROR: fprintf(stderr, "out of memory\n"); break;
	case Z_VERSION_ERROR: fprintf(stderr, "zlib version mismatch!\n");
	}
}

int compressdir(const QString &dir, const QString &filename)
{
	int ret = Z_OK;
	FILE *out = fopen(filename.toStdString().c_str(),"w+b");
	
	// Scaning Directory for files and adding information about it into header
	QDir path(dir);
	if(path.exists()) 
	{
	QFileInfoList entrylist=path.entryInfoList( QDir::Readable | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Files);
	// Writing Information about Files_Count in Directory to Stream
	int tint = entrylist.count();
	fwrite(&tint, sizeof(int), 1, out);
	z_header *head = new z_header;
	head->offset=sizeof(int)+(sizeof(z_header)*tint);
	head->size=0;

	FILE *tmp1;
	FILE *tmp2;
	foreach(QFileInfo fi, entrylist) 
		if(fi.exists() && fi.isFile() && fi.isReadable() && !fi.isSymLink()) 
		{
			strncpy(head->filename, fi.fileName().toStdString().c_str(), 40);
			tmp1 = fopen( (dir+fi.fileName()).toStdString().c_str() , "r+b");
			tmp2 = tmpfile();
			def(tmp1, tmp2, Z_BEST_SPEED);
			head->size = ftell(tmp2);
			fwrite(head, sizeof(z_header), 1, out);
			head->offset += head->size;
			fclose(tmp1);
			fclose(tmp2);
		}
	FILE *in;
	foreach(QFileInfo fi, entrylist) 
		if(fi.exists() && fi.isFile() && fi.isReadable() && !fi.isSymLink()) 
		{
			in = fopen((dir+fi.fileName()).toStdString().c_str(),"r+b");
			ret = def(in, out, Z_BEST_SPEED);
			fclose(in);
		}
	}

	fclose(out);
	if (ret != Z_OK) zerr(ret);
	return ret;
}

int compressdir(QString &dir, QString &filename)
{
	const QString &a1 = dir;
	const QString &a2 = filename;
	return compressdir(a1, a2);
}

HeaderList * GetZList(const QString &filename)
{
	FILE *in = fopen(filename.toStdString().c_str(),"r+b");
	int N = 0;
	fread(&N,sizeof(int),1,in);
	
	HeaderList * lst = new HeaderList;
	lst->clear();
	
	z_header head;
	for (int i=0; i<N; i++)
	{
		fread(&head,sizeof(z_header),1,in);
		lst->append(head);
	}
	fclose(in);
	return lst;
}

HeaderList * GetZList(QString &filename)
{
	const QString &a1 = filename;
	return GetZList(a1);
}

int unpack2dir(const QString &filename, const QString &dir)
{
	QDir(".").mkpath(dir);
	int ret = Z_OK;
	HeaderList * lst = GetZList(filename);
	
	FILE *in = fopen(filename.toStdString().c_str(),"r+b");
	
	// Unpacking files writing to directory
	FILE *out;
	z_header head;
	for (int i=0;i<lst->count();i++)
	{
		head = lst->at(i);
		QString fn = head.filename;
		fn = dir + fn;
		out = fopen( fn.toStdString().c_str() ,"w+b");
		ret = readZfile(in, head, out);
		fclose(out);
	}
	
	fclose(in);
	if (ret != Z_OK) zerr(ret);
	return ret;
}

int unpack2dir(QString &filename, QString &dir)
{
	const QString &a1 = dir;
	const QString &a2 = filename;
	return unpack2dir(a2, a1);
}

QByteArray getFileData(const QString &archive_filename, const QString &filename)
{
	QByteArray ret;
	ret.clear();
	if (filename.isEmpty() || (filename.length()>40)) return ret;
	
	HeaderList * lst = GetZList(archive_filename);
	int index = -1;
	
 	for (int i = 0; i< lst->count(); i++) 
 		if (!strncmp(filename.toStdString().c_str(), lst->at(i).filename, 40))
 		{
 			index = i;
 			break;
		}
	if (index == -1) return ret; // FILE NOT FOUND
		
	FILE *in = fopen(archive_filename.toStdString().c_str(),"r+b");
	// Unpacking file
	FILE *out = tmpfile();
	if (readZfile(in, lst->at(index), out) != Z_OK ) return ret;
	long int ps = ftell(out);
	rewind(out);
	char * buf = (char * )malloc(ps);
	fread(buf, ps, 1, out);
	ret = QByteArray(buf, ps);
	free(buf);
	fclose(out);
	fclose(in);
	return ret;
}
QByteArray getFileData(QString &archive_filename, QString &filename)
{
	const QString &a1 = archive_filename;
	const QString &a2 = filename;
	return getFileData(a1, a2);
}

}; // of namespace
