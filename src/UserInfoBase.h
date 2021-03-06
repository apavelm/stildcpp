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


#ifndef USERINFOBASE_H_
#define USERINFOBASE_H_

#include <QTabWidget>

//#include "hub_win.h"
//#include "mainwindowimpl.h"

//
#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
#include "client/forward.h"
#include "client/ResourceManager.h"
#include "client/QueueManager.h"
#include "client/LogManager.h"
#include "client/FavoriteManager.h"
#include "client/UploadManager.h"
#include "client/User.h"
//

//#include "PrivateFrame.h"
#define VS_VERSION_INFO 1

#define IDR_MAINFRAME				100
#define IDR_PUBLICHUBS				101
#define IDR_SEARCH					102
#define IDR_FAVORITES				103
#define IDR_PRIVATE					104
#define IDR_DIRECTORY				105
#define IDR_HUB						106
#define IDR_NOTEPAD					107
#define IDR_QUEUE					108
#define IDR_FINISHED_DL				109
#define IDR_FINISHED_UL				110
#define IDR_ADLSEARCH				111
#define IDR_USERS					112
#define IDR_SPY						113
#define IDR_NET_STATS				114
#define IDR_WAITING_USERS			115
#define IDR_MAGNET					116

#define IDB_FOLDERS					150
#define IDB_ARROWS					151
#define IDB_USERS					152
#define IDB_TOOLBAR					153
#define IDB_TOOLBAR20				154
#define IDB_TOOLBAR20_HOT			155

#define IDD_ABOUTBOX 				200
#define IDD_ADLS_PROPERTIES			201
#define IDD_ADVANCED3PAGE			202
#define IDD_ADVANCEDPAGE			203
#define IDD_APPEARANCE2PAGE			204
#define IDD_APPEARANCEPAGE			205
#define IDD_CERTIFICATESPAGE		206
#define IDD_DOWNLOADPAGE			207
#define IDD_FAVORITE_DIRSPAGE		208
#define IDD_FAVORITEHUB				209
#define IDD_GENERALPAGE				210
#define IDD_HASH_PROGRESS			211
#define IDD_HUB_LIST				212
#define IDD_LINE					213
#define IDD_LOGPAGE					214
#define IDD_MAGNET					215
#define IDD_NETWORKPAGE				216
#define IDD_QUEUEPAGE				217
#define IDD_TABSPAGE				218
#define IDD_UCPAGE					219
#define IDD_UPLOADPAGE				220
#define IDD_USER_COMMAND			221
#define IDD_WINDOWSPAGE				222
#define IDD_SETTINGS				223

// Stuff not in the resource file
#define IDC_EXIT 					1000
#define IDC_SYSTEM_LOG				1001
#define IDC_QUICK_CONNECT			1003
#define IDC_RECONNECT				1004
#define IDC_SEARCH					1005
#define IDC_VIEW_STATUS_BAR			1006
#define IDC_VIEW_TOOLBAR			1007
#define IDC_VIEW_TRANSFER_VIEW		1008
#define IDC_PUBLIC_HUBS				1009
#define IDC_FAVORITE_HUBS			1010
#define IDC_GETLIST					1011
#define IDC_COPY_FILENAME			1012
#define IDC_GRANTSLOT				1013
#define IDC_ADD_TO_FAVORITES		1014
#define IDC_PRIVATEMESSAGE			1015
#define IDC_ADL_SEARCH				1016
#define IDC_PRIORITY_PAUSED			1017
#define IDC_PRIORITY_LOWEST			1018
#define IDC_PRIORITY_LOW			1019
#define IDC_PRIORITY_NORMAL			1020
#define IDC_PRIORITY_HIGH			1021
#define IDC_PRIORITY_HIGHEST		1022
#define IDC_SEARCH_ALTERNATES		1023
#define IDC_BITZI_LOOKUP			1024
#define IDC_COPY_MAGNET				1025
#define IDC_MOVE					1026
#define IDC_FINISHED_DL				1027
#define IDC_FINISHED_UL				1028
#define IDC_VIEW_AS_TEXT			1029
#define IDC_OPEN_FILE				1030
#define IDC_OPEN_FOLDER				1031
#define IDC_REMOVE_ALL				1032
#define IDC_SETTINGS				1033
#define IDC_SEARCH_SPY				1034
#define IDC_DOWNLOAD				1035
#define IDC_GO_TO_DIRECTORY			1036
#define IDC_DOWNLOAD_BROWSE			1037
#define IDC_OPEN_OWN_LIST			1038
#define IDC_DOWNLOADTO				1039
#define IDC_DOWNLOADDIRTO			1040
#define IDC_COPY_NICK				1041
#define IDC_FORCE					1042
#define IDC_MATCH_ALL				1043
#define IDC_OPEN_DOWNLOADS			1044
#define IDC_WAITING_USERS			1045
#define IDC_NET_STATS				1046
#define IDC_CLOSE_ALL_DISCONNECTED	1053
#define IDC_CLOSE_ALL_PM			1054
#define IDC_CLOSE_ALL_OFFLINE_PM	1055
#define IDC_CLOSE_ALL_DIR_LIST		1056
#define IDC_CLOSE_ALL_SEARCH_FRAME	1057
#define IDC_HELP_CONTENTS			1058
#define IDC_HELP_CHANGELOG			1059
#define IDC_ABOUT					1060
#define IDC_HELP_HOMEPAGE			1061
#define IDC_HELP_DOWNLOADS			1062
#define IDC_HELP_GEOIPFILE			1063
#define IDC_HELP_TRANSLATIONS		1064
#define IDC_HELP_FORUM				1065
#define IDC_HELP_DISCUSS			1066
#define IDC_HELP_REQUEST_FEATURE	1067
#define IDC_HELP_REPORT_BUG			1068
#define IDC_HELP_DONATE				1069
#define IDC_MATCH_QUEUE				1070
#define IDC_COPY_HUB				1071
#define IDC_CLOSE_WINDOW			1072
#define IDC_TRAY_SHOW				1073
#define IDC_TRAY_QUIT				1074

// Stuff that uses multiple id's
#define IDC_BROWSELIST				3000
#define IDC_REMOVE_SOURCE			3200
#define IDC_REMOVE_SOURCES			3400
#define IDC_READD					3600
#define IDC_PM						3800
#define IDC_DOWNLOAD_FAVORITE_DIRS	4000
#define IDC_DOWNLOAD_TARGET			4200
#define IDC_DOWNLOAD_LASTDIR		4400
#define IDC_DOWNLOAD_WHOLE_DIR		4600
#define IDC_DOWNLOAD_WHOLE_FAVORITE_DIRS	4800
#define IDC_DOWNLOAD_WHOLE_TARGET	5000
#define IDC_USER_COMMAND			5200
#define ID_SHELLCONTEXTMENU_MIN				7000
#define ID_SHELLCONTEXTMENU_MAX				8000

#define IDC_ADD 2000
#define IDC_ADD_MENU 2001
#define IDC_ADLSP_DESTINATION 2002
#define IDC_ADLSP_SEARCH 2003
#define IDC_ADLSP_SIZE_MAX 2004
#define IDC_ADLSP_SIZE_MIN 2005
#define IDC_ADLSP_TYPE 2006
#define IDC_ADLSP_UNITS 2007
#define IDC_ADVANCED_BOOLEANS 2008
#define IDC_APPEARANCE_BOOLEANS 2009
#define IDC_AUTODROP_ELAPSED 2010
#define IDC_AUTODROP_FILESIZE 2011
#define IDC_AUTODROP_INACTIVITY 2012
#define IDC_AUTODROP_INTERVAL 2013
#define IDC_AUTODROP_MINSOURCES 2014
#define IDC_AUTODROP_SPEED 2015
#define IDC_AUTOQUEUE 2016
#define IDC_AUTO_REFRESH_TIME 2017
#define IDC_AUTO_SEARCH_LIMIT 2018
#define IDC_BEEPFILE 2019
#define IDC_BEEP_NOTIFICATION 2020
#define IDC_BIND_ADDRESS 2021
#define IDC_BOLD_BOOLEANS 2022
#define IDC_BROWSE 2023
#define IDC_BROWSE_CERTIFICATE 2024
#define IDC_BROWSEDIR 2025
#define IDC_BROWSE_LOG 2026
#define IDC_BROWSE_PRIVATE_KEY 2027
#define IDC_BROWSETEMPDIR 2028
#define IDC_BROWSE_TRUSTED_PATH 2029
#define IDC_BUFFERSIZE 2030
#define IDC_CHANGE_MENU 2031
#define IDC_COLOREXAMPLE 2032
#define IDC_COMMAND 2033
#define IDC_CONFIRM_OPTIONS 2034
#define IDC_CONNECT 2035
#define IDC_CONNECTION 2036
#define IDC_CURRENT_FILE 2037
#define IDC_DEFAULT_AWAY_MESSAGE 2038
#define IDC_DESCRIPTION 2039
#define IDC_DEST_DIR 2040
#define IDC_DIRECT 2041
#define IDC_DIRECTORIES 2042
#define IDC_DIRECT_OUT 2043
#define IDC_DOWNLOADDIR 2044
#define IDC_DOWNLOADS 2045
#define IDC_EDIT 2046
#define IDC_EMAIL 2047
#define IDC_EXTERNAL_IP 2048
#define IDC_FAVORITE_DIRECTORIES 2049
#define IDC_FAVORITES 2050
#define IDC_FAVUSERS 2051
#define IDC_FH_ADDRESS 2052
#define IDC_FH_HUB 2053
#define IDC_FH_HUB_DESC 2054
#define IDC_FH_IDENT 2055
#define IDC_FH_NAME 2056
#define IDC_FH_NICK 2057
#define IDC_FH_PASSWORD 2058
#define IDC_FH_USER_DESC 2059
#define IDC_FILES_PER_HOUR 2060
#define IDC_FILTER_FOCUS 2061
#define IDC_FIREWALL_NAT 2062
#define IDC_FIREWALL_PASSIVE 2063
#define IDC_FIREWALL_UPNP 2064
#define IDC_FOLLOW 2065
#define IDC_GENERATE_CERTS 2066
#define IDC_HASH_INDEXING 2067
#define IDC_HASH_PROGRESS 2068
#define IDC_HASH_SPEED 2069
#define IDC_HELP_FAQ 2070
#define IDC_HUB 2071
#define IDC_HUBADDR 2072
#define IDC_HUBDESCR 2073
#define IDC_HUBNAME 2074
#define IDC_HUBNICK 2075
#define IDC_HUBPASS 2076
#define IDC_HUBUSERDESCR 2077
#define IDC_IS_ACTIVE 2078
#define IDC_LANGUAGE 2079
#define IDC_LATEST 2080
#define IDC_LINE 2081
#define IDC_LIST_ADD 2082
#define IDC_LIST_DESC 2083
#define IDC_LIST_DOWN 2084
#define IDC_LIST_EDIT 2085
#define IDC_LIST_EDIT_BOX 2086
#define IDC_LIST_LIST 2087
#define IDC_LIST_REMOVE 2088
#define IDC_LIST_UP 2089
#define IDC_LOG_DIRECTORY 2090
#define IDC_LOG_FILE 2091
#define IDC_LOG_FORMAT 2092
#define IDC_LOG_OPTIONS 2093
#define IDC_MAGNET_DISP_HASH 2094
#define IDC_MAGNET_DISP_NAME 2095
#define IDC_MAGNET_1_QUEUE 2096
#define IDC_MAGNET_2_SEARCH 2097
#define IDC_MAGNET_3_NOTHING 2098
#define IDC_MAGNET_HASH 2099
#define IDC_MAGNET_NAME 2100
#define IDC_MAGNET_REMEMBER 2101
#define IDC_MAGNET_TEXT 2102
#define IDC_MAX_FILELIST_SIZE 2103
#define IDC_MAX_FILE_SIZE 2104
#define IDC_MAX_HASH_SPEED 2105
#define IDC_MAXSPEED 2106
#define IDC_MAX_TAB_ROWS 2107
#define IDC_MENU_ITEMS 2108
#define IDC_MIN_FILE_SIZE 2109
#define IDC_MIN_UPLOAD_SPEED 2110
#define IDC_MIN_UPLOAD_SPIN 2111
#define IDC_MOVE_DOWN 2112
#define IDC_MOVE_UP 2113
#define IDC_NAME 2114
#define IDC_NEWFAV 2115
#define IDC_NICK 2116
#define IDC_NOTEPAD 2117
#define IDC_OPEN_FILE_LIST 2118
#define IDC_OTHER_QUEUE_OPTIONS 2119
#define IDC_OVERRIDE 2120
#define IDC_PORT_TCP 2121
#define IDC_PORT_TLS 2122
#define IDC_PORT_UDP 2123
#define IDC_PRIO_HIGHEST_SIZE 2124
#define IDC_PRIO_HIGH_SIZE 2125
#define IDC_PRIO_LOW_SIZE 2126
#define IDC_PRIO_NORMAL_SIZE 2127
#define IDC_PRIVATE_ID 2128
#define IDC_PRIVATE_MESSAGE_BEEP 2129
#define IDC_PRIVATE_MESSAGE_BEEP_OPEN 2130
#define IDC_PROXY 2131
#define IDC_PUB_LIST_CONFIG 2132
#define IDC_QUEUE 2133
#define IDC_RATIO 2134
#define IDC_REFRESH 2135
#define IDC_REFRESH_FILE_LIST 2136
#define IDC_REMOVE 2137
#define IDC_REMOVE_MENU 2138
#define IDC_RENAME 2139
#define IDC_RESULT 2140
#define IDC_ROLLBACK 2141
#define IDC_SEARCH_HISTORY 2142
#define IDC_SEARCH_HISTORY_SPIN 2143
#define IDC_SEARCH_STRING 2144
#define IDC_SELTEXT 2145
#define IDC_SELWINCOLOR 2146
#define IDC_SEND_MESSAGE 2147
#define IDC_SET_MINISLOT_SIZE 2148
#define IDC_SETTINGS_APPEARANCE_OPTIONS 2149
#define IDC_SETTINGS_AUTODROP 2150
#define IDC_SETTINGS_AUTODROP_ELAPSED 2151
#define IDC_SETTINGS_AUTODROP_FILESIZE 2152
#define IDC_SETTINGS_AUTODROP_INACTIVITY 2153
#define IDC_SETTINGS_AUTODROP_INTERVAL 2154
#define IDC_SETTINGS_AUTODROP_MINSOURCES 2155
#define IDC_SETTINGS_AUTODROP_SPEED 2156
#define IDC_SETTINGS_AUTO_OPEN 2157
#define IDC_SETTINGS_AUTOPRIO 2158
#define IDC_SETTINGS_AUTO_REFRESH_TIME 2159
#define IDC_SETTINGS_AUTO_SEARCH_LIMIT 2160
#define IDC_SETTINGS_B 2161
#define IDC_SETTINGS_BIND_ADDRESS 2162
#define IDC_SETTINGS_BOLD_CONTENTS 2163
#define IDC_SETTINGS_BPS 2164
#define IDC_SETTINGS_CHAT 2165
#define IDC_SETTINGS_COLORS 2166
#define IDC_SETTINGS_COMMAND 2167
#define IDC_SETTINGS_CONFIRM_OPTIONS 2168
#define IDC_SETTINGS_CONTEXT 2169
#define IDC_SETTINGS_DEFAULT_AWAY_MSG 2170
#define IDC_SETTINGS_DESCRIPTION 2171
#define IDC_SETTINGS_DIRECTORIES 2172
#define IDC_SETTINGS_DOWNLOAD_BAR_COLOR 2173
#define IDC_SETTINGS_DOWNLOAD_DIRECTORY 2174
#define IDC_SETTINGS_DOWNLOAD_LIMITS 2175
#define IDC_SETTINGS_DOWNLOADS_MAX 2176
#define IDC_SETTINGS_DOWNLOADS_SPEED_PAUSE 2177
#define IDC_SETTINGS_EMAIL 2178
#define IDC_SETTINGS_FAVORITE_DIRECTORIES 2179
#define IDC_SETTINGS_FILELIST_MENU 2180
#define IDC_SETTINGS_FILE_NAME 2181
#define IDC_SETTINGS_FORMAT 2182
#define IDC_SETTINGS_HUB 2183
#define IDC_SETTINGS_HUB_MENU 2184
#define IDC_SETTINGS_INCOMING 2185
#define IDC_SETTINGS_IP 2186
#define IDC_SETTINGS_KB 2187
#define IDC_SETTINGS_KB2 2188
#define IDC_SETTINGS_KB3 2189
#define IDC_SETTINGS_KB4 2190
#define IDC_SETTINGS_KB5 2191
#define IDC_SETTINGS_KB6 2192
#define IDC_SETTINGS_KB7 2193
#define IDC_SETTINGS_KBPS 2194
#define IDC_SETTINGS_LANGUAGE_FILE 2195
#define IDC_SETTINGS_LIST_CONFIG 2196
#define IDC_SETTINGS_LOG_DIR 2197
#define IDC_SETTINGS_LOGGING 2198
#define IDC_SETTINGS_MAX_FILELIST_SIZE 2199
#define IDC_SETTINGS_MAX_HASH_SPEED 2200
#define IDC_SETTINGS_MAX_TAB_ROWS 2201
#define IDC_SETTINGS_MB 2202
#define IDC_SETTINGS_MBS 2203
#define IDC_SETTINGS_MEBIBITS 2204
#define IDC_SETTINGS_NAME 2205
#define IDC_SETTINGS_NICK 2206
#define IDC_SETTINGS_ONCE 2207
#define IDC_SETTINGS_ONLY_HASHED 2208
#define IDC_SETTINGS_OTHER_QUEUE_OPTIONS 2209
#define IDC_SETTINGS_OUTGOING 2210
#define IDC_SETTINGS_PARAMETERS 2211
#define IDC_SETTINGS_PERSONAL_INFORMATION 2212
#define IDC_SETTINGS_PM 2213
#define IDC_SETTINGS_PM_HISTORY 2214
#define IDC_SETTINGS_PORTS 2215
#define IDC_SETTINGS_PORT_TCP 2216
#define IDC_SETTINGS_PORT_TLS 2217
#define IDC_SETTINGS_PORT_UDP 2218
#define IDC_SETTINGS_PRIO_HIGH 2219
#define IDC_SETTINGS_PRIO_HIGHEST 2220
#define IDC_SETTINGS_PRIO_LOW 2221
#define IDC_SETTINGS_PRIO_NORMAL 2222
#define IDC_SETTINGS_PUBLIC_HUB_LIST 2223
#define IDC_SETTINGS_PUBLIC_HUB_LIST_HTTP_PROXY 2224
#define IDC_SETTINGS_PUBLIC_HUB_LIST_URL 2225
#define IDC_SETTINGS_RAW 2226
#define IDC_SETTINGS_REQUIRES_RESTART 2227
#define IDC_SETTINGS_ROLLBACK 2228
#define IDC_SETTINGS_S1 2229
#define IDC_SETTINGS_S2 2230
#define IDC_SETTINGS_S3 2231
#define IDC_SETTINGS_SEARCH_HISTORY 2232
#define IDC_SETTINGS_SEARCH_MENU 2233
#define IDC_SETTINGS_SEPARATOR 2234
#define IDC_SETTINGS_SHARED_DIRECTORIES 2235
#define IDC_SETTINGS_SHARE_SIZE 2236
#define IDC_SETTINGS_SOCKET_IN_BUFFER 2237
#define IDC_SETTINGS_SOCKET_OUT_BUFFER 2238
#define IDC_SETTINGS_SOCKS5_IP 2239
#define IDC_SETTINGS_SOCKS5_PASSWORD 2240
#define IDC_SETTINGS_SOCKS5_PORT 2241
#define IDC_SETTINGS_SOCKS5_USERNAME 2242
#define IDC_SETTINGS_SOUNDS 2243
#define IDC_SETTINGS_SPEEDS_NOT_ACCURATE 2244
#define IDC_SETTINGS_TEXT_MINISLOT 2245
#define IDC_SETTINGS_TIME_STAMPS_FORMAT 2246
#define IDC_SETTINGS_TO 2247
#define IDC_SETTINGS_TYPE 2248
#define IDC_SETTINGS_UNFINISHED_DOWNLOAD_DIRECTORY 2249
#define IDC_SETTINGS_UPLOAD_BAR_COLOR 2250
#define IDC_SETTINGS_UPLOAD_LINE_SPEED 2251
#define IDC_SETTINGS_UPLOADS_MIN_SPEED 2252
#define IDC_SETTINGS_UPLOADS_SLOTS 2253
#define IDC_SETTINGS_USER_MENU 2254
#define IDC_SETTINGS_WINDOWS_OPTIONS 2255
#define IDC_SETTINGS_WRITE_BUFFER 2256
#define IDC_SHAREHIDDEN 2257
#define IDC_SHOW_LAST_LINES_LOG 2258
#define IDC_SIZE_TYPE 2259
#define IDC_SLOTS 2260
#define IDC_SLOTSPIN 2261
#define IDC_SLOTSSPIN 2262
#define IDC_SOCKET_IN_BUFFER 2263
#define IDC_SOCKET_OUT_BUFFER 2264
#define IDC_SOCKS5 2265
#define IDC_SOCKS_PASSWORD 2266
#define IDC_SOCKS_PORT 2267
#define IDC_SOCKS_RESOLVE 2268
#define IDC_SOCKS_SERVER 2269
#define IDC_SOCKS_USER 2270
#define IDC_SOURCE_TYPE 2271
#define IDC_SPEEDSPIN 2272
#define IDC_STATIC 2273
#define IDC_STATISTICS 2274
#define IDC_TEMP_DOWNLOAD_DIRECTORY 2275
#define IDC_THANKS 2276
#define IDC_TIME_LEFT 2277
#define IDC_TIME_STAMPS_FORMAT 2278
#define IDC_TLS_CERTIFICATE_FILE 2279
#define IDC_TLS_OPTIONS 2280
#define IDC_TLS_PRIVATE_KEY_FILE 2281
#define IDC_TLS_TRUSTED_CERTIFICATES_PATH 2282
#define IDC_TOTAL 2283
#define IDC_TOTALS 2284
#define IDC_TTH 2285
#define IDC_USER_CMD_OPEN_HELP 2286
#define IDC_USER_CMD_PREVIEW 2287
#define IDC_VERSION 2288
#define IDC_WINDOWS_OPTIONS 2289
#define IDC_WINDOWS_STARTUP 2290
#define IDC_SETTINGS_PAGES 2291
#define IDC_SETTINGS_MIN_SEGMENT_SIZE 2292
#define IDC_MIN_SEGMENT_SIZE 2293


class UserInfoBase {
public:
	UserInfoBase(const dcpp::UserPtr& u) : user(u) { }

	void getList();
	void browseList();
	void matchQueue();
	void pm(QWidget*);
	void grant();
	void addFav();
	void removeAll();
	void connectFav(QWidget*);

	dcpp::UserPtr& getUser() { return user; }
	dcpp::UserPtr user;
	
	struct UserTraits {
		UserTraits() : adcOnly(true), favOnly(true), nonFavOnly(true) { }
		void operator()(UserInfoBase* ui);

		bool adcOnly;
		bool favOnly;
		bool nonFavOnly;
	};

};

template<class T>
class AspectUserInfo {
public:
	typedef AspectUserInfo<T> ThisType;
	
	void handleMatchQueue() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::matchQueue);
	}
	void handleGetList() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::getList);
	}
	void handleBrowseList() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::browseList);
	}
	void handleAddFavorite() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::addFav);
	}
	// std::tr1::bind(&UserInfoBase::connectFav, _1, parent) doesn't seem to work with g++ svn 2007-07-30...
	// wonder if it's me or the implementation as boost::bind/function swallows it...
	struct Caller {
		Caller(QWidget* parent_, void (UserInfoBase::*f_)(QWidget*)) : parent(parent_), f(f_) { }
		void operator()(UserInfoBase* uib) { (uib->*f)(parent); }
		QWidget* parent;
		void (UserInfoBase::*f)(QWidget*);
	};
	
	void handlePrivateMessage(QWidget* parent) {
		static_cast<T*>(this)->getUserList()->forEachSelectedT(Caller(parent, &UserInfoBase::pm));
	}
	void handleGrantSlot() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::grant);
	}
	void handleRemoveAll() {
		static_cast<T*>(this)->getUserList()->forEachSelected(&UserInfoBase::removeAll);
	}
	void handleConnectFav(QWidget* parent) {
		static_cast<T*>(this)->getUserList()->forEachSelectedT(Caller(parent, &UserInfoBase::connectFav));
	}
/*
	template<typename MenuType>
	void appendUserItems(QWidget* parent, MenuType menu) {
		T* This = static_cast<T*>(this);
		UserInfoBase::UserTraits traits = This->getUserList()->forEachSelectedT(UserInfoBase::UserTraits());
		
		//menu = new QMenu(this);
		QAction a = new QAction(tr("Get File List"), this);
		a->setStatusTip(tr("Get FileList From User"));
		a->setToolTip(tr("Get user filelist"));
		connect(a, SIGNAL(triggered()), this, SLOT(&T::handleGetList);
		menu->addAction(a);
		
		
		menu->appendItem(IDC_GETLIST, TSTRING(GET_FILE_LIST), std::tr1::bind(&T::handleGetList, This));
		if(traits.adcOnly)
			menu->appendItem(IDC_BROWSELIST, TSTRING(BROWSE_FILE_LIST), std::tr1::bind(&T::handleBrowseList, This));
		menu->appendItem(IDC_MATCH_QUEUE, TSTRING(MATCH_QUEUE), std::tr1::bind(&T::handleMatchQueue, This));
		menu->appendItem(IDC_PRIVATEMESSAGE, TSTRING(SEND_PRIVATE_MESSAGE), std::tr1::bind(&T::handlePrivateMessage, This, parent));
		if(!traits.favOnly)
			menu->appendItem(IDC_ADD_TO_FAVORITES, TSTRING(ADD_TO_FAVORITES), std::tr1::bind(&T::handleAddFavorite, This));
		menu->appendItem(IDC_GRANTSLOT, TSTRING(GRANT_EXTRA_SLOT), std::tr1::bind(&T::handleGrantSlot, This));
		if(!traits.nonFavOnly)
			menu->appendItem(IDC_CONNECT, TSTRING(CONNECT_FAVUSER_HUB), std::tr1::bind(&T::handleConnectFav, This, parent));
		menu->appendSeparatorItem();
		menu->appendItem(IDC_REMOVE_ALL, TSTRING(REMOVE_FROM_ALL), std::tr1::bind(&T::handleRemoveAll, This));
	}*/
};

#endif /*USERINFOBASE_H_*/
