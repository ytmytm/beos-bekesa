
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

	#include <SpLocaleApp.h>
	#include <Window.h>
	#include <sqlite.h>
	#include "globals.h"

// helper
int toint(const char *input);

// kesa data storage
#include <String.h>
class kesadat {
	public:
	kesadat(void) { dbData = NULL; clear(); };
	~kesadat() { };
	void dump_all(void);
	void clear(void);
	//
	void fetch(int nid);
	// data holders
	bool dirty;
	int id;
	// t1
	BString t1miejsc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo;
	BString t1nrobszaru, t1nrinwent, t1x, t1y, t1stanmiejsc, t1stanobszar;
	int t1zrodloinformacji;
	int t2nadmorska, t2duzedoliny, t2maledoliny, t2pozadolinami;
	int t2ekswys, t2eksstop, t2ekskier, t2ekspozycja, t2ekspozycja2;
	BString t2forma;
	int t3zabudowa, t3rodzaj;
	BString t3okreslenie;
	int t5gleba, t5kamienistosc;
	BString t5okreslenie;
	int t6obserwacja, t6pole, t6nasycenie, t6koncen, t6pow, t6gestosc;
	int t7zagrozenie, t7stale, t7przezco;
	BString t7dodatkowe;
	// database
	sqlite *dbData;
	char *dbErrMsg;
};

	class BButton;
	class BCheckBox;
	class BListView;
	class BMenu;
	class BMenuBar;
	class BTabView;
	class BTextControl;

	class BeKESAMainWindow : public BWindow {
		public:
			BeKESAMainWindow(const char *windowTitle);
			~BeKESAMainWindow();
//			virtual void FrameResized(float width, float height);
			virtual void MessageReceived(BMessage *Message);
//			virtual void DispatchMessage(BMessage *message, BHandler *handler);
			virtual bool QuitRequested();
//			virtual void RefsReceived(BMessage *Message);
		private:
			// database handlers
			int OpenDatabase(void);
			void CloseDatabase(void);
			void RefreshIndexList(void);
			void FetchCurdata(int id);
			bool CommitCurdata(bool haveCancelButton = true);	// action?
			void DoCommitCurdata(void);
			void DoDeleteCurdata(void);
			int GenerateId(void);
			// tab handlers
			void initTabs(BTabView *tv);
			void curdataFromTabs(void);
			void curdata2Tabs(void);
			// init - setup widgets, call update
			// update - enable/disable widgets, parse msg (combo items)
			// curdata2 - read data from curtab to widgets, call update
			// curdatafrom - read from widgets to curtab
			// tab1
			void initTab1(BTabView *tv);
			void updateTab1(BMessage *msg = NULL);
			void curdataFromTab1(void);
			void curdata2Tab1(void);
			// tab2
			void initTab2(BTabView *tv);
			void updateTab2(BMessage *msg = NULL);
			void curdataFromTab2(void);
			void curdata2Tab2(void);
			// tab3
			void initTab3(BTabView *tv);
			void updateTab3(BMessage *msg = NULL);
			void curdataFromTab3(void);
			void curdata2Tab3(void);
			// tab4
			void initTab4(BTabView *tv);
			void updateTab4(BMessage *msg = NULL);
			void curdataFromTab4(void);
			void curdata2Tab4(void);
			// tab5
			void initTab5(BTabView *tv);
			void updateTab5(BMessage *msg = NULL);
			void curdataFromTab5(void);
			void curdata2Tab5(void);

			// action
			void ChangedSelection(int newid);
			// menu
			void PrintCurdata(void);
			status_t PageSetup(const char *documentname);

			// main view widgets
			BMenuBar *menuBar;
			BMenu *menu;
			BListView *listView;
			BTabView *tabView;

			// bottom navigation/control
			BButton *but_new, *but_clear, *but_restore, *but_delete, *but_save;
			// tab1 controls
			BTextControl *t1miejsc, *t1nazwalokalna, *t1gmina, *t1powiat, *t1wojewodztwo;
			BTextControl *t1nrobszaru, *t1nrinwent, *t1x, *t1y, *t1stanmiejsc, *t1stanobszar;
			BCheckBox *t1cz, *t1ct, *t1cl, *t1cw, *t1ca, *t1cp, *t1cr, *t1cs;
			// tab2 controls
			BCheckBox *t2nadmorska, *t2nm, *t2np, *t2ni, *t2ns, *t2nw;
			BCheckBox *t2duzedoliny, *t2dw, *t2dd, *t2dz, *t2dy, *t2db;
			BCheckBox *t2maledoliny, *t2md, *t2ms, *t2mk;
			BCheckBox *t2pozadoliny, *t2pr, *t2pf, *t2pp, *t2pg;
			// tab3 controls
			BMenuItem *t2wysitems[8], *t2stopitems[7];
			BCheckBox *t2knw, *t2knn, *t2kne, *t2kww, *t2kee, *t2ksw, *t2kss, *t2kse;
			BRadioButton *t2te, *t2to, *t2tn, *t2tb;
			BCheckBox *t2ek, *t2es, *t2ec, *t2eg, *t2ew;
			BCheckBox *t2op, *t2od, *t2ok, *t2oj;
			BTextControl *t2forma;
			// tab4 controls
			BMenuItem *t3zabitems[4], *t3kamitems[4];
			BCheckBox *t3tl, *t3ts, *t3tp, *t3to, *t3ta, *t3tr, *t3te, *t3tz;
			BTextControl *t3tokr, *t3gokr;
			BCheckBox *t3gp, *t3gg, *t3gt;
			// tab5 controls
			BMenuItem *t6obsitems[3], *t6politems[3], *t6nasitems[3];
			BMenuItem *t6konitems[4], *t6powitems[8], *t6gesitems[4];
			BMenuItem *t7zagitems[3], *t7staitems[3];
			BCheckBox *t7zl, *t7zn, *t7zp, *t7zs;
			BTextControl *t7tdodatkowe;

			// database
			sqlite *dbData;
			char *dbErrMsg;
			// data holders
			kesadat *curdata;	// must exist BEFORE tabs are created
			int currentid;	// id of currently edited data, <0 -> INSERT >=0 -> UPDATE
			int *idlist;	// ids of listView data

			// printing
			BMessage *printSettings;	// z pagesetup
	};

#endif
