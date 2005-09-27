//
// start: open db, ask for dbfile if not found
// deal with database file like in bsap (sqlite_open never fails)
//	(if database doesn't exist - create it with embedded schema?)
//
// open: new or 1st from list?
//
// currentid < 0 -> INSERT, currentid > 0 -> UPDATE
//
// nav buttons (or menu?): 1st, last, next, prev
//
// order list with sth; write 2/more fields
//
// note: with current id generation PRIMARY KEY(id,t1miescowosc,etc.) may be back
//       and id must not be auto_increment augumented
// ^^^XXX DONE, CHECKIT; NEEDS to enforce id&&t1miejscowosc!=NULL
//						 or not... id is always different
//
// note: tabs functions should be exported to different files as BView derivs
//		 (what about messages? must be passed here?)
//
// tabadd: init, update, cur2, curfrom, msg, kesaclear, kesadump
//			update, insert, select

#include "mainwindow.h"
#include <View.h>
#include <Alert.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <ListView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <RadioButton.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TabView.h>
#include <TextControl.h>
#include <stdio.h>

const uint32 MENU_DEFMSG 	= 'M000';
const uint32 BUT_NEW		= 'Bnew';
const uint32 BUT_CLEAR		= 'Bclr';
const uint32 BUT_RESTORE	= 'Bres';
const uint32 BUT_DELETE		= 'Bdel';
const uint32 BUT_SAVE		= 'Bsav';
const uint32 LIST_INV		= 'Linv';
const uint32 LIST_SEL		= 'Lsel';
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
}

void BeKESAMainWindow::curdata2Tabs(void) {
	curdata2Tab1();
	curdata2Tab2();

	BString tmp;
	tmp = APP_NAME;
	tmp += ": ";
	tmp += curdata->t1miejsc;
	tmp += "; nr inwent.: ";
	tmp += curdata->t1nrinwent;	// XXX dodac jeszcze # faktow, nr obszaru i nrstanobszar
	this->SetTitle(tmp.String());
}

void BeKESAMainWindow::initTab3(BTabView *tv) {
	BTab *tab;
	BBox *box, *box2;
	BRect r, sl, sr, sm;

	r = tv->Bounds();
	r.InsetBy(5, 10);
	BView *view = new BView(r, "viewtab3", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	tab = new BTab(view);
	tv->AddTab(view, tab);
	tab->SetLabel("Ekspozycja");

	r = view->Bounds();
	r.InsetBy(10, 10);
	r.bottom = r.top+140;

	box = new BBox(r, "box3x1");
	box->SetLabel("Stopień i kierunek ekspozycji");
	view->AddChild(box);

	sl = box->Bounds();
	sl.InsetBy(10, 20);
	sl.right = sl.left + sl.Width()/2;
	sr = sl; sr.OffsetBy(sl.Width(), 0);

	BPopUpMenu *menu;
	BMenuItem *item;
	BMessage *msg;
	menu = new BPopUpMenu("t2mwysokosc");
	msg = new BMessage(TC3W); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t2wysitems[0] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 1);
	item = new BMenuItem("0 m", msg); menu->AddItem(item);
	t2wysitems[1] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 2);
	item = new BMenuItem("0-2 m", msg); menu->AddItem(item);
	t2wysitems[2] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 3);
	item = new BMenuItem("2-4 m", msg); menu->AddItem(item);
	t2wysitems[3] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 4);
	item = new BMenuItem("4-8 m", msg); menu->AddItem(item);
	t2wysitems[4] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 5);
	item = new BMenuItem("8-15 m", msg); menu->AddItem(item);
	t2wysitems[5] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 6);
	item = new BMenuItem(">15 m", msg); menu->AddItem(item);
	t2wysitems[6] = item;
	msg = new BMessage(TC3W); msg->AddInt32("_item", 7);
	item = new BMenuItem("?", msg); menu->AddItem(item);
	t2wysitems[7] = item;
	BMenuField *t2wysokosc = new BMenuField(sl, "t2wysokosc", "Wysokość ekspozycji:", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t2wysokosc->SetDivider(130);
	box->AddChild(t2wysokosc);

	menu = new BPopUpMenu("t2mstopien");
	msg = new BMessage(TC3S); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t2stopitems[0] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 1);
	item = new BMenuItem("0-1 %", msg); menu->AddItem(item);
	t2stopitems[1] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 2);
	item = new BMenuItem("1-3 %", msg); menu->AddItem(item);
	t2stopitems[2] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 3);
	item = new BMenuItem("4-7 %", msg); menu->AddItem(item);
	t2stopitems[3] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 4);
	item = new BMenuItem("8-15 %", msg); menu->AddItem(item);
	t2stopitems[4] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 5);
	item = new BMenuItem(">15 %", msg); menu->AddItem(item);
	t2stopitems[5] = item;
	msg = new BMessage(TC3S); msg->AddInt32("_item", 6);
	item = new BMenuItem("?", msg); menu->AddItem(item);
	t2stopitems[6] = item;
	BMenuField *t2stopien = new BMenuField(sr, "t2stopien", "Stopień ekspozycji:", menu, B_FOLLOW_RIGHT, B_WILL_DRAW);
	box->AddChild(t2stopien);

	box2 = new BBox(BRect(120, 55, 250, 125), "box3x2");
	box->AddChild(box2);
	sl = box2->Bounds();
	sl.InsetBy(3, 3);
	sl.right = sl.left + sl.Width()/3;
	sl.bottom = sl.top + 20;
	sm = sl; sm.OffsetBy(sl.Width(), 0);
	sr = sm; sr.OffsetBy(sm.Width(), 0);
	t2knw = new BCheckBox(sl, "t2knw", "NW", new BMessage(TC3));
	t2knn = new BCheckBox(sm, "t2knn", "N",  new BMessage(TC3));
	t2kne = new BCheckBox(sr, "t2kne", "NE", new BMessage(TC3));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20); sr.OffsetBy(0, 20);
	t2kww = new BCheckBox(sl, "t2kee", "E",  new BMessage(TC3));
	t2kee = new BCheckBox(sr, "t2kww", "W",  new BMessage(TC3));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20); sr.OffsetBy(0, 20);
	t2ksw = new BCheckBox(sl, "t2ksw", "SW", new BMessage(TC3));
	t2kss = new BCheckBox(sm, "t2kss", "S",  new BMessage(TC3));
	t2kse = new BCheckBox(sr, "t2kse", "SE", new BMessage(TC3));
	box2->AddChild(t2knw); box2->AddChild(t2knn); box2->AddChild(t2kne);
	box2->AddChild(t2kww); box2->AddChild(t2kee);
	box2->AddChild(t2ksw); box2->AddChild(t2kss); box2->AddChild(t2kse);

	box->AddChild(new BButton(BRect(270, 55, 370, 80), "t2butfill", "Dookolna", new BMessage(TC3dookolna), B_FOLLOW_RIGHT));
	box->AddChild(new BButton(BRect(270, 100, 370, 125), "t2butclear", "Wyczyść", new BMessage(TC3wyczysc), B_FOLLOW_RIGHT));
	box->AddChild(new BStringView(BRect(10, 55, 100, 75), "t2e1", "Kierunek"));
	box->AddChild(new BStringView(BRect(10, 85, 100, 105), "t2e2", "ekspozycji"));

	r.OffsetBy(0, r.Height()+20); r.bottom = 355;
	box = new BBox(r, "box3x2");
	box->SetLabel("Rodzaj ekspozycji");
	view->AddChild(box);
	sl = box->Bounds();
	sl.InsetBy(10, 20);
	sl.right = sl.left + sl.Width()/3; sl.bottom = sl.top + 20;
	sm = sl; sm.OffsetBy(sl.Width(), 0);
	sr = sm; sr.OffsetBy(sm.Width(), 0);
	t2te = new BRadioButton(sl, "t2te", "teren eksponowany", new BMessage(TC3));
	t2to = new BRadioButton(sm, "t2to", "teren osłonięty", new BMessage(TC3));
	t2tn = new BRadioButton(sr, "t2tn", "teren nieeksponowany", new BMessage(TC3));
	sr.OffsetBy(0, 25);
	t2tb = new BRadioButton(sr, "t2tb", "[brak]", new BMessage(TC3));
	box->AddChild(t2te); box->AddChild(t2to); box->AddChild(t2tn);
	box->AddChild(t2tb);
	t2tb->SetValue(B_CONTROL_ON);
	sl.OffsetBy(0, 25); sm.OffsetBy(0, 25);
	t2ek = new BCheckBox(sl, "t2ek", "krawędź, stok", new BMessage(TC3));
	t2op = new BCheckBox(sm, "t2op", "podstawa stoku", new BMessage(TC3));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20);
	t2es = new BCheckBox(sl, "t2es", "sfałdowanie, mały cypel", new BMessage(TC3));
	t2od = new BCheckBox(sm, "t2od", "dolina denund., jar", new BMessage(TC3));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20);
	t2ec = new BCheckBox(sl, "t2ec", "cypel wybitny", new BMessage(TC3));
	t2ok = new BCheckBox(sm, "t2ok", "kotlinka", new BMessage(TC3));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20);
	t2eg = new BCheckBox(sl, "t2eg", "wał, garb terenowy", new BMessage(TC3));
	t2oj = new BCheckBox(sm, "t2oj", "jaskinia", new BMessage(TC3));
	sl.OffsetBy(0, 20);
	t2ew = new BCheckBox(sl, "t2ew", "wyniesienie okrężne", new BMessage(TC3));
	box->AddChild(t2ek); box->AddChild(t2es); box->AddChild(t2ec);
	box->AddChild(t2eg); box->AddChild(t2ew);
	box->AddChild(t2op); box->AddChild(t2od); box->AddChild(t2ok);
	box->AddChild(t2oj);
	sl.OffsetBy(0, 30); sl.right = 440;
	t2forma = new BTextControl(sl, "t2forma", "Forma szczeg.", NULL, new BMessage(TC3));
	box->AddChild(t2forma);
	t2forma->SetDivider(100);
	//updateTab3();
//	t3wysitems[7]->SetMarked(true);
}

void BeKESAMainWindow::updateTab3(BMessage *msg = NULL) {
	int32 item = 0;
	if (msg) {
		switch (msg->what) {
			case TC3W:
				if (msg->FindInt32("_item",&item) == B_OK)
					curdata->t2ekswys = item;
//				printf("tc3w: %i\n", item);
				break;
			case TC3S:
				if (msg->FindInt32("_item",&item) == B_OK)
					curdata->t2eksstop = item;
//				printf("tc3s: %i\n", item);
				break;
			case TC3dookolna:
				t2knw->SetValue(B_CONTROL_ON); t2knn->SetValue(B_CONTROL_ON); t2kne->SetValue(B_CONTROL_ON);
				t2kww->SetValue(B_CONTROL_ON); t2kee->SetValue(B_CONTROL_ON);
				t2ksw->SetValue(B_CONTROL_ON); t2kss->SetValue(B_CONTROL_ON); t2kse->SetValue(B_CONTROL_ON);
				break;
			case TC3wyczysc:
				t2knw->SetValue(B_CONTROL_OFF); t2knn->SetValue(B_CONTROL_OFF); t2kne->SetValue(B_CONTROL_OFF);
				t2kww->SetValue(B_CONTROL_OFF); t2kee->SetValue(B_CONTROL_OFF);
				t2ksw->SetValue(B_CONTROL_OFF); t2kss->SetValue(B_CONTROL_OFF); t2kse->SetValue(B_CONTROL_OFF);
				break;
			default:
				break;
		}
	}
}

void BeKESAMainWindow::initTab2(BTabView *tv) {
	BTab *tab;
	BBox *box;
	BRect r, sl, sm, sr;

	r = tv->Bounds();
	r.InsetBy(5, 10);
	BView *view = new BView(r, "viewtab2", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	tab = new BTab(view);
	tv->AddTab(view, tab);
	tab->SetLabel("Położenie");

	r = view->Bounds();
	r.InsetBy(10, 20);
	r.bottom = r.bottom - 50;

	box = new BBox(r, "box2x1");
	box->SetLabel("W obrębie jednostek fizjograficznych");
	view->AddChild(box);

	r = box->Bounds();
	r.top = 20; r.bottom = 40; r.left = 10; r.right = 200;
	t2nadmorska = new BCheckBox(r, "t2nadmorska", "Strefa nadmorska", new BMessage(TC2));

	sl = r; sl.right = 130; sl.OffsetBy(0, 20);
	sm = sl; sm.OffsetBy(130, 0);
	sr = sm; sr.OffsetBy(130, 0);
	t2nm = new BCheckBox(sl, "t2nm", "w morzu", new BMessage(TC2));
	t2np = new BCheckBox(sm, "t2np", "plaża", new BMessage(TC2));
	t2ni = new BCheckBox(sr, "t2ni", "mierzeja", new BMessage(TC2));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20);
	t2ns = new BCheckBox(sl, "t2ns", "skarpa", new BMessage(TC2));
	t2nw = new BCheckBox(sm, "t2nw", "wał wydmowy", new BMessage(TC2));

	r.OffsetBy(0, 75);
	t2duzedoliny = new BCheckBox(r, "t2duzedoliny", "Duże doliny", new BMessage(TC2));
	sl = r; sl.right = 130; sl.OffsetBy(0, 20);
	sm = sl; sm.OffsetBy(130, 0);
	sr = sm; sr.OffsetBy(130, 0);
	t2dw = new BCheckBox(sl, "t2dw", "w wodzie", new BMessage(TC2));
	t2dd = new BCheckBox(sm, "t2dd", "terasa denna", new BMessage(TC2));
	t2dz = new BCheckBox(sr, "t2dz", "ter. nadzalewowa", new BMessage(TC2));
	sl.OffsetBy(0, 20); sm.OffsetBy(0, 20);
	t2dy = new BCheckBox(sl, "t2dy", "terasy wyższe", new BMessage(TC2));
	t2db = new BCheckBox(sm, "t2db", "brzeg wysoczyzny", new BMessage(TC2));

	r.OffsetBy(0, 75);
	t2maledoliny = new BCheckBox(r, "t2maledoliny", "Małe doliny", new BMessage(TC2));
	sl = r; sl.right = 130; sl.OffsetBy(0, 20);
	sm = sl; sm.OffsetBy(130, 0);
	sr = sm; sr.OffsetBy(130, 0);
	t2md = new BCheckBox(sl, "t2md", "dno doliny", new BMessage(TC2));
	t2ms = new BCheckBox(sm, "t2ms", "stok doliny", new BMessage(TC2));
	t2mk = new BCheckBox(sr, "t2mk", "krawędź doliny", new BMessage(TC2));

	r.OffsetBy(0, 55);
	t2pozadoliny = new BCheckBox(r, "t2pozadoliny", "Strefa poza dolinami", new BMessage(TC2));
	sl = r; sl.right = 130; sl.OffsetBy(0, 20);
	sm = sl; sm.OffsetBy(130, 0);
	sr = sm; sr.OffsetBy(130, 0);
	t2pr = new BCheckBox(sl, "t2pr", "równina", new BMessage(TC2));
	t2pf = new BCheckBox(sm, "t2pf", "obszar falisty", new BMessage(TC2));
	t2pp = new BCheckBox(sr, "t2pp", "obszar pagórkowaty", new BMessage(TC2));
	sl.OffsetBy(0, 20);
	t2pg = new BCheckBox(sl, "t2pg", "obszar górzysty", new BMessage(TC2));

	box->AddChild(t2nadmorska);
	box->AddChild(t2nm); box->AddChild(t2np); box->AddChild(t2ni);
	box->AddChild(t2ns); box->AddChild(t2nw);
	box->AddChild(t2duzedoliny);
	box->AddChild(t2dw); box->AddChild(t2dd); box->AddChild(t2dz);
	box->AddChild(t2dy); box->AddChild(t2db);
	box->AddChild(t2maledoliny);
	box->AddChild(t2md); box->AddChild(t2ms); box->AddChild(t2mk);
	box->AddChild(t2pozadoliny);
	box->AddChild(t2pr); box->AddChild(t2pf); box->AddChild(t2pp);
	box->AddChild(t2pg);

	updateTab2();
}

void BeKESAMainWindow::updateTab2(BMessage *msg = NULL) {
	bool state;
	state = (t2nadmorska->Value() == B_CONTROL_ON);
	t2nm->SetEnabled(state); t2np->SetEnabled(state); t2ni->SetEnabled(state);
	t2ns->SetEnabled(state); t2nw->SetEnabled(state);
	state = (t2duzedoliny->Value() == B_CONTROL_ON);
	t2dw->SetEnabled(state); t2dd->SetEnabled(state); t2dz->SetEnabled(state);
	t2dy->SetEnabled(state); t2db->SetEnabled(state);
	state = (t2maledoliny->Value() == B_CONTROL_ON);
	t2md->SetEnabled(state); t2ms->SetEnabled(state); t2mk->SetEnabled(state);
	state = (t2pozadoliny->Value() == B_CONTROL_ON);
	t2pr->SetEnabled(state); t2pf->SetEnabled(state); t2pp->SetEnabled(state);
	t2pg->SetEnabled(state);
}

void BeKESAMainWindow::curdata2Tab2(void) {
	int t;
	t = curdata->t2nadmorska;
	t2nadmorska->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2nm->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2np->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ni->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ns->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2nw->SetValue((t & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t2duzedoliny;
	t2duzedoliny->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2dw->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2dd->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2dz->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2dy->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2db->SetValue((t & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t2maledoliny;
	t2maledoliny->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2md->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ms->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2mk->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t2pozadolinami;
	t2pozadoliny->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2pr->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2pf->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2pp->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2pg->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	updateTab2();
}

void BeKESAMainWindow::curdataFromTab2(void) {
	int t;
	t = 0;
	if (t2nadmorska->Value() == B_CONTROL_ON) {
		t = 1;
		if (t2nm->Value() == B_CONTROL_ON) t |= 0x0002;
		if (t2np->Value() == B_CONTROL_ON) t |= 0x0004;
		if (t2ni->Value() == B_CONTROL_ON) t |= 0x0008;
		if (t2ns->Value() == B_CONTROL_ON) t |= 0x0010;
		if (t2nw->Value() == B_CONTROL_ON) t |= 0x0020;
	}
	curdata->t2nadmorska = t;
	t = 0;
	if (t2duzedoliny->Value() == B_CONTROL_ON) {
		t = 1;
		if (t2dw->Value() == B_CONTROL_ON) t |= 0x0002;
		if (t2dd->Value() == B_CONTROL_ON) t |= 0x0004;
		if (t2dz->Value() == B_CONTROL_ON) t |= 0x0008;
		if (t2dy->Value() == B_CONTROL_ON) t |= 0x0010;
		if (t2db->Value() == B_CONTROL_ON) t |= 0x0020;		
	}
	curdata->t2duzedoliny = t;
	t = 0;
	if (t2maledoliny->Value() == B_CONTROL_ON) {
		t = 1;
		if (t2md->Value() == B_CONTROL_ON) t |= 0x0002;
		if (t2ms->Value() == B_CONTROL_ON) t |= 0x0004;
		if (t2mk->Value() == B_CONTROL_ON) t |= 0x0008;
	}
	curdata->t2maledoliny = t;
	t = 0;
	if (t2pozadoliny->Value() == B_CONTROL_ON) {
		t = 1;
		if (t2pr->Value() == B_CONTROL_ON) t |= 0x0002;
		if (t2pf->Value() == B_CONTROL_ON) t |= 0x0004;
		if (t2pp->Value() == B_CONTROL_ON) t |= 0x0008;
		if (t2pg->Value() == B_CONTROL_ON) t |= 0x0010;
	}
	curdata->t2pozadolinami = t;
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

	box->AddChild(t1miejsc); box->AddChild(t1nazwalokalna);
	box->AddChild(t1gmina); box->AddChild(t1powiat);
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

	box->AddChild(t1nrobszaru); box->AddChild(t1nrinwent);
	box->AddChild(t1stanmiejsc); box->AddChild(t1stanobszar);
	box->AddChild(box2);

	r.top += 115; r.bottom += 115;
	box = new BBox(r, "box1x3");
	box->SetLabel("Źródła informacji");
	view->AddChild(box);
	// text controls for box3
	sl = box->Bounds();
	sl.right = sl.left+sl.Width()/3; sl.top = 10; sl.left = 10;
	sl.bottom = sl.top + 20; sl.right -= 10;
	sr = sl; sr.OffsetBy(sl.Width(), 0);
	s = sl; s.OffsetBy(sl.Width()*2, 30);

	t1cz = new BCheckBox(sl, "t1cz", "z wywiadu", new BMessage(TC1));
	t1cw = new BCheckBox(sr, "t1cw", "wykopaliskowe", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1ct = new BCheckBox(sl, "t1ct", "terenowe", new BMessage(TC1));
	t1ca = new BCheckBox(sr, "t1ca", "archiwalne", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30);
	t1cl = new BCheckBox(sl, "t1cl", "luźne", new BMessage(TC1));
	t1cp = new BCheckBox(sr, "t1cp", "powierzchniowe", new BMessage(TC1));
	t1cr = new BCheckBox(s, "t1cr", "rekonstrukcja lokaliz.", new BMessage(TC1));

	box->AddChild(t1cz); box->AddChild(t1ct);
	box->AddChild(t1cl); box->AddChild(t1cw);
	box->AddChild(t1ca); box->AddChild(t1cp);
	box->AddChild(t1cr);

	updateTab1();
}

void BeKESAMainWindow::updateTab1(BMessage *msg = NULL) {

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
	if (t1cz->Value() == B_CONTROL_ON) t1zrodlo |= 0x0001;
	if (t1ct->Value() == B_CONTROL_ON) t1zrodlo |= 0x0002;
	if (t1cl->Value() == B_CONTROL_ON) t1zrodlo |= 0x0004;
	if (t1cw->Value() == B_CONTROL_ON) t1zrodlo |= 0x0008;
	if (t1ca->Value() == B_CONTROL_ON) t1zrodlo |= 0x0010;
	if (t1cp->Value() == B_CONTROL_ON) t1zrodlo |= 0x0020;
	if (t1cr->Value() == B_CONTROL_ON) t1zrodlo |= 0x0040;
	curdata->t1zrodloinformacji = t1zrodlo;
	updateTab1();
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
		sql += " WHERE id = %i";
	} else {	// INSERT
		int newid = GenerateId();
		curdata->id = newid;
		sql = "INSERT INTO karta (";
		sql += "t1miejscowosc, t1nazwalokalna, t1gmina, t1powiat, t1wojewodztwo, t1nrobszaru, t1nrinwentarza, t1x, t1y, t1nrstanmiejsc, t1nrstanobszar, t1zrodloinformacji"; // t1
		sql += ", t2nadmorska, t2duzedoliny, t2maledoliny, t2pozadolinami";
		sql += ", id ) VALUES ( ";
		sql += "%Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %Q, %i,";	// t1
		sql += "%i, %i, %i, %i,"; // t2
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
