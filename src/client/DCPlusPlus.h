/*
 * Copyright (C) 2001-2008 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DCPP_DCPLUSPLUS_H
#define DCPP_DCPLUSPLUS_H

#ifdef _WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif

#ifdef _DEBUG

inline void CDECL debugTrace(const char* format, ...)
{
	va_list args;
	va_start(args, format);

#if defined _WIN32 && defined _MSC_VER
	char buf[512];

	_vsnprintf(buf, sizeof(buf), format, args);
	OutputDebugStringA(buf);
#else // _WIN32
	vprintf(format, args);
#endif // _WIN32
	va_end(args);
}

#define dcdebug debugTrace
#ifdef _MSC_VER
#define dcassert(exp) \
do { if (!(exp)) { \
	dcdebug("Assertion hit in %s(%d): " #exp "\n", __FILE__, __LINE__); \
	if(1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #exp)) \
_CrtDbgBreak(); } } while(false)
#define dcasserta(exp) dcassert(0)
#else
#define dcasserta(exp) assert(exp)
#define dcassert(exp) assert(exp)
#endif
#define dcdrun(exp) exp
#else //_DEBUG
#ifdef _MSC_VER
#define dcasserta(exp) __assume(exp)
#else
#define dcasserta(exp)
#endif // _WIN32
#define dcdebug if (false) printf
#define dcassert(exp)
#define dcdrun(exp)
#endif //_DEBUG

// Make sure we're using the templates from algorithm...
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

namespace dcpp {

typedef vector<string> StringList;
typedef StringList::iterator StringIter;
typedef StringList::const_iterator StringIterC;

typedef pair<string, string> StringPair;
typedef vector<StringPair> StringPairList;
typedef StringPairList::iterator StringPairIter;

typedef std::tr1::unordered_map<string, string> StringMap;
typedef StringMap::iterator StringMapIter;

typedef std::tr1::unordered_set<string> StringSet;
typedef StringSet::iterator StringSetIter;

typedef vector<wstring> WStringList;
typedef WStringList::iterator WStringIter;
typedef WStringList::const_iterator WStringIterC;

typedef pair<wstring, wstring> WStringPair;
typedef vector<WStringPair> WStringPairList;
typedef WStringPairList::iterator WStringPairIter;

typedef vector<uint8_t> ByteVector;

#if defined(_MSC_VER) || defined(__MINGW32__)
#define _LL(x) x##ll
#define _ULL(x) x##ull
#define I64_FMT "%I64d"
#define U64_FMT "%I64d"

#elif defined(SIZEOF_LONG) && SIZEOF_LONG == 8
#define _LL(x) x##l
#define _ULL(x) x##ul
#define I64_FMT "%ld"
#define U64_FMT "%ld"
#else
#define _LL(x) x##ll
#define _ULL(x) x##ull
#define I64_FMT "%lld"
#define U64_FMT "%lld"
#endif

#ifdef _WIN32

# define PATH_SEPARATOR '\\'
# define PATH_SEPARATOR_STR "\\"

#else

# define PATH_SEPARATOR '/'
# define PATH_SEPARATOR_STR "/"

#endif


typedef unordered_map<wstring, wstring> WStringMap;
typedef WStringMap::iterator WStringMapIter;

#ifdef UNICODE

typedef wstring tstring;
typedef WStringList TStringList;
typedef WStringIter TStringIter;
typedef WStringIterC TStringIterC;

typedef WStringPair TStringPair;
typedef WStringPairIter TStringPairIter;
typedef WStringPairList TStringPairList;

typedef WStringMap TStringMap;
typedef WStringMapIter TStringMapIter;

#else

typedef string tstring;
typedef StringList TStringList;
typedef StringIter TStringIter;
typedef StringIterC TStringIterC;

typedef StringPair TStringPair;
typedef StringPairIter TStringPairIter;
typedef StringPairList TStringPairList;

typedef StringMap TStringMap;
typedef StringMapIter TStringMapIter;

#endif

extern void startup(void (*f)(void*, const string&), void* p);
extern void shutdown();

#ifdef BUILDING_DCPP
#define PACKAGE "libdcpp"
#define LOCALEDIR Util::getLocalePath().c_str()
#define _(String) gettext(String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)
#define T_(String) Text::toT(gettext(String))
#define CT_(String) T_(String).c_str()
#define F_(String) boost::format(gettext(String))
#define FN_(String1,String2, N) boost::format(ngettext(String1, String2, N))

#ifdef UNICODE
#define TF_(String) boost::wformat(Text::toT(gettext(String)))
#define TFN_(String1,String2, N) boost::wformat(Text::toT(ngettext(String1, String2, N)))
#else
#define TF_(String) boost::format(Text::toT(gettext(String)))
#define TFN_(String1,String2, N) boost::format(Text::toT(ngettext(String1, String2, N)))
#endif

#if defined(UNICODE) && defined(_UNICODE)
#define _T(s) L ## s 
#else
#define _T(s) ## s
#endif

#endif

} // namespace dcpp

#endif // !defined(DC_PLUS_PLUS_H)
