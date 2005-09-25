//
// start: open db, ask for dbfile if not found
// select first from list, if list empty - act as if new and untouched?
//
// currentid < 0 -> INSERT, currentid > 0 -> UPDATE
//
// deal with database file like in bsap (sqlite_open never fails)
//
// buttons: New, Save, Delete, Clear; 1st, last, next, prev
//

#include "mainwindow.h"
#include <View.h>
#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <TabView.h>
#include <TextControl.h>
#include <stdio.h>

const uint32 MENU_DEFMSG 	= 'M000';
const uint32 BUT_NEW		= 'Bnew';
const uint32 BUT_SAVE		= 'Bsav';
const uint32 LIST_INV		= 'Linv';
const uint32 LIST_SEL		= 'Lsel';
const uint32 TC1			= 'TC01';

BeKESAMainWindow::BeKESAMainWindow(const char *windowTitle) : BWindow(
	BRect(100, 100, 740, 580), windowTitle, B_DOCUMENT_WINDOW, B_OUTLINE_RESIZE, B_CURRENT_WORKSPACE ) {

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
	mainView->AddChild(new BScrollView("scrollView", listView, B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true));
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
	but_new = new BButton(BRect(170,440,230,470), "but_new", "New", new BMessage(BUT_NEW), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_save = new BButton(BRect(570,440,630,470), "but_save", "Save", new BMessage(BUT_SAVE), B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	mainView->AddChild(but_new);
	mainView->AddChild(but_save);

	tabView->Select(0);

	// get memory for objects
	idlist = NULL;
	curdata = new kesadat();
	newdata = new kesadat();
	// initialize database
	int ret = OpenDatabase();
	if (ret < 0)
		exit(1);
	// initialize list
	RefreshIndexList();
//	XXX act like this one is new or select the first one?
	currentid = -1;
}

BeKESAMainWindow::~BeKESAMainWindow() {
	CloseDatabase();
}

void BeKESAMainWindow::initTabs(BTabView *tv) {
	initTab1(tv);
}

void BeKESAMainWindow::curdataFromTabs(void) {
	curdataFromTab1();
	curdata->dirty = true;
}

void BeKESAMainWindow::curdata2Tabs(void) {
	curdata2Tab1();
}

void BeKESAMainWindow::initTab1(BTabView *tv) {
	BTab *tab;
	BBox *box, *box2;
	BRect r,s,sl,sr;

	r = tv->Bounds();
	r.InsetBy(5,10);
	BView *view = new BView(r, "viewtab1", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	tab = new BTab(view);
	tv->AddTab(view, tab);
	tab->SetLabel("Lokalizacja");

	r = view->Bounds();
	r.top = 10; r.left = 10; r.right -= 10;
	r.bottom = r.top + 100;
	box = new BBox(r, "box1x1");
	box->SetLabel("Miejscowość");
	view->AddChild(box);
	// text controls for box1
	sl = box->Bounds();
	sl.right = sl.left+sl.right/2;
	sl.top = 10; sl.left = 10;
	sl.bottom = sl.top + 20; sl.right -= 10;
	sr = sl;
	sr.left = sl.left + sl.Width() + 10;
	sr.right = box->Bounds().right - 10;
	t1miejsc = new BTextControl(sl, "t1miejsc", "Miejscowość", NULL, new BMessage(TC1));
	t1nazwalokalna = new BTextControl(sr, "t1nazwalokalna", "Nazwa lokalna", NULL, new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1gmina	= new BTextControl(sl, "t1gmina", "Gmina", NULL, new BMessage(TC1));
	t1powiat = new BTextControl(sr, "t1powiat", "Powiat", NULL, new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1wojewodztwo = new BTextControl(sl, "t1wojewodztwo", "Województwo", NULL, new BMessage(TC1));

	box->AddChild(t1miejsc);
	box->AddChild(t1nazwalokalna);
	box->AddChild(t1gmina);
	box->AddChild(t1powiat);
	box->AddChild(t1wojewodztwo);

	r.top += 115; r.bottom += 115;
	box = new BBox(r, "box1x2");
	box->SetLabel("Obszar");
	view->AddChild(box);
	// text controls for box2
	sl = box->Bounds();
	sl.right = sl.left+sl.Width()/2;
	sl.top = 10; sl.left = 10;
	sl.bottom = sl.top + 20; sl.right -= 10;
	sr = sl;
	sr.left = sl.left + sl.Width() + 10;
	sr.right = box->Bounds().right - 10;
	t1nrobszaru = new BTextControl(sl, "t1nrobszaru", "Nr obszaru AZP", NULL, new BMessage(TC1));
	t1nrinwent = new BTextControl(sr, "t1nrinwent", "Nr inwentarza", NULL, new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
		s = sr; s.bottom += 30;
		box2 = new BBox(s, "box1x3");
		box2->SetLabel("Współrzędne na mapie 1:25000");
		s = box2->Bounds();
		s.InsetBy(10,15);
		s.right = s.left + s.Width()/2;
		t1x = new BTextControl(s, "t1x", "X", NULL, new BMessage(TC1));
		box2->AddChild(t1x);
		s.OffsetBy(s.Width(), 0);
		t1y = new BTextControl(s, "t1y", "Y", NULL, new BMessage(TC1));
		box2->AddChild(t1y);
	t1stanmiejsc = new BTextControl(sl, "t1stanmiejsc", "Nr stan/miejsc", NULL, new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1stanobszar = new BTextControl(sl, "t1stanobszar", "Nr stan/obszar", NULL, new BMessage(TC1));

	box->AddChild(t1nrobszaru);
	box->AddChild(t1nrinwent);
	box->AddChild(t1stanmiejsc);
	box->AddChild(t1stanobszar);
	box->AddChild(box2);

	r.top += 115; r.bottom += 115;
	box = new BBox(r, "box1x3");
	box->SetLabel("Źródła informacji");
	view->AddChild(box);
	// text controls for box3
	sl = box->Bounds();
	sl.right = sl.left+sl.Width()/3;
	sl.top = 10; sl.left = 10;
	sl.bottom = sl.top + 20; sl.right -= 10;
	sr = sl;
	sr.OffsetBy(sl.Width(), 0);
	s = sl;
	s.OffsetBy(sl.Width()*2, 30);

	t1cz = new BCheckBox(sl, "t1cz", "z wywiadu", new BMessage(TC1));
	t1cw = new BCheckBox(sr, "t1cw", "wykopaliskowe", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1ct = new BCheckBox(sl, "t1ct", "terenowe", new BMessage(TC1));
	t1ca = new BCheckBox(sr, "t1ca", "archiwalne", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1cl = new BCheckBox(sl, "t1cl", "luźne", new BMessage(TC1));
	t1cp = new BCheckBox(sr, "t1cp", "powierzchniowe", new BMessage(TC1));
	t1cr = new BCheckBox(s, "t1cr", "rekonstrukcja lokaliz.", new BMessage(TC1));

	box->AddChild(t1cz);
	box->AddChild(t1ct);
	box->AddChild(t1cl);
	box->AddChild(t1cw);
	box->AddChild(t1ca);
	box->AddChild(t1cp);
	box->AddChild(t1cr);
}

void BeKESAMainWindow::curdata2Tab1(void) {
	t1miejsc->SetText(curdata->t1miejsc.String());
	t1nazwalokalna->SetText(curdata->t1nazwalokalna.String());
	t1gmina->SetText(curdata->t1gmina.String());
	t1powiat->SetText(curdata->t1powiat.String());
	t1wojewodztwo->SetText(curdata->t1wojewodztwo.String());
	t1nrobszaru->SetText(curdata->t1nrobszaru.String());
	t1nrinwent->SetText(curdata->t1nrinwent.String());
	t1x->SetText(curdata->t1x.String());
	t1y->SetText(curdata->t1y.String());
	t1stanmiejsc->SetText(curdata->t1stanmiejsc.String());
	t1stanobszar->SetText(curdata->t1stanobszar.String());
	int t1 = curdata->t1zrodloinformacji;
	t1cz->SetValue((t1 & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1ct->SetValue((t1 & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cl->SetValue((t1 & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cw->SetValue((t1 & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1ca->SetValue((t1 & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cp->SetValue((t1 & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cr->SetValue((t1 & 0x0040) ? B_CONTROL_ON : B_CONTROL_OFF);
}

void BeKESAMainWindow::curdataFromTab1(void) {
	curdata->t1miejsc = t1miejsc->Text();
	curdata->t1nazwalokalna = t1nazwalokalna->Text();
	curdata->t1gmina = t1gmina->Text();
	curdata->t1powiat = t1powiat->Text();
	curdata->t1wojewodztwo = t1wojewodztwo->Text();
	curdata->t1nrobszaru = t1nrobszaru->Text();
	curdata->t1nrinwent = t1nrinwent->Text();
	curdata->t1x = t1x->Text();
	curdata->t1y = t1y->Text();
	curdata->t1stanmiejsc = t1stanmiejsc->Text();
	curdata->t1stanobszar = t1stanobszar->Text();
	int t1zrodlo = 0;
	if (t1cz->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0001;
	if (t1ct->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0002;
	if (t1cl->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0004;
	if (t1cw->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0008;
	if (t1ca->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0010;
	if (t1cp->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0020;
	if (t1cr->Value() == B_CONTROL_ON)
		t1zrodlo |= 0x0040;
	curdata->t1zrodloinformacji = t1zrodlo;
}

void BeKESAMainWindow::MessageReceived(BMessage *Message) {
	int i;
	this->DisableUpdates();
	switch (Message->what) {
		case TC1:
			// update internal state of tab1 or mark it dirty
			curdataFromTabs();
//			curdata->dump_all();
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
		case BUT_SAVE:
			DoCommitCurdata();
			break;
		case LIST_SEL:
			printf("list selection\n");
			i = listView->CurrentSelection(0);
			printf("got:%i\n",i);
			if (i>=0) {
				printf("sel:%i,id=%i\n",i,idlist[i]);
				ChangedSelection(idlist[i]);
				currentindex = i;	// XXX really need to remember that?
			} else {
				// XXX deselection? what to do???
			}
			break;
		case LIST_INV:
			printf("list invocation\n");
			i = listView->CurrentSelection(0);
			printf("got:%i\n",i);
			if (i>=0) {
				/// XXX do smth here
				printf("sel:%i,id=%i\n",i,idlist[i]);
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
	if (! CommitCurdata()) {
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
	int ret;
	if (!curdata->dirty)
		return true;
	// ask if store
	BAlert *ask;
	if (haveCancelButton)
		ask = new BAlert(APP_NAME, "Zapisać zmiany w aktualnej karcie?", "Tak", "Nie", "Anuluj", B_WIDTH_AS_USUAL, B_IDEA_ALERT);
	else
		ask = new BAlert(APP_NAME, "Zapisać zmiany w aktualnej karcie?", "Tak", "Nie", NULL, B_WIDTH_AS_USUAL, B_IDEA_ALERT);
	ret = ask->Go();
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
			DoCommitCurdata();
	};
	return true;
}

// insert/update curdata unconditionally
void BeKESAMainWindow::DoCommitCurdata(void) {
	BString sql;
	int ret, newid;
	printf("in commit data\n");
	// maybe a real UPDATE should be done
	if (currentid >= 0) {
		// remove current and fall through insert
		ret = sqlite_exec_printf(dbData, "DELETE FROM karta WHERE id = %i", 0, 0, &dbErrMsg, currentid);
		printf("del: %i, %s\n",ret, dbErrMsg);
	}
	newid = GenerateId();
	curdata->id = newid;
	// INSERT INTO karta (t1,t2..) VALUES (v1,v2..)
	sql = "INSERT INTO karta (id, ";
	sql += "t1miejscowosc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo, t1nrobszaru, t1nrinwentarza, t1x, t1y, t1nrstanmiejsc, t1nrstanobszar, t1zrodloinformacji";
	sql += ") VALUES ( %i, ";
	sql += "%Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %i";	// t1
	sql += ")";
	printf("inssql:%s\n",sql.String());
	ret = sqlite_exec_printf(dbData, sql.String(), 0, 0, &dbErrMsg, curdata->id,
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
		curdata->t1zrodloinformacji
	);
//	printf("result: %i, %s; newid=%i\n", ret, dbErrMsg, newid);
	currentid = curdata->id;
	curdata->dirty = false;
	RefreshIndexList();	//XXX here or later?
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
		void *anItem;
		for (int i=0; (anItem=listView->ItemAt(i)); i++)
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
		return;
	}
	idlist = new int[nRows];
	for (int i=1;i<=nRows;i++) {
		idlist[i-1] = toint(result[i*nCols+0]);
		listView->AddItem(new BStringItem(result[i*nCols+1]));
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
	BString sqlQuery;	
	sqlQuery = "SELECT ";
	sqlQuery += "t1miejscowosc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo, t1nrobszaru, t1nrinwentarza, t1x, t1y, t1nrstanmiejsc, t1nrstanobszar, t1zrodloinformacji";
	sqlQuery += " FROM karta WHERE id = ";
	sqlQuery << id;
printf("sql:%s\n",sqlQuery.String());
	sqlite_get_table(dbData, sqlQuery.String(), &result, &nRows, &nCols, &dbErrMsg);
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
	printf("\n");
}

void kesadat::clear(void) {
	dirty = false;
	id = -1;
	t1miejsc = t1nazwalokalna = t1gmina = t1powiat = t1wojewodztwo = t1nrobszaru = t1nrinwent = t1x = t1y = t1stanmiejsc = t1stanobszar = "";
	t1zrodloinformacji = 0;
}

//---------------------

#include <stdlib.h>

int toint(const char *input) {
	if (input != NULL)
		return strtol(input, NULL, 10);
	else
		return 0;
}
