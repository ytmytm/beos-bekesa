
#include "mainwindow.h"

#include <Box.h>
#include <CheckBox.h>
#include <TabView.h>

const uint32 TC2			= 'TC02';

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
