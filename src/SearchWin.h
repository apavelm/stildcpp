/***************************************************************************
 *   Copyright (C) 2007 - 2008 by Pavel Andreev                            *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *   Copyright (C) 2007 - 2008 by Yakov Suraev aka BigBiker                *
 *   Mail: adminbsd on gmail point com (adminbsd@gmail.com)                *
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

#ifndef __SEARCHWIN_H__
#define __SEARCHWIN_H__

#include "mdi_c.h"


#include "client/stdinc.h"
#include "client/DCPlusPlus.h"
//#include "client/forward.h"
#include "client/ResourceManager.h"
#include "client/SearchManager.h"
#include "client/ClientManagerListener.h"
#include "client/StringTokenizer.h"
#include "UserInfoBase.h"

#include "ui_search.h"
#include "stilutils.h"

#include <QTimer>
#include <QStandardItemModel>

class SearchWindow : public MdiChild, private Ui::mdiSEARCHwin, private SearchManagerListener, private ClientManagerListener
{
	Q_OBJECT
private slots:
	void SearchBtn();
	void purgeClicked();
	void zeroStatusChanged(int state);
	bool eachSecond();
	int handleSpeaker(unsigned int wParam, long lParam);
	void hubStateChanged(QListWidgetItem * item);

	void actionDownload();
	void actionDownloadTo();
	void actionDownloadWholeDir();
	void	actionDownloadWholeDirTo();
	void actionViewAsText();
	void actionSearchAlt();
	void actionBitzi();
	void actionCopyMagnetToClipboard();
	void actionGetFilelist();
	void actionMatchQueue();
	void actionSendPM();
	void actionAddToFavorites();
	void actionGrandExtraSlot();
	void actionRemoveUserFromQueue();
	void actionRemove();
	void showSearchMenu(const QPoint &point);

signals:
	void speakerSignal(unsigned int wParam, long lParam=0);
	
	
public:
	SearchWindow(QWidget *parent, const tstring& initialString_, int64_t initialSize_, SearchManager::SizeModes initialMode, SearchManager::TypeModes initialType_);
	~SearchWindow();

	enum Status
	{
		STATUS_SHOW_UI,
		STATUS_STATUS,
		STATUS_COUNT,
		STATUS_FILTERED,
		STATUS_LAST
	};

private:
	/*
	class HubList : private Ui::mdiSEARCHwin
	{
	public:
		HubList();
	};
	
	HubList *hubs;
	*/
	enum Speakers {
		SPEAK_ADD_RESULT,
		SPEAK_FILTER_RESULT,
		SPEAK_HUB_ADDED,
		SPEAK_HUB_CHANGED,
		SPEAK_HUB_REMOVED
	};

	enum {
		COLUMN_FIRST,
		COLUMN_FILENAME = COLUMN_FIRST,
		COLUMN_NICK,
		COLUMN_TYPE,
		COLUMN_SIZE,
		COLUMN_PATH,
		COLUMN_SLOTS,
		COLUMN_CONNECTION,
		COLUMN_HUB,
		COLUMN_EXACT_SIZE,
		COLUMN_IP,
		COLUMN_TTH,
		COLUMN_CID,
		COLUMN_LAST
	};

	class SearchInfo : public UserInfoBase {
	public:
		SearchInfo(SearchResult* aSR) : UserInfoBase(aSR->getUser()), sr(aSR) {
			sr->incRef(); update();
		}
		~SearchInfo() {
			sr->decRef();
		}

//		void getList();
//		void browseList();

		void view();
		void Download(const tstring& tgt);
		void DownloadWhole(const tstring& tgt);
//		void DownloadTarget(const tstring& aTarget);
		
		/*
		struct Download {
			Download(const tstring& aTarget) : tgt(aTarget) { }
			void operator()(SearchInfo* si);
			const tstring& tgt;
		};
		struct DownloadWhole {
			DownloadWhole(const tstring& aTarget) : tgt(aTarget) { }
			void operator()(SearchInfo* si);
			const tstring& tgt;
		};
		struct DownloadTarget {
			DownloadTarget(const tstring& aTarget) : tgt(aTarget) { }
			void operator()(SearchInfo* si);
			const tstring& tgt;
		};
		struct CheckTTH {
			CheckTTH() : firstHubs(true), op(true), hasTTH(false), firstTTH(true) { }
			void operator()(SearchInfo* si);
			bool firstHubs;
			StringList hubs;
			bool op;
			bool hasTTH;
			bool firstTTH;
			tstring tth;
		};
		*/
		const tstring& getText(int col) const { return columns[col]; }
		QIcon* getImage() const;

//		static int compareItems(SearchInfo* a, SearchInfo* b, int col);

		void update();

		SearchResult* sr;

		tstring columns[COLUMN_LAST];
	};

	struct HubInfo : public FastAlloc<HubInfo> {
		HubInfo(const tstring& aUrl, const tstring& aName, bool aOp) : url(aUrl),
			name(aName), op(aOp) { }

		const tstring& getText() const //int col) const {
		{
			//return (col == 0) ? name : Util::emptyStringT;
			return name;
		}
		/*
		int getImage() const {
			return 0;
		}*
		*/
		static int compareItems(HubInfo* a, HubInfo* b)//, int col) {
		{
//			return (col == 0) ? lstrcmpi(a->name.c_str(), b->name.c_str()) : 0;
			return QString::compare(StilUtils::TstrtoQ(a->name), StilUtils::TstrtoQ(b->name), Qt::CaseInsensitive);
		}

		tstring url;
		tstring name;
		bool op;
	};

	CriticalSection cs;
	std::string token;
	size_t droppedResults;
	TStringList currentSearch;
	static TStringList lastSearches;

	tstring initialString;
	int64_t initialSize;
	SearchManager::SizeModes initialMode;
	SearchManager::TypeModes initialType;

	bool isHash;
	bool onlyFree;

	// I know: this is ugly
	void setStatus(int s, const tstring& text);
	void setStatus(int s, const QString& text);
	//May be replace setText() with setTabText() simply...
	void setText(const tstring& text);
	void setText(const QString& text);

	void initSecond();

	// SearchManagerListener
	virtual void on(SearchManagerListener::SR, SearchResult* aResult) throw();

	// ClientManagerListener
	virtual void on(ClientConnected, Client* c) throw() { speak(SPEAK_HUB_ADDED, c); }
	virtual void on(ClientUpdated, Client* c) throw() { speak(SPEAK_HUB_CHANGED, c); }
	virtual void on(ClientDisconnected, Client* c) throw() { speak(SPEAK_HUB_REMOVED, c); }

	void onHubAdded(HubInfo* info);
	void onHubChanged(HubInfo* info);
	void onHubRemoved(HubInfo* info);

	void speak(Speakers s, Client* aClient){};

	// Use QMap for replacing TypedListView of original client
	QMap<HubInfo*, QListWidgetItem*> hubMap;
	QMap<QStandardItem*, SearchInfo*> searchMap;

	void initHubs();
	void initSearchList();
	void initSearchMenu();
	void runSearch();

	void insertSearchResult(SearchInfo* si);
	void clearSearchResult();
	void removeSearchResult(QStandardItem* item);
	
	QStandardItemModel *model;
	QTimer* timer;
	QMenu *userMenu;
	QList<QAction*> fantomActions;
	
	//reimplemented thiz
	void speak(unsigned int, long=0);
};

#endif // __SEARCHWIN_H__
