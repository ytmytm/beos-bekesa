
#include "mainwindow.h"
#include <Box.h>
#include <CheckBox.h>
#include <TabView.h>
#include <TextControl.h>

const uint32 TC1			= 'TC01';

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
	s = sl; s.OffsetBy(sl.Width()*2, 0);

	t1ca = new BCheckBox(sl, "t1ca", "archiwalne", new BMessage(TC1));
	t1cp = new BCheckBox(sr, "t1cp", "powierzchniowe", new BMessage(TC1));
	t1cz = new BCheckBox(s,  "t1cz", "zasłyszane", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30); s.OffsetBy(0,30);
	t1ct = new BCheckBox(sl, "t1ct", "terenowe", new BMessage(TC1));
	t1cl = new BCheckBox(sr, "t1cl", "luźne", new BMessage(TC1));
	t1cs = new BCheckBox(s,  "t1cs", "samolot", new BMessage(TC1));
	sl.OffsetBy(0,30); sr.OffsetBy(0,30); s.OffsetBy(0,30);
	t1cw = new BCheckBox(sl, "t1cw", "wykopaliskowe", new BMessage(TC1));
	t1cr = new BCheckBox(sr, "t1cr", "rekonstrukcja lokaliz.", new BMessage(TC1));

	box->AddChild(t1ca); box->AddChild(t1ct);
	box->AddChild(t1cw); box->AddChild(t1cp);
	box->AddChild(t1cl); box->AddChild(t1cr);
	box->AddChild(t1cz); box->AddChild(t1cs);

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
	t1ca->SetValue((t1 & 0x0001) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1ct->SetValue((t1 & 0x0002) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cw->SetValue((t1 & 0x0004) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cp->SetValue((t1 & 0x0008) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cl->SetValue((t1 & 0x0010) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cr->SetValue((t1 & 0x0020) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cz->SetValue((t1 & 0x0040) ? B_CONTROL_ON : B_CONTROL_OFF);
	t1cs->SetValue((t1 & 0x0080) ? B_CONTROL_ON : B_CONTROL_OFF);
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
	if (t1ca->Value() == B_CONTROL_ON) t1zrodlo |= 0x0001;
	if (t1ct->Value() == B_CONTROL_ON) t1zrodlo |= 0x0002;
	if (t1cw->Value() == B_CONTROL_ON) t1zrodlo |= 0x0004;
	if (t1cp->Value() == B_CONTROL_ON) t1zrodlo |= 0x0008;
	if (t1cl->Value() == B_CONTROL_ON) t1zrodlo |= 0x0010;
	if (t1cr->Value() == B_CONTROL_ON) t1zrodlo |= 0x0020;
	if (t1cz->Value() == B_CONTROL_ON) t1zrodlo |= 0x0040;
	if (t1cs->Value() == B_CONTROL_ON) t1zrodlo |= 0x0080;
	curdata->t1zrodloinformacji = t1zrodlo;
	updateTab1();
}
