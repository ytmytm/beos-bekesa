//
// start: open db, ask for dbfile if not found
// deal with database file like in bsap (sqlite_open never fails)
//	(if database doesn't exist - create it with embedded schema?)
//
// open: new or 1st from list?
//
// nav buttons (or menu?): 1st, last, next, prev
//
// order list with sth; write 2/more fields (nr inw, etc)
//
// note: with current id generation PRIMARY KEY(id,t1miescowosc,etc.) may be back
//       and id must not be auto_increment augumented
// ^^^XXX DONE, CHECKIT; NEEDS to enforce id&&t1miejscowosc!=NULL
//						 or not... id is always different
//
// note: tabs functions should be exported to different files as BView derivs
//		 (what about messages? must be passed here? NO - updateTab only)
// note: rewrite combine_checkboxes as struct { size, cb_list } and common set of functions
//
// tabadd: init, update, cur2, curfrom, msg, kesaclear, kesadump
//			update, insert, select

#include "mainwindow.h"
#include <View.h>
#include <Alert.h>
#include <Button.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <TabView.h>
#include <stdio.h>

const uint32 MENU_DEFMSG 	= 'M000';
const uint32 BUT_NEW		= 'Bnew';
const uint32 BUT_CLEAR		= 'Bclr';
const uint32 BUT_RESTORE	= 'Bres';
const uint32 BUT_DELETE		= 'Bdel';
const uint32 BUT_SAVE		= 'Bsav';
const uint32 LIST_INV		= 'Linv';
const uint32 LIST_SEL		= 'Lsel';
// XXX these are repeated in tab*.cpp files!
const uint32 TC1			= 'TC01';
const uint32 TC2			= 'TC02';
const uint32 TC3			= 'TC03';
const uint32 TC3W			= 'TC3W';
const uint32 TC3S			= 'TC3S';
const uint32 TC3dookolna	= 'TC3D';
const uint32 TC3wyczysc		= 'TC3C';

BeKESAMainWindow::BeKESAMainWindow(const char *windowTitle) : BWindow(
	BRect(100, 100, 740, 580), windowTitle, B_DOCUMENT_WINDOW, B_OUTLINE_RESIZE, B_CURRENT_WORKSPACE ) {

	// get memory for objects
	idlist = NULL;
	curdata = new kesadat();

	BView *mainView = new BView(this->Bounds(), "mainView", B_FOLLOW_ALL_SIDES, 0);

	if (mainView == NULL) {
		AppReturnValue = B_NO_MEMORY;
		be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}

	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	this->AddChild(mainView);

	BRect r;
	r = mainView->Bounds();
	r.bottom = 20;
	menuBar = new BMenuBar(r, "menuBar");
	mainView->AddChild(menuBar);

	menu = new BMenu("Plik", B_ITEMS_IN_COLUMN);
	((SpLocaleApp*)be_app)->AddToFileMenu(menu,false,false,false);
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Item", new BMessage(MENU_DEFMSG), 0, 0));
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	menuBar->AddItem(menu);

	// listview in scrollview
	r = mainView->Bounds();
	r.right = 150-20; r.top = 20;
	listView = new BListView(r, "listView");
	mainView->AddChild(new BScrollView("scrollView", listView, B_FOLLOW_LEFT|B_FOLLOW_TOP_BOTTOM, 0, false, true));
	listView->SetInvocationMessage(new BMessage(LIST_INV));
	listView->SetSelectionMessage(new BMessage(LIST_SEL));

	// tabview
	r = mainView->Bounds();
	r.left = 150; r.top = 20; r.bottom = r.bottom - 50;
	tabView = new BTabView(r, "tabView");
	mainView->AddChild(tabView);

	// initialize widgets
	initTabs(tabView);

	// buttons
	but_new = new BButton(BRect(170,440,230,470), "but_new", "Nowy", new BMessage(BUT_NEW), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_clear = new BButton(BRect(250,440,310,470), "but_clear", "Wyczyść", new BMessage(BUT_CLEAR), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_restore = new BButton(BRect(330,440,390,470), "but_restore", "Przywróć", new BMessage(BUT_RESTORE), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_delete = new BButton(BRect(410,440,470,470), "but_delete", "Usuń", new BMessage(BUT_DELETE), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_save = new BButton(BRect(570,440,630,470), "but_save", "Zapisz", new BMessage(BUT_SAVE), B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	mainView->AddChild(but_new);
	mainView->AddChild(but_clear);
	mainView->AddChild(but_restore);
	mainView->AddChild(but_delete);
	mainView->AddChild(but_save);

	tabView->Select(0);

	// initialize database
	int ret = OpenDatabase();
	if (ret < 0)
		exit(1);
	// initialize list
	RefreshIndexList();
//	XXX act like if this one is new or select the first one?
	currentid = -1;
}

BeKESAMainWindow::~BeKESAMainWindow() {
	CloseDatabase();
}

void BeKESAMainWindow::initTabs(BTabView *tv) {
	initTab1(tv);
	initTab2(tv);
	initTab3(tv);
}

void BeKESAMainWindow::curdataFromTabs(void) {
	curdataFromTab1();
	curdataFromTab2();
	curdataFromTab3();
}

void BeKESAMainWindow::curdata2Tabs(void) {
	curdata2Tab1();
	curdata2Tab2();
	curdata2Tab3();

	BString tmp;
	tmp = APP_NAME;
	tmp += ": ";
	tmp += curdata->t1miejsc;
	tmp += "; nr inwent.: ";
	tmp += curdata->t1nrinwent;	// XXX dodac jeszcze # faktow, nr obszaru i nrstanobszar
	this->SetTitle(tmp.String());
}

void BeKESAMainWindow::MessageReceived(BMessage *Message) {
	int i;
	this->DisableUpdates();
	switch (Message->what) {
		case TC1:
			curdata->dirty = true;
			updateTab1(Message);
			break;
		case TC2:
			curdata->dirty = true;
			updateTab2(Message);
			break;
		case TC3:
		case TC3W:
		case TC3S:
		case TC3dookolna:
		case TC3wyczysc:
			curdata->dirty = true;
			updateTab3(Message);
			break;
		case BUT_NEW:
			if (CommitCurdata()) {
				// clear curdata
				curdata->clear();
				currentid = -1;
				// refresh tabs
				curdata2Tabs();
			}
			break;
		case BUT_CLEAR:
			curdata->clear();
			curdata->dirty = true;
			curdata2Tabs();
			break;
		case BUT_RESTORE:
			if (currentid >= 0) {
				FetchCurdata(currentid);
				curdata2Tabs();
			}
			break;
		case BUT_DELETE:
			DoDeleteCurdata();
			currentid = -1;
			curdata2Tabs();
			break;
		case BUT_SAVE:
			curdataFromTabs();
			DoCommitCurdata();
			curdata2Tabs();
			break;
		case LIST_SEL:
		case LIST_INV:
			printf("list selection/invoc\n");
			i = listView->CurrentSelection(0);
			printf("got:%i\n",i);
			if (i>=0) {
				printf("sel:%i,id=%i\n",i,idlist[i]);
				ChangedSelection(idlist[i]);
//				currentindex = i;	// XXX really need to remember that?
			} else {
				// XXX deselection? what to do???
			}
			break;
		case MENU_DEFMSG:
			RefreshIndexList();
			break;
		default:
			BWindow::MessageReceived(Message);
			break;
	}
	this->EnableUpdates();
}

void BeKESAMainWindow::ChangedSelection(int newid) {
	if (!(CommitCurdata())) {
		// XXX do nothing if cancel, restore old selection?
		return;
	}
	// fetch and store into new data
//	printf("fetching for index=%i\n",newid);
	FetchCurdata(newid);
	curdata2Tabs();
	currentid = newid;
}

bool BeKESAMainWindow::QuitRequested() {
//	config->position = this->Frame();
//	config->save();
	CommitCurdata(false);
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

//--------------------
// database stuff, abstract it?

// if returns false -> cancel action and resume editing current data
bool BeKESAMainWindow::CommitCurdata(bool haveCancelButton = true) {
	if (!(curdata->dirty))
		return true;
	// ask if store
	BAlert *ask;
	if (haveCancelButton)
		ask = new BAlert(APP_NAME, "Zapisać zmiany w aktualnej karcie?", "Tak", "Nie", "Anuluj", B_WIDTH_AS_USUAL, B_IDEA_ALERT);
	else
		ask = new BAlert(APP_NAME, "Zapisać zmiany w aktualnej karcie?", "Tak", "Nie", NULL, B_WIDTH_AS_USUAL, B_IDEA_ALERT);
	int ret = ask->Go();
	switch (ret) {
		case 2:
//printf("cancel\n");
			return false;
			break;
		case 1:
//printf("nie\n");
			break;
		case 0:
		default:
//printf("commiting data\n");
			curdataFromTabs();
			DoCommitCurdata();
	};
	return true;
}

// insert/update curdata unconditionally
void BeKESAMainWindow::DoCommitCurdata(void) {
	BString sql;
	int ret;
	printf("in commit data\n");
	if (currentid >= 0) {	// UPDATE
		sql = "UPDATE karta SET ";
		sql += "t1miejscowosc = %Q, t1nazwalokalna = %Q, t1gmina = %Q, t1powiat = %Q, t1wojewodztwo = %Q, t1nrobszaru = %Q, t1nrinwentarza = %Q, t1x = %Q, t1y = %Q, t1nrstanmiejsc = %Q, t1nrstanobszar = %Q, t1zrodloinformacji = %i";	// t1
		sql += ", t2nadmorska = %i, t2duzedoliny = %i, t2maledoliny = %i, t2pozadolinami = %i";	// t2
		sql += ", t2ekswys = %i, t2eksstop = %i, t2ekskier = %i, t2ekspozycja = %i, t2ekspozycja2 = %i, t2forma = %Q"; // t3
		sql += " WHERE id = %i";
	} else {	// INSERT
		int newid = GenerateId();
		curdata->id = newid;
		sql = "INSERT INTO karta (";
		sql += "t1miejscowosc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo, t1nrobszaru, t1nrinwentarza, t1x, t1y, t1nrstanmiejsc, t1nrstanobszar, t1zrodloinformacji"; // t1
		sql += ", t2nadmorska, t2duzedoliny, t2maledoliny, t2pozadolinami";
		sql += ", t2ekswys, t2eksstop, t2ekskier, t2ekspozycja, t2ekspozycja2, t2forma";
		sql += ", id ) VALUES ( ";
		sql += "%Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %i,";	// t1
		sql += "%i, %i, %i, %i,"; // t2
		sql += "%i, %i, %i, %i, %i, %Q,"; // t3
		sql += " %i)";
	}
	printf("sql:%s\n",sql.String());
	ret = sqlite_exec_printf(dbData, sql.String(), 0, 0, &dbErrMsg,
		curdata->t1miejsc.String(),
		curdata->t1nazwalokalna.String(),
		curdata->t1gmina.String(),
		curdata->t1powiat.String(),
		curdata->t1wojewodztwo.String(),
		curdata->t1nrobszaru.String(),
		curdata->t1nrinwent.String(),
		curdata->t1x.String(),
		curdata->t1y.String(),
		curdata->t1stanmiejsc.String(),
		curdata->t1stanobszar.String(),
		curdata->t1zrodloinformacji,
		curdata->t2nadmorska,
		curdata->t2duzedoliny,
		curdata->t2maledoliny,
		curdata->t2pozadolinami,
		curdata->t2ekswys,
		curdata->t2eksstop,
		curdata->t2ekskier,
		curdata->t2ekspozycja,
		curdata->t2ekspozycja2,
		curdata->t2forma.String(),
		curdata->id
	);
	printf("result: %i, %s; id=%i\n", ret, dbErrMsg, curdata->id);
	currentid = curdata->id;
	curdata->dirty = false;
	RefreshIndexList();	//XXX here or later?
}

void BeKESAMainWindow::DoDeleteCurdata(void) {
	if (currentid >= 0) {
		int ret = sqlite_exec_printf(dbData, "DELETE FROM karta WHERE id = %i", 0, 0, &dbErrMsg, currentid);
		printf("del: %i, %s\n",ret, dbErrMsg);
	}
	curdata->clear();
	RefreshIndexList();
}

int BeKESAMainWindow::OpenDatabase(void) {
	// XXX port BSAP solution for non-existing db
	dbData = sqlite_open("kesa.sq2", 0666, &dbErrMsg);
	if ((dbData==0)||(dbErrMsg!=0)) {
		printf("database not found\n");
		return -1;
	}
	sqlite_exec(dbData, "VACUUM", 0, 0, &dbErrMsg);
	return 0;
}

void BeKESAMainWindow::CloseDatabase(void) {
	sqlite_exec(dbData, "VACUUM", 0, 0, &dbErrMsg);
	sqlite_close(dbData);
}

void BeKESAMainWindow::RefreshIndexList(void) {
	// clear current list
	if (listView->CountItems()>0) {
		BStringItem *anItem;
		for (int i=0; (anItem=(BStringItem*)listView->ItemAt(i)); i++)
			delete anItem;
		if (!listView->IsEmpty())
			listView->MakeEmpty();
	}
	// clear current idlist
	delete [] idlist;
	// select list from db
	int nRows, nCols;
	char **result;
	BString sqlQuery;

	sqlQuery = "SELECT id, t1miejscowosc FROM karta ORDER BY id";
	sqlite_get_table(dbData, sqlQuery.String(), &result, &nRows, &nCols, &dbErrMsg);
	if (nRows < 1) {
		// XXX database is empty, do sth about it?
		printf("database is empty\n");
	} else {
		idlist = new int[nRows];
		for (int i=1;i<=nRows;i++) {
			idlist[i-1] = toint(result[i*nCols+0]);
			listView->AddItem(new BStringItem(result[i*nCols+1]));
		}
	}
	sqlite_free_table(result);
}

int BeKESAMainWindow::GenerateId(void) {
	int id = 1;
	int nRows, nCols;
	char **result;
	sqlite_get_table(dbData, "SELECT MAX(id) FROM karta", &result, &nRows, &nCols, &dbErrMsg);
	if (nRows > 0) {
		// there is something in db
		id = toint(result[1]) + 1;
	}
	sqlite_free_table(result);
	return id;
}

void BeKESAMainWindow::FetchCurdata(int id) {
//printf("in fetchcurdata with %i\n",id);
	int i;
	int nRows, nCols;
	char **result;
	BString sql;	
	sql = "SELECT ";
	sql += "t1miejscowosc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo, t1nrobszaru, t1nrinwentarza, t1x, t1y, t1nrstanmiejsc, t1nrstanobszar, t1zrodloinformacji"; // t1
	sql += ", t2nadmorska, t2duzedoliny, t2maledoliny, t2pozadolinami";	// t2
	sql += ", t2ekswys, t2eksstop, t2ekskier, t2ekspozycja, t2ekspozycja2, t2forma"; // t3
	sql += " FROM karta WHERE id = ";
	sql << id;
printf("sql:%s\n",sql.String());
	sqlite_get_table(dbData, sql.String(), &result, &nRows, &nCols, &dbErrMsg);
//printf ("got:%ix%i\n", nRows, nCols);
	// readout data
	i = nCols;
	curdata->t1miejsc = result[i++];
	curdata->t1nazwalokalna = result[i++];
	curdata->t1gmina = result[i++];
	curdata->t1powiat = result[i++];
	curdata->t1wojewodztwo = result[i++];
	curdata->t1nrobszaru = result[i++];
	curdata->t1nrinwent = result[i++];
	curdata->t1x = result[i++];
	curdata->t1y = result[i++];
	curdata->t1stanmiejsc = result[i++];
	curdata->t1stanobszar = result[i++];
	curdata->t1zrodloinformacji = toint(result[i++]);
	curdata->t2nadmorska = toint(result[i++]);
	curdata->t2duzedoliny = toint(result[i++]);
	curdata->t2maledoliny = toint(result[i++]);
	curdata->t2pozadolinami = toint(result[i++]);
	curdata->t2ekswys = toint(result[i++]);
	curdata->t2eksstop = toint(result[i++]);
	curdata->t2ekskier = toint(result[i++]);
	curdata->t2ekspozycja = toint(result[i++]);
	curdata->t2ekspozycja2 = toint(result[i++]);
	curdata->t2forma = result[i++];

	sqlite_free_table(result);
	curdata->id = id;
	curdata->dirty = false;
}

//--------------------

void kesadat::dump_all(void) {
	printf("------------\n");
	printf("miejsc:%s, nazwa: %s, gmina %s\npowiat: %s, wojewodz: %s\n",
		t1miejsc.String(), t1nazwalokalna.String(), t1gmina.String(), t1powiat.String(), t1wojewodztwo.String());
	printf("nrobsz: %s, nrinwent: %s, x: %s, y: %s, stan/msc: %s, stan/obsz: %s\n",
		t1nrobszaru.String(), t1nrinwent.String(), t1x.String(), t1y.String(), t1stanmiejsc.String(), t1stanobszar.String());
	printf("zrodlo: %x\n", t1zrodloinformacji);
	printf("nadmor: %i, duzedol: %i, maledol: %i, pozadol: %i\n",t2nadmorska, t2duzedoliny, t2maledoliny, t2pozadolinami);
	printf("wys: %i, stop: %i, kier: %i\n", t2ekswys, t2eksstop, t2ekskier);
	printf("eks: %i, eks2: %i, forma: %s\n", t2ekspozycja, t2ekspozycja2, t2forma.String());
	printf("\n");
}

void kesadat::clear(void) {
	dirty = false;
	id = -1;
	t1miejsc = t1nazwalokalna = t1gmina = t1powiat = t1wojewodztwo = t1nrobszaru = t1nrinwent = t1x = t1y = t1stanmiejsc = t1stanobszar = "";
	t1zrodloinformacji = 0;
	t2nadmorska = t2duzedoliny = t2maledoliny = t2pozadolinami = 0;
	t2ekswys = t2eksstop = t2ekskier = t2ekspozycja = t2ekspozycja2 = 0;
	t2forma = "";
}

//---------------------

#include <stdlib.h>

int toint(const char *input) {
	if (input != NULL)
		return strtol(input, NULL, 10);
	else
		return 0;
}
