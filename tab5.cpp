
#include "mainwindow.h"
#include <Box.h>
#include <CheckBox.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <TabView.h>
#include <TextControl.h>

const uint32 TC6			= 'TC06';
const uint32 TC6O			= 'TC6O';
const uint32 TC6P			= 'TC6P';
const uint32 TC6N			= 'TC6N';
const uint32 TC6C			= 'TC6C';
const uint32 TC6W			= 'TC6W';
const uint32 TC6G			= 'TC6G';
const uint32 TC6Z			= 'TC6Z';
const uint32 TC6S			= 'TC6S';

void BeKESAMainWindow::initTab5(BTabView *tv) {
	BTab *tab;
	BBox *box;
	BRect r,sl,sr;

	r = tv->Bounds();
	r.InsetBy(5,10);
	BView *view = new BView(r, "viewtab5", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	tab = new BTab(view);
	tv->AddTab(view, tab);
	tab->SetLabel("Charakt./Zagr.");

	r = view->Bounds();
	r.top = 10; r.left = 10; r.right -= 10;
	r.bottom = r.top + 140;
	box = new BBox(r, "box5x1");
	box->SetLabel("Charakterystyka");
	view->AddChild(box);

	sl = box->Bounds();
	sl.InsetBy(10,20);
	sl.right = sl.left + sl.Width()/2;
	sl.bottom = sl.top + 20;
	sr = sl; sr.OffsetBy(sl.Width(),0);

	BPopUpMenu *menu;
	BMenuItem *item;
	BMessage *msg;
	menu = new BPopUpMenu("t6mobserwacja");
	msg = new BMessage(TC6O); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6obsitems[0] = item;
	msg = new BMessage(TC6O); msg->AddInt32("_item", 1);
	item = new BMenuItem("utrudniona", msg); menu->AddItem(item);
	t6obsitems[1] = item;
	msg = new BMessage(TC6O); msg->AddInt32("_item", 2);
	item = new BMenuItem("bez przeszkód", msg); menu->AddItem(item);
	t6obsitems[2] = item;
	BMenuField *t6mobserwacja = new BMenuField(sl, "t6mfobserwacja", "Obserwacja", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mobserwacja->SetDivider(100);
	box->AddChild(t6mobserwacja);

	menu = new BPopUpMenu("t6mpole");
	msg = new BMessage(TC6P); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6politems[0] = item;
	msg = new BMessage(TC6P); msg->AddInt32("_item", 1);
	item = new BMenuItem("otwarte", msg); menu->AddItem(item);
	t6politems[1] = item;
	msg = new BMessage(TC6P); msg->AddInt32("_item", 2);
	item = new BMenuItem("zamkniete", msg); menu->AddItem(item);
	t6politems[2] = item;
	BMenuField *t6mpole = new BMenuField(sr, "t6mfpole", "Pole", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mpole->SetDivider(100);
	box->AddChild(t6mpole);

	sl.OffsetBy(0,35); sr.OffsetBy(0,35);

	menu = new BPopUpMenu("t6mnasycenie");
	msg = new BMessage(TC6N); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6nasitems[0] = item;
	msg = new BMessage(TC6N); msg->AddInt32("_item", 1);
	item = new BMenuItem("równomierne", msg); menu->AddItem(item);
	t6nasitems[1] = item;
	msg = new BMessage(TC6N); msg->AddInt32("_item", 2);
	item = new BMenuItem("nierównomierne", msg); menu->AddItem(item);
	t6nasitems[2] = item;
	BMenuField *t6mnas = new BMenuField(sl, "t6mfnas", "Nasycenie", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mnas->SetDivider(100);
	box->AddChild(t6mnas);

	menu = new BPopUpMenu("t6mkonc");
	msg = new BMessage(TC6C); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6konitems[0] = item;
	msg = new BMessage(TC6C); msg->AddInt32("_item", 1);
	item = new BMenuItem("jednocentryczne", msg); menu->AddItem(item);
	t6konitems[1] = item;
	msg = new BMessage(TC6C); msg->AddInt32("_item", 2);
	item = new BMenuItem("wielocentryczne", msg); menu->AddItem(item);
	t6konitems[2] = item;
	msg = new BMessage(TC6C); msg->AddInt32("_item", 3);
	item = new BMenuItem("dekoncentryczne", msg); menu->AddItem(item);
	t6konitems[3] = item;
	BMenuField *t6mkon = new BMenuField(sr, "t6mfkon", "Nasycenie", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mkon->SetDivider(100);
	box->AddChild(t6mkon);

	sl.OffsetBy(0,35); sr.OffsetBy(0,35);

	menu = new BPopUpMenu("t6mpow");
	msg = new BMessage(TC6W); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6powitems[0] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 1);
	item = new BMenuItem("0 ha", msg); menu->AddItem(item);
	t6powitems[1] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 2);
	item = new BMenuItem("< 0,01 ha", msg); menu->AddItem(item);
	t6powitems[2] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 3);
	item = new BMenuItem("< 0,5 ha", msg); menu->AddItem(item);
	t6powitems[3] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 4);
	item = new BMenuItem("< 1 ha", msg); menu->AddItem(item);
	t6powitems[4] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 5);
	item = new BMenuItem("< 5 ha", msg); menu->AddItem(item);
	t6powitems[5] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 6);
	item = new BMenuItem("< 15 ha", msg); menu->AddItem(item);
	t6powitems[6] = item;
	msg = new BMessage(TC6W); msg->AddInt32("_item", 7);
	item = new BMenuItem("> 15 ha", msg); menu->AddItem(item);
	t6powitems[7] = item;
	BMenuField *t6mpow = new BMenuField(sl, "t6mfpow", "Powierzchnia", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mpow->SetDivider(100);
	box->AddChild(t6mpow);

	menu = new BPopUpMenu("t6mgestosc");
	msg = new BMessage(TC6G); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t6gesitems[0] = item;
	msg = new BMessage(TC6G); msg->AddInt32("_item", 1);
	item = new BMenuItem("mała", msg); menu->AddItem(item);
	t6gesitems[1] = item;
	msg = new BMessage(TC6G); msg->AddInt32("_item", 2);
	item = new BMenuItem("średnia", msg); menu->AddItem(item);
	t6gesitems[2] = item;
	msg = new BMessage(TC6G); msg->AddInt32("_item", 3);
	item = new BMenuItem("duża", msg); menu->AddItem(item);
	t6gesitems[3] = item;
	BMenuField *t6mges = new BMenuField(sr, "t6mfges", "Gęstość występ.", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t6mges->SetDivider(100);
	box->AddChild(t6mges);

	r.top += 145; r.bottom += 145;
	box = new BBox(r, "box5x2");
	box->SetLabel("Zagrożenia");
	view->AddChild(box);

	sl = box->Bounds();
	sl.InsetBy(10,20);
	sl.right = sl.left + sl.Width()/2;
	sl.bottom = sl.top + 20;
	sr = sl; sr.OffsetBy(sl.Width(),0);

	menu = new BPopUpMenu("t7mzagrozenie");
	msg = new BMessage(TC6Z); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t7zagitems[0] = item;
	msg = new BMessage(TC6Z); msg->AddInt32("_item", 1);
	item = new BMenuItem("istnieje", msg); menu->AddItem(item);
	t7zagitems[1] = item;
	msg = new BMessage(TC6Z); msg->AddInt32("_item", 2);
	item = new BMenuItem("nie istnieje", msg); menu->AddItem(item);
	t7zagitems[2] = item;
	BMenuField *t7mzag = new BMenuField(sl, "t6mfzag", "Zagrożenie", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t7mzag->SetDivider(100);
	box->AddChild(t7mzag);

	menu = new BPopUpMenu("t7mstale");
	msg = new BMessage(TC6S); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t7staitems[0] = item;
	msg = new BMessage(TC6S); msg->AddInt32("_item", 1);
	item = new BMenuItem("stałe", msg); menu->AddItem(item);
	t7staitems[1] = item;
	msg = new BMessage(TC6S); msg->AddInt32("_item", 2);
	item = new BMenuItem("doraźne", msg); menu->AddItem(item);
	t7staitems[2] = item;
	BMenuField *t7msta = new BMenuField(sr, "t6mfsta", "Rodzaj", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t7msta->SetDivider(100);
	box->AddChild(t7msta);

	sl.OffsetBy(0,35); sr.OffsetBy(0,35);

	t7zl = new BCheckBox(sl, "t7zl", "przez ludzi", new BMessage(TC6));
	t7zp = new BCheckBox(sr, "t7zp", "użytkownik prywatny", new BMessage(TC6));
	sl.OffsetBy(0,25); sr.OffsetBy(0,25);
	t7zn = new BCheckBox(sl, "t7zn", "przez naturę", new BMessage(TC6));
	t7zs = new BCheckBox(sr, "t7zs", "użytkownik społeczny", new BMessage(TC6));

	box->AddChild(t7zl); box->AddChild(t7zn);
	box->AddChild(t7zp); box->AddChild(t7zs);

	sl.OffsetBy(0,35);
	sl.right = sr.right;

	t7tdodatkowe = new BTextControl(sl, "t7tdodatkowe", "Dod. dane", NULL, new BMessage(TC6));
	t7tdodatkowe->SetDivider(100);
	box->AddChild(t7tdodatkowe);

	updateTab5();
}

void BeKESAMainWindow::updateTab5(BMessage *msg = NULL) {
	int32 item = 0;
	if (msg) {
		switch (msg->what) {
			case TC6O:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6obserwacja = item;
				break;
			case TC6P:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6pole = item;
				break;
			case TC6N:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6nasycenie = item;
				break;
			case TC6C:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6koncen = item;
				break;
			case TC6W:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6pow = item;
				break;
			case TC6G:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t6gestosc = item;
				break;
			case TC6Z:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t7zagrozenie = item;
				break;
			case TC6S:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t7stale = item;
				break;			
			default:
				break;
		}
	}
}

void BeKESAMainWindow::curdata2Tab5(void) {
	int i, t;
	i = curdata->t6obserwacja;
	if ((i<0) || (i>2)) i=0;
	t6obsitems[i]->SetMarked(true);
	i = curdata->t6pole;
	if ((i<0) || (i>2)) i=0;
	t6politems[i]->SetMarked(true);
	i = curdata->t6nasycenie;
	if ((i<0) || (i>2)) i=0;
	t6nasitems[i]->SetMarked(true);
	i = curdata->t6koncen;
	if ((i<0) || (i>3)) i=0;
	t6konitems[i]->SetMarked(true);
	i = curdata->t6pow;
	if ((i<0) || (i>7)) i=0;
	t6powitems[i]->SetMarked(true);
	i = curdata->t6gestosc;
	if ((i<0) || (i>3)) i=0;
	t6gesitems[i]->SetMarked(true);
	i = curdata->t7zagrozenie;
	if ((i<0) || (i>2)) i=0;
	t7zagitems[i]->SetMarked(true);
	i = curdata->t7stale;
	if ((i<0) || (i>2)) i=0;
	t7staitems[i]->SetMarked(true);
	t = curdata->t7przezco;
	t7zl->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t7zn->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t7zp->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t7zs->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t7tdodatkowe->SetText(curdata->t7dodatkowe.String());
}

void BeKESAMainWindow::curdataFromTab5(void) {
	// menu items set in update from msg
	int t;
	t = 0;
	if (t7zl->Value() == B_CONTROL_ON) t |= 0x0001;
	if (t7zn->Value() == B_CONTROL_ON) t |= 0x0002;
	if (t7zp->Value() == B_CONTROL_ON) t |= 0x0004;
	if (t7zs->Value() == B_CONTROL_ON) t |= 0x0008;
	curdata->t7przezco = t;
	curdata->t7dodatkowe = t7tdodatkowe->Text();
}
