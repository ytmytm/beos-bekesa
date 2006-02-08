
#include "mainwindow.h"

#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <RadioButton.h>
#include <StringView.h>
#include <TabView.h>
#include <TextControl.h>

const uint32 TC3			= 'TC03';
const uint32 TC3W			= 'TC3W';
const uint32 TC3S			= 'TC3S';
const uint32 TC3dookolna	= 'TC3D';
const uint32 TC3wyczysc		= 'TC3C';

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
	updateTab3();
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
	bool state;
	state = (t2te->Value() == B_CONTROL_ON);
	t2ek->SetEnabled(state); t2es->SetEnabled(state); t2ec->SetEnabled(state);
	t2eg->SetEnabled(state); t2ew->SetEnabled(state);
	state = (t2to->Value() == B_CONTROL_ON);
	t2op->SetEnabled(state); t2od->SetEnabled(state); t2ok->SetEnabled(state);
	t2oj->SetEnabled(state);
}

void BeKESAMainWindow::curdata2Tab3(void) {
	int i, t;
	i = curdata->t2ekswys;
	if ((i<0) || (i>7)) i=0;
	t2wysitems[i]->SetMarked(true);
	i = curdata->t2eksstop;
	if ((i<0) || (i>6)) i=0;
	t2stopitems[i]->SetMarked(true);
	t = curdata->t2ekskier;
	t2knw->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2knn->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2kne->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2kww->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2kee->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ksw->SetValue((t & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2kss->SetValue((t & 0x0040) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2kse->SetValue((t & 0x0080) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t2ekspozycja;
	t2te->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2to->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2tn->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2tb->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t2ekspozycja2;
	t2ek->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2es->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ec->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2eg->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ew->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2op->SetValue((t & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2od->SetValue((t & 0x0040) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2ok->SetValue((t & 0x0080) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2oj->SetValue((t & 0x0100) ? B_CONTROL_ON : B_CONTROL_OFF);
	t2forma->SetText(curdata->t2forma.String());
	updateTab3();
}

void BeKESAMainWindow::curdataFromTab3(void) {
	// t2ekswys and t2eksstop set in update from msg
	int t, u;
	t = 0;
	if (t2knw->Value() == B_CONTROL_ON) t |= 0x0001;
	if (t2knn->Value() == B_CONTROL_ON) t |= 0x0002;
	if (t2kne->Value() == B_CONTROL_ON) t |= 0x0004;
	if (t2kww->Value() == B_CONTROL_ON) t |= 0x0008;
	if (t2kee->Value() == B_CONTROL_ON) t |= 0x0010;
	if (t2ksw->Value() == B_CONTROL_ON) t |= 0x0020;
	if (t2kss->Value() == B_CONTROL_ON) t |= 0x0040;
	if (t2kse->Value() == B_CONTROL_ON) t |= 0x0080;
	curdata->t2ekskier = t;
	t = 0; u = 0;
	if (t2te->Value() == B_CONTROL_ON) {
		t |= 0x0001;
		if (t2ek->Value() == B_CONTROL_ON) u |= 0x0001;
		if (t2es->Value() == B_CONTROL_ON) u |= 0x0002;
		if (t2ec->Value() == B_CONTROL_ON) u |= 0x0004;
		if (t2eg->Value() == B_CONTROL_ON) u |= 0x0008;
		if (t2ew->Value() == B_CONTROL_ON) u |= 0x0010;
	}
	if (t2to->Value() == B_CONTROL_ON) {
		t |= 0x0002;
		if (t2op->Value() == B_CONTROL_ON) u |= 0x0020;
		if (t2od->Value() == B_CONTROL_ON) u |= 0x0040;
		if (t2ok->Value() == B_CONTROL_ON) u |= 0x0080;
		if (t2oj->Value() == B_CONTROL_ON) u |= 0x0100;
	}
	if (t2tn->Value() == B_CONTROL_ON) t |= 0x0004;
	if (t2tb->Value() == B_CONTROL_ON) t |= 0x0008;
	curdata->t2ekspozycja = t;
	curdata->t2ekspozycja2 = u;
	curdata->t2forma = t2forma->Text();
}
