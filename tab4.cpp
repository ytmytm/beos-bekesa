
#include "mainwindow.h"
#include <Box.h>
#include <CheckBox.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <TabView.h>
#include <TextControl.h>

const uint32 TC4			= 'TC04';
const uint32 TC4Z			= 'TC4Z';
const uint32 TC4K			= 'TC4K';

void BeKESAMainWindow::initTab4(BTabView *tv) {
	BTab *tab;
	BBox *box;
	BRect r,s,sl,sr;

	r = tv->Bounds();
	r.InsetBy(5,10);
	BView *view = new BView(r, "viewtab4", B_FOLLOW_ALL_SIDES, 0);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	tab = new BTab(view);
	tv->AddTab(view, tab);
	tab->SetLabel("Teren/Gleba");

	r = view->Bounds();
	r.top = 10; r.left = 10; r.right -= 10;
	r.bottom = r.top + 140;
	box = new BBox(r, "box4x1");
	box->SetLabel("Dostępność terenu");
	view->AddChild(box);

	sl = box->Bounds();
	sl.InsetBy(10,20);
	sl.right = sl.left + sl.Width()/2;
	sl.bottom = sl.top + 20;

	BPopUpMenu *menu;
	BMenuItem *item;
	BMessage *msg;
	menu = new BPopUpMenu("t3mzabudowa");
	msg = new BMessage(TC4Z); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t3zabitems[0] = item;
	msg = new BMessage(TC4Z); msg->AddInt32("_item", 1);
	item = new BMenuItem("niezabudowany", msg); menu->AddItem(item);
	t3zabitems[1] = item;
	msg = new BMessage(TC4Z); msg->AddInt32("_item", 2);
	item = new BMenuItem("średniozabudowany", msg); menu->AddItem(item);
	t3zabitems[2] = item;
	msg = new BMessage(TC4Z); msg->AddInt32("_item", 3);
	item = new BMenuItem("zabudowany", msg); menu->AddItem(item);
	t3zabitems[3] = item;
	BMenuField *t3zabudowa = new BMenuField(sl, "t3zabudowa", "Dostępność", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t3zabudowa->SetDivider(100);
	box->AddChild(t3zabudowa);

	s = box->Bounds();
	s.InsetBy(10,20);
	s.top = s.top + sl.Height() + 5;
	s.right = s.left + s.Width()/3;
	s.bottom = s.top + 20;
	sl = s;
	s.OffsetBy(s.Width(), 0);
	sr = s;
	sr.OffsetBy(s.Width(), 0);
	t3tl = new BCheckBox(sl, "t3tl", "las", new BMessage(TC4));
	t3ts = new BCheckBox(s,  "t3ts", "sad", new BMessage(TC4));
	t3tp = new BCheckBox(sr, "t3tp", "park", new BMessage(TC4));
	sl.OffsetBy(0,20); s.OffsetBy(0,20); sr.OffsetBy(0,20);
	t3to = new BCheckBox(sl, "t3to", "pole orne", new BMessage(TC4));
	t3ta = new BCheckBox(s,  "t3ta", "łąka", new BMessage(TC4));
	t3tr = new BCheckBox(sr, "t3tr", "ter. roln. prywatny", new BMessage(TC4));
	sl.OffsetBy(0,20); s.OffsetBy(0,20); sr.OffsetBy(0,20);
	t3te = new BCheckBox(sl, "t3te", "ter. roln. społeczny", new BMessage(TC4));
	t3tz = new BCheckBox(s,  "t3tz", "teren przemysłowy", new BMessage(TC4));

	box->AddChild(t3tl); box->AddChild(t3ts); box->AddChild(t3tp);
	box->AddChild(t3to); box->AddChild(t3ta); box->AddChild(t3tr);
	box->AddChild(t3te); box->AddChild(t3tz);

	s = box->Bounds();
	s.InsetBy(10,20);
	sl.right = sl.left + s.Width();
	sl.OffsetBy(0,25);
	t3tokr = new BTextControl(sl, "t3tokr", "Okr. bliższe", NULL, new BMessage(TC4));
	t3tokr->SetDivider(100);
	box->AddChild(t3tokr);

	r.top += 145; r.bottom += 100;
	box = new BBox(r, "box4x2");
	box->SetLabel("Gleba");
	view->AddChild(box);

	sl = box->Bounds();
	sl.InsetBy(10,20);
	sl.right = sl.left + sl.Width()/3;
	sl.bottom = sl.top + 20;
	s = sl; s.OffsetBy(sl.Width(),0);
	sr = s; sr.OffsetBy(sl.Width(),0);
	sl.OffsetBy(0,10); sr.OffsetBy(0,10);

	menu = new BPopUpMenu("t3mkam");
	msg = new BMessage(TC4K); msg->AddInt32("_item", 0);
	item = new BMenuItem("[brak]", msg); item->SetMarked(true); menu->AddItem(item);
	t3kamitems[0] = item;
	msg = new BMessage(TC4K); msg->AddInt32("_item", 1);
	item = new BMenuItem("mała", msg); menu->AddItem(item);
	t3kamitems[1] = item;
	msg = new BMessage(TC4K); msg->AddInt32("_item", 2);
	item = new BMenuItem("średnia", msg); menu->AddItem(item);
	t3kamitems[2] = item;
	msg = new BMessage(TC4K); msg->AddInt32("_item", 3);
	item = new BMenuItem("duża", msg); menu->AddItem(item);
	t3kamitems[3] = item;
	BMenuField *t3kamienie = new BMenuField(sl, "t3kamienie", "Kamienistość", menu, B_FOLLOW_LEFT, B_WILL_DRAW);
	t3kamienie->SetDivider(70);
	box->AddChild(t3kamienie);

	t3gp = new BCheckBox(s, "t3gp", "piaszczysta", new BMessage(TC4));
	s.OffsetBy(0,20);
	t3gg = new BCheckBox(s, "t3gg", "gliniasta", new BMessage(TC4));
	t3gt = new BCheckBox(sr, "t3gt", "torfowo-bagnista", new BMessage(TC4));

	box->AddChild(t3gp); box->AddChild(t3gg); box->AddChild(t3gt);

	s = box->Bounds();
	s.InsetBy(10,20);
	sl.right = sl.left + s.Width();
	sl.OffsetBy(0,35);
	t3gokr = new BTextControl(sl, "t3gokr", "Okr. specj.", NULL, new BMessage(TC4));
	t3gokr->SetDivider(100);
	box->AddChild(t3gokr);

	updateTab4();
}

void BeKESAMainWindow::updateTab4(BMessage *msg = NULL) {
	int32 item = 0;
	if (msg) {
		switch (msg->what) {
			case TC4Z:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t3zabudowa = item;
				break;
			case TC4K:
				if (msg->FindInt32("_item", &item) == B_OK)
					curdata->t5kamienistosc = item;
			default:
				break;
		}
	}
}

void BeKESAMainWindow::curdata2Tab4(void) {
	int i, t;
	i = curdata->t3zabudowa;
	if ((i<0) || (i>3)) i=0;
	t3zabitems[i]->SetMarked(true);
	i = curdata->t5kamienistosc;
	t3kamitems[i]->SetMarked(true);
	t = curdata->t3rodzaj;
	t3tl->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3ts->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3tp->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3to->SetValue((t & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3ta->SetValue((t & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3tr->SetValue((t & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3te->SetValue((t & 0x0040) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3tz->SetValue((t & 0x0080) ? B_CONTROL_ON : B_CONTROL_OFF);
	t = curdata->t5gleba;
	t3gp->SetValue((t & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3gg->SetValue((t & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3gt->SetValue((t & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t3tokr->SetText(curdata->t3okreslenie.String());
	t3gokr->SetText(curdata->t5okreslenie.String());
}

void BeKESAMainWindow::curdataFromTab4(void) {
	// t3zabudowa and t5kamienistosc set in update from msg
	int t;
	t = 0;
	if (t3tl->Value() == B_CONTROL_ON) t |= 0x0001;
	if (t3ts->Value() == B_CONTROL_ON) t |= 0x0002;
	if (t3tp->Value() == B_CONTROL_ON) t |= 0x0004;
	if (t3to->Value() == B_CONTROL_ON) t |= 0x0008;
	if (t3ta->Value() == B_CONTROL_ON) t |= 0x0010;
	if (t3tr->Value() == B_CONTROL_ON) t |= 0x0020;
	if (t3te->Value() == B_CONTROL_ON) t |= 0x0040;
	if (t3tz->Value() == B_CONTROL_ON) t |= 0x0080;
	curdata->t3rodzaj = t;
	t = 0;
	if (t3gp->Value() == B_CONTROL_ON) t |= 0x0001;
	if (t3gg->Value() == B_CONTROL_ON) t |= 0x0002;
	if (t3gt->Value() == B_CONTROL_ON) t |= 0x0004;
	curdata->t5gleba = t;
	curdata->t3okreslenie = t3tokr->Text();
	curdata->t5okreslenie = t3gokr->Text();
}

