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

#include "config.h"
/*
Config* Config::m_instance = 0;

Config::Config() :

	QSettings("StilDC++ Client", "StilDC++ Client") {

	// Path for cache
	m_cachePath = fileName().section('/', 0, -2, QString::SectionSkipEmpty | QString::SectionIncludeLeadingSep) + "/";

	// Path for system wide, and user's data files
	m_systemPath = QString(PREFIX) + "/share/stildcpp/";
	m_userPath = QDir::homePath() + "/.local/share/stildcpp/";

	setObjectName("config");
}

Config *Config::instance() {
	if (!m_instance) {
		m_instance = new Config;
		IconManager::update();
	}
	return m_instance;
}

QString Config::cachePath() const {
	return m_cachePath;
}

QString Config::systemPath() const {
	return m_systemPath;
}

QString Config::userPath() const {
	return m_userPath;
}

// SERVER SETTINGS


// MPD Host
void Config::setServers(const QList<ServerInfo> &servers) {
	QVariantList res;
	foreach(ServerInfo s, servers) {
		QVariantMap map;
		map.insert("name", s.name());
		map.insert("address", s.address());
		map.insert("port", s.port());
		map.insert("password", s.password());
		res += map;
	}
	setValue("/mpd/servers", res);
	emit serverListChanged(servers);
}

QList<ServerInfo> Config::servers() const {
	QList<ServerInfo> res;
	QVariantList vl = value("/mpd/servers").toList();
	if (vl.isEmpty()) {
		ServerInfo si("Localhost");
		res << si;
		return res;
	}
	foreach(QVariant var, vl) {
		QVariantMap map = var.toMap();
		ServerInfo si(map.value("name").toString(),
		              map.value("address").toString(),
		              map.value("port").toInt(),
		              map.value("password").toString());
		res << si;
	}
	return res;
}

ServerInfo Config::server(const QString &name) const {
	QList<ServerInfo> sil = servers();
	foreach(ServerInfo si, sil) {
		if (si.name() == name)
			return si;
	}
	DEBUG("Server `%s' not found, returning first in list.", qPrintable(name));
	// No matches, return first server
	return sil.first();
}

void Config::setAutoconnect(bool e) {
	setValue("/mpd/autoconnect", e);
}

void Config::setReconnect(bool r) {
	setValue("/mpd/reconnect", r);
}

void Config::setReconnectTime(int t) {
	setValue("/mpd/reconnecttime", t);
}

void Config::setTimeoutTime(int t) {
	setValue("/mpd/timeout", t);
}

bool Config::autoconnect() const {
	return value("/mpd/autoconnect").toBool();
}

bool Config::reconnect() const {
	return value("/mpd/reconnect").toBool();
}

int Config::reconnectTime() const {
	return value("/mpd/reconnecttime", 60).toInt();
}

int Config::timeoutTime() const {
	return value("/mpd/timeout", 10).toInt();
}

// Look and feel settings
void Config::setAlternatingRowColors(bool a) {
	if (a != alternatingRowColors())
		emit alternatingChanged(a);
	setValue("/looknfeel/alternatingrowcolors", a);
}

void Config::setEnqueue(bool e) {
	setValue("/looknfeel/enqueue", e);
}

void Config::setFilterByAlbumOnly(bool a) {
	setValue("/looknfeel/filter_by_album_only", a);
	emit filterByAlbumOnlyChanged(a);
}

void Config::setFont(const QFont &font) {
	setValue("/looknfeel/font", font);
	emit fontChanged(font);
}

void Config::setOpaqueResize(bool o) {
	setValue("/looknfeel/opaqueresize", o);
	emit opaqueResizeChanged(o);
}

void Config::setRecurse(bool r) {
	setValue("/looknfeel/recursiveview", r);
}

void Config::setShowAll(bool r) {
	setValue("/looknfeel/showall", r);
	emit showAllChanged(r);
}

void Config::setShowRemaining(bool r) {
	setValue("/looknfeel/showremaning", r);
}

void Config::setAutoResizeColumns(bool a) {
	if (a != autoResizeColumns())
		emit autoResizeChanged(a);
	setValue("/looknfeel/autoresizecolumns", a);
}

bool Config::alternatingRowColors() const {
	return value("/looknfeel/alternatingrowcolors", true).toBool();
}

bool Config::enqueue() const {
	return value("/looknfeel/enqueue", false).toBool();
}

bool Config::filterByAlbumOnly() const {
	return value("/looknfeel/filter_by_album_only", false).toBool();
}

bool Config::opaqueResize() const {
	return value("/looknfeel/opaqueresize", false).toBool();
}

bool Config::recurse() const {
	return value("/looknfeel/recursiveview", false).toBool();
}

bool Config::showAll() const {
	return value("/looknfeel/showall", false).toBool();
}

bool Config::showRemaining() const {
	return value("/looknfeel/showremaning", false).toBool();
}

QFont Config::font() const {
	return qvariant_cast<QFont>(value("/looknfeel/font"));
}

bool Config::autoResizeColumns() const {
	return value("/looknfeel/autoresizecolumns", true).toBool();
}

// Icon set
void Config::setIconSet(const QString &dir) {
	QString orig = iconSetPath();
	setValue("/looknfeel/iconset", dir);
	if (orig != dir) {
		IconManager::update();
		emit iconSetChanged();
	}
}

QString Config::iconSetPath() const {
	QString path = value("/looknfeel/iconset", ":/icons/crystal_amarok").toString();
	return path;
}

// Playlist
void Config::setPlaylistPattern(const QString &p) {
	QString orig = playlistPattern();
	setValue("/looknfeel/playlistpattern", p);
	if (orig != p)
		emit playlistPatternChanged(p);
}

void Config::setScrollToPlaying(bool s) {
	setValue("/looknfeel/scrolltoplaying", s);
}

QString Config::playlistPattern() const {
	return value("/looknfeel/playlistpattern", "%b - %t").toString();
}

bool Config::scrollToPlaying() const {
	return value("/looknfeel/scrolltoplaying", false).toBool();
}

// Style
void Config::setStyleFile(const QString &s) {
	QString orig = styleFile();
	setValue("/looknfeel/style", s);
	if (orig != s)
		emit styleChanged(style(s));
}

QString Config::styleFile() const {
	return value("/looknfeel/style").toString();
}

QString Config::style(const QString &fn) const {
	QString filename = !fn.isEmpty() ? fn : styleFile();
	if (filename.isEmpty())
		return QString();
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		DEBUG("Style file not readable: %s", qPrintable(filename));
		return QString();
	}
	QString style = file.readAll();
	file.close();
	return style;
}

// Shortcuts
void Config::setOriginalShortcut(const QString &action, const QString &key) {
	m_originalShortcuts[action] = key;
}

void Config::setShortcut(const QString &action, const QString &key) {
	setValue(QString("/shortcuts/%1").arg(action), key);
}

QString Config::shortcut(const QString &action) const {
	QString shortcut = value(QString("/shortcuts/%1").arg(action)).toString();
	if (shortcut.isEmpty())
		return m_originalShortcuts.value(action);
	return shortcut;
}

// Language
void Config::setTranslate(bool t) {
	bool old = translate();
	setValue("/language/translate", t);
	if (t != old)
		emit localeChanged(locale());
}

void Config::setLocale(const QString &l) {
	QString old = locale();
	setValue("/language/locale", l);
	if (l != old)
		emit localeChanged(localeFile());
}

bool Config::translate() const {
	return value("/language/translate", false).toBool();
}

QString Config::locale() const {
	return value("/language/locale", "").toString();
}

QString Config::localeFile() const {
	QString tmp = locale();
	if (tmp.isEmpty()) {
		QString filename = QLocale::system().name() + ".qm";
		QDir systemDir(systemPath() + "translations", "??_??.qm");
		if (systemDir.entryList(QDir::Files | QDir::Readable).contains(filename))
			return systemDir.absolutePath() + "/" + filename;
		QDir userDir(userPath() + "translations", "??_??.qm");
		if (userDir.entryList(QDir::Files | QDir::Readable).contains(filename))
			return userDir.absolutePath() + "/" + filename;
	}
	return tmp;
}

// Notifications
void Config::setNotificationsEnabled(bool e) {
	setValue("/notifications/enabled", e);
}

void Config::setNotificationsPosition(int p) {
	setValue("/notifications/position", p);
}

void Config::setNotificationsTimeout(int t) {
	setValue("/notifications/timeout", t);
}

void Config::setNotifier(int t) {
	setValue("/notifications/type", t);
}

bool Config::notificationsEnabled() const {
	return value("/notifications/enabled", true).toBool();
}

int Config::notificationsPosition() const {
	return value("/notifications/position", PassivePopup::BottomRight).toInt();
}

int Config::notificationsTimeout() const {
	return value("/notifications/timeout", 3).toInt();
}

int Config::notifier() const {
	return value("/notifications/type", 0).toInt();
}

// System tray

void Config::setTrayIconEnabled(bool e) {
	bool old = trayIconEnabled();
	setValue("/trayicon/enable", e);
	if (old != e)
		emit trayIconChanged(e);
}

void Config::setMinimizeToTray(bool e) {
	setValue("/trayicon/minimizetotray", e);
}

void Config::setStartHidden(bool e) {
	setValue("/trayicon/starthidden", e);
}

bool Config::trayIconEnabled() const {
	return value("/trayicon/enable", true).toBool();
}

bool Config::minimizeToTray() const {
	return value("/trayicon/minimizetotray", true).toBool();
}

bool Config::startHidden() const {
	return value("/trayicon/starthidden").toBool();
}

// Tag-guesser settings

void Config::setTagGuesserEnabled(bool e) {
	setValue("/tagguesser/enabled", e);
}

void Config::setGuessPattern(const QString &p) {
	setValue("/tagguesser/pattern", p);
}

void Config::setTestFilename(const QString &t) {
	setValue("/tagguesser/test", t);
}

bool Config::tagGuesserEnabled() const {
	return value("/tagguesser/enabled", false).toBool();
}

QString Config::guessPattern() const {
	return value("/tagguesser/pattern", "/%b/%a/%n. %t.").toString();
}

QString Config::testFilename() const {
	return value("/tagguesser/test", "/ogg/Artist/Album/42. The Title.ogg").toString();
}

// Dynamic playlist

void Config::setAutoAddSongs(bool a) {
	setValue("/dynamicplaylist/add", a);
}

void Config::setAutoAddCount(int c) {
	setValue("/dynamicplaylist/count", c);
}

void Config::setAutoAddPos(int p) {
	setValue("/dynamicplaylist/pos", p);
}

void Config::setAutoRemoveSongs(bool r) {
	setValue("/dynamicplaylist/remove", r);
}

bool Config::autoAddSongs() const {
	return value("/dynamicplaylist/add", false).toBool();
}

int Config::autoAddCount() const {
	return value("/dynamicplaylist/count", 2).toInt();
}

int Config::autoAddPos() const {
	return value("/dynamicplaylist/pos", 0).toInt();
}

bool Config::autoRemoveSongs() const {
	return value("/dynamicplaylist/remove", false).toBool();
}

// Window and splitter sizes
++ Client
void Config::setWindowSize(const QSize &s) {
	setValue("/qmpdclient/windowsize", s);
}

void Config::setRightBarTab(int t) {
	setValue("/qmpdclient/righttab", t);
}

void Config::setDirectorySplitterSizes(const QByteArray &l) {
	setValue("/qmpdclient/directorysplittersizes", l);
}

void Config::setLibraryHSplitterSizes(const QByteArray &l) {
	setValue("/qmpdclient/libraryhsplittersizes", l);
}

void Config::setLibraryVSplitterSizes(const QByteArray &l) {
	setValue("/qmpdclient/libraryvsplittersizes", l);
}

void Config::setMainSplitterSizes(const QByteArray &l) {
	setValue("/qmpdclient/mainsplittersizes", l);
}

void Config::setPlaylistsSplitterSizes(const QByteArray &l) {
	setValue("/qmpdclient/playlistssplittersizes", l);
}

QSize Config::windowSize() const {
	return value("/qmpdclient/windowsize", QSize(800, 600)).toSize();
}

QByteArray Config::directorySplitterSizes() const {
	return value("/qmpdclient/directorysplittersizes").toByteArray();
}

QByteArray Config::libraryHSplitterSizes() const {
	return value("/qmpdclient/libraryhsplittersizes").toByteArray();
}

QByteArray Config::libraryVSplitterSizes() const {
	return value("/qmpdclient/libraryvsplittersizes").toByteArray();
}

QByteArray Config::mainSplitterSizes() const {
	return value("/qmpdclient/mainsplittersizes").toByteArray();
}

QByteArray Config::playlistsSplitterSizes() const {
	return value("/qmpdclient/playlistssplittersizes").toByteArray();
}

// Header sizes
void Config::setHeaderViewState(const QString &view, const QByteArray &ba) {
	setValue(QString("/headerstates/%1").arg(view), ba);
}

QByteArray Config::headerViewState(const QString &view) const {
	return value(QString("/headerstates/%1").arg(view)).toByteArray();
}

// Radio stations
 
void Config::addRadioStation(const QString &title, const QString &url) {
	QMap<QString, QVariant> map = value("/radioview/radiostations").toMap();
	map[url] = title;
	setValue("/radioview/radiostations", map);
}

void Config::removeRadioStations(const MPDSongList &stations) {
	QMap<QString, QVariant> map = value("/radioview/radiostations").toMap();
	foreach(MPDSong s, stations) {
		map.remove(s.url());
	}
	setValue("/radioview/radiostations", map);
}

*/

