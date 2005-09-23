
//
// start: open db, ask for dbfile if not found
// fetch list from db, populate left list
// select none, act as if new and untouched
// 
// click on 'new' -> store/abort current, clear new struct, clear widgets
// click on 'save' -> update/store current, update left list
//					  refetch from list
//
// click on list -> store/abort current, fetch data, fill widgets with data
//
// track if data from current is new or edited from list
// currentid < 0 -> INSERT, currentid > 0 -> UPDATE

#include "mainwindow.h"
#include <View.h>
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
//#include <Path.h>
//#include <Clipboard.h>
//#include <Alert.h>
#include <stdio.h>

const uint32 MENU_DEFMSG 	= 'M000';
const uint32 BUT_NEW		= 'Bnew';
const uint32 BUT_SAVE		= 'Bsav';
const uint32 TC1			= 'TC01';

BeKESAMainWindow::BeKESAMainWindow(const char *windowTitle) : BWindow(
	BRect(100, 100, 740, 580), windowTitle, B_DOCUMENT_WINDOW, B_OUTLINE_RESIZE, B_CURRENT_WORKSPACE ) {

	this->Hide();
//	config = new bydpConfig();

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

	menu = new BMenu("File", B_ITEMS_IN_COLUMN);
	((SpLocaleApp*)be_app)->AddToFileMenu(menu,false,false,false);
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Item", new BMessage(MENU_DEFMSG), 0, 0));
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	menuBar->AddItem(menu);

	// listview in scrollview
	r = mainView->Bounds();
	r.right = 150-20; r.top = 20;
	listView = new BListView(r, "listView");
	listView->AddItem(new BStringItem("dummy"));
	mainView->AddChild(new BScrollView("scrollView", listView, B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true));

	// tabview
	r = mainView->Bounds();
	r.left = 150; r.top = 20; r.bottom = r.bottom - 50;
	tabView = new BTabView(r, "tabView");
	mainView->AddChild(tabView);

	// each table in tabview in own function...
	initTab1(tabView);

	// buttons
	but_new = new BButton(BRect(170,440,230,470), "but_new", "New", new BMessage(BUT_NEW), B_FOLLOW_LEFT|B_FOLLOW_BOTTOM);
	but_save = new BButton(BRect(570,440,630,470), "but_save", "Save", new BMessage(BUT_SAVE), B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM);
	mainView->AddChild(but_new);
	mainView->AddChild(but_save);

	tabView->Select(0);
	this->Show();

	// get memory for objects
	curdata = new kesadat();
	newdata = new kesadat();
}

BeKESAMainWindow::~BeKESAMainWindow() {
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

void BeKESAMainWindow::valiDataTab1(void) {
	printf("in validata1\n");
	curdata->dirty = true;
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
	this->DisableUpdates();
	switch (Message->what) {
		case TC1:
			// update internal state of tab1 or mark it dirty
			valiDataTab1();
			curdata->dump_all();
			break;
		default:
			BWindow::MessageReceived(Message);
			break;
	}
	this->EnableUpdates();
}

bool BeKESAMainWindow::QuitRequested() {
//	config->position = this->Frame();
//	config->save();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

//--------------------

void kesadat::dump_all( void ) {
	printf("------------\n");
	printf("miejsc:%s, nazwa: %s, gmina %s\npowiat: %s, wojewodz: %s\n",
		t1miejsc.String(), t1nazwalokalna.String(), t1gmina.String(), t1powiat.String(), t1wojewodztwo.String());
	printf("nrobsz: %s, nrinwent: %s, x: %s, y: %s, stan/msc: %s, stan/obsz: %s\n",
		t1nrobszaru.String(), t1nrinwent.String(), t1x.String(), t1y.String(), t1stanmiejsc.String(), t1stanobszar.String());
	printf("zrodlo: %x\n", t1zrodloinformacji);
	printf("\n");
}