//
// wymuszac w printsettings landscape i format A4
//
// z jakiegos powodu wywala sie na dolaczaniu /this/ do bwindow jezeli
// optymalizacja jest wlaczona - czyzby problem z niemoznoscia skopiowania
// kesadat?
//

#include "mainwindow.h"
#include "bekesaprint.h"

#include <Font.h>
#include <Message.h>
#include <PrintJob.h>
#include <View.h>
#include <Window.h>

#include <stdio.h>

BeKESAPrint::BeKESAPrint(int id, sqlite *db, BMessage *pSettings) :
	BView(BRect(0,0,100,100), "BeKESAPrint", B_FOLLOW_ALL, B_WILL_DRAW|B_SUBPIXEL_PRECISE) {

	if (id<0)
		return;

	dbData = db;
	data = new kesadat();
	data->dbData = db;
	data->fetch(id);	

	status_t result = B_OK;
	printJob = new BPrintJob(data->t1miejsc.String());
	printJob->SetSettings(new BMessage(*pSettings));
// dla przyspieszenia preview, potem wlaczyc!!!
//XXX	result = printJob->ConfigJob();
	// XXX return, ale jak błąd zgłosić? geterror lub sth?
	if (result != B_OK)
		return;
	if ((printJob->LastPage() - printJob->FirstPage() + 1) <= 0)
		return;
}

BeKESAPrint::~BeKESAPrint() {
	if (pWindow->Lock()) {
		RemoveSelf();
		pWindow->Quit();
	}
}

void BeKESAPrint::Go(void) {
	// information from printJob
	BRect pageRect = printJob->PrintableRect();	
//	int32 firstPage = printJob->FirstPage();
//	int32 lastPage = printJob->LastPage();
//	int32 pageCount = printJob->LastPage() - printJob->FirstPage() + 1;
//	printf("rect:[%f,%f,%f,%f]\n",pageRect.left,pageRect.top,pageRect.right,pageRect.bottom);
//	printf("page1:%i,lastp:%i;pages:%i\n",firstPage,lastPage,pageCount);
	// calculate # of own pages (according to rect), clip lastpage against pages in document
	// dla debugu:
	// - przygotowac okno
	// - w oknie view do rysowania, tam w Draw() wszystko wyrysowac
	// - wyswietlic
	BRect r = pageRect;
	r.OffsetBy(20,20);
	pWindow = new BWindow(r, "Podgląd wydruku", B_TITLED_WINDOW, 0);
	pWindow->AddChild(this);
	MoveTo(pageRect.LeftTop());
	ResizeTo(pageRect.Width(),pageRect.Height());
//	pWindow->Show();
//	return;	// XXX removeme!
	printJob->BeginJob();
	// for all pages...
	printJob->DrawView(this,BRect(pageRect),BPoint(0.0,0.0));	// cala strona, od (0,0)
	printJob->SpoolPage();
	printJob->CommitJob();
}

#define TOP		(pageRect.top+font.Size()+10)
#define LEFT	(pageRect.left+10)

void BeKESAPrint::Draw(BRect pageRect) {
printf("indraw\n");
//	printf("(%f,%f)->(%f,%f)\n",pageRect.top,pageRect.left,pageRect.bottom,pageRect.right);

//	BFont font(be_plain_font);
//	font.SetFamilyAndStyle("Arial","Regular");
//	font.SetFlags(B_DISABLE_ANTIALIASING);
//	BFont fontb(be_bold_font);
//	fontb.SetFamilyAndStyle("Arial","Bold");
//	fontb.SetFlags(B_DISABLE_ANTIALIASING);
//	font.SetSize(10.0);
//	fontb.SetSize(10.0);

//	SetFont(&font);
//	MovePenTo(LEFT, TOP);
//	DrawString("hello, world! drugalinia trzecialinia czwartalinia");

	DrawWires(pageRect);
	DrawValues(pageRect);
}

#define MM(x) (2.84*(x))

void BeKESAPrint::DrawWires(BRect r) {
	BFont font(be_plain_font);
	font.SetFamilyAndStyle("Arial","Regular");
	font.SetFlags(B_DISABLE_ANTIALIASING);
	BFont fontb(be_bold_font);
	fontb.SetFamilyAndStyle("Arial","Bold");
	fontb.SetFlags(B_DISABLE_ANTIALIASING);
	font.SetSize(10.0);
	fontb.SetSize(10.0);

	// top,left (x=10mm, y=7mm, xright-17mm, ybot-9mm)
	// 1p = 1"/72 1" - 2.54cm = 25.4mm --> 1p = 25.4mm/72 = .352mm
	//		--> 1mm = 2.84p
	BRect frame(r.left+MM(10),r.top+MM(7),r.right-MM(17),r.bottom-MM(9));
	StrokeRect(frame);
	BRect line1(frame);
	line1.bottom = line1.top + MM(10);
	StrokeLine(line1.LeftBottom(),line1.RightBottom());
	StrokeLine(BPoint(line1.left+MM(28),line1.top),BPoint(line1.left+MM(28),line1.bottom));
	BRect r0(line1.right-MM(70),line1.top,line1.right-MM(70),line1.bottom);
	BPoint t0(r0.left+MM(3.5),r0.top+MM(4));
	for (int i=0;i<8;i++) {
		StrokeLine(r0.LeftTop(),r0.RightBottom());
		r0.OffsetBy(MM(8.75),0);
	}
	char tt1zrodlo[] = "ATWPLRZS"; char tt1z[2];
	tt1z[1]='\0';
	font.SetSize(MM(3.5)); SetFont(&font);
	for (int i=0;i<8;i++) {
		tt1z[0] = tt1zrodlo[i]; MovePenTo(t0); DrawString(tt1z); t0.x += MM(8.75);
	}
	line1.OffsetBy(0,MM(5));
	StrokeLine(BPoint(line1.right-MM(70),line1.top),line1.RightTop());
	MovePenTo(frame.left+MM(7),frame.top+MM(8));
	font.SetSize(MM(7)); SetFont(&font);
	DrawString("ODZ");
	MovePenTo(frame.left+MM(35),frame.top+MM(8));
	fontb.SetSize(MM(5.75)); SetFont(&fontb);
	DrawString("KARTA EWIDENCJI STANOWISKA ARCHEOLOGICZNEGO");
	// box1
	BRect rbox1;
	rbox1.left = frame.left; rbox1.right = frame.left + MM(68);
	rbox1.top = frame.top + MM(10); rbox1.bottom = rbox1.top + MM(65);
	StrokeLine(rbox1.LeftBottom(),rbox1.RightBottom());
	StrokeLine(rbox1.RightTop(),rbox1.RightBottom());
	// box1 lines
	{
	BRect line(rbox1);
	line.top = line.top + MM(5); line.bottom = line.top;
	font.SetSize(MM(3.5)); SetFont(&font);
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("1");
	MovePenTo(line.left+MM(14),line.top-MM(1));
	DrawString("LOKALIZACJA");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("M.");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("Nazwa lokalna");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("GM.");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("POW.");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("WOJ.");
	font.SetSize(MM(4)); SetFont(&font);
	BRect line2(line);
	line2.left = line2.right - MM(23);
	line2.bottom = rbox1.bottom;
	StrokeLine(line2.LeftTop(),line2.LeftBottom());
	line.OffsetBy(0,MM(7));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("NR OBSZARU");
	font.SetSize(MM(2)); SetFont(&font);
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("nr stanowiska w miejscowości");
	line.OffsetBy(0,MM(5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("nr stanowiska na obszarze");
	line.OffsetBy(0,MM(10));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(4));
	DrawString("współrzędne");
	MovePenTo(line2.left+MM(2),line.top-MM(6));
	DrawString("x-");
	MovePenTo(line2.left+MM(2),line.top-MM(1));
	DrawString("y-");
	StrokeLine(BPoint(line2.left,line.top-MM(5)),BPoint(line2.right,line.top-MM(5)));
	line.top = rbox1.bottom; line.bottom = line.top;
	MovePenTo(line.left+MM(3),line.top-MM(4));
	DrawString("nr inwentarza");
	}
	// box3
	BRect rbox3;
	rbox3.left = rbox1.left; rbox3.right = rbox1.right;
	rbox3.top = rbox1.bottom; rbox3.bottom = rbox3.top + MM(60);
	StrokeLine(rbox3.LeftBottom(),rbox3.RightBottom());
	StrokeLine(rbox3.RightTop(),rbox3.RightBottom());
	{
	// box3 lines
	BRect line(rbox3);
	line.top = line.top + MM(5); line.bottom = line.top;
	font.SetSize(MM(3.5)); SetFont(&font);
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(3),line.top-MM(1));
	DrawString("3");
	MovePenTo(line.left+MM(14),line.top-MM(1));
	DrawString("DOSTĘPNOŚĆ TERENU");
	line.OffsetBy(0,MM(8.5));
	StrokeLine(line.LeftTop(),line.RightBottom());
	font.SetSize(MM(2)); SetFont(&font);
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("niezabudowany/średniozabudowany/zabudowany");
	line.OffsetBy(0,MM(7));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("las  /  sad  /  park");
	line.OffsetBy(0,MM(7));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("pole orne  /  łąka");
	line.OffsetBy(0,MM(7));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("teren rolniczy: prywatny/społeczny");
	line.OffsetBy(0,MM(7));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("teren przemysłowy");
	line.OffsetBy(0,MM(6));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("Nr działki:");
	line.OffsetBy(0,MM(6));
	StrokeLine(line.LeftTop(),line.RightBottom());
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("Obręb:");
	line.OffsetBy(0,MM(6));
	MovePenTo(line.left+MM(2),line.top-MM(1));
	DrawString("Okr. blizsze:");	// XXX 'ż!'
	// box3 strokes
	BRect line2(line);
	line2.left = line2.right - MM(21);
	line2.top = rbox3.top+MM(5); line2.bottom = line2.top+MM(8.5)+MM(4*7);
	StrokeLine(line2.LeftTop(),line2.LeftBottom());
	// XXX finish this
	}
}

void BeKESAPrint::DrawValues(BRect r) {
	BFont font(be_plain_font);
	font.SetFamilyAndStyle("Arial","Regular");
	font.SetFlags(B_DISABLE_ANTIALIASING);
	BFont fontb(be_bold_font);
	fontb.SetFamilyAndStyle("Arial","Bold");
	fontb.SetFlags(B_DISABLE_ANTIALIASING);
	font.SetSize(10.0);
	fontb.SetSize(10.0);

	BRect frame(r.left+MM(10),r.top+MM(7),r.right-MM(17),r.bottom-MM(9));
	// XXX wstawic 'X' w ATWPLRZS
	// XXX osobna funkcja do centrowanych "X" w boksach
	// box1
	BRect rbox1;
	rbox1.left = frame.left; rbox1.right = frame.left + MM(68);
	rbox1.top = frame.top + MM(10); rbox1.bottom = rbox1.top + MM(65);
	{
	font.SetSize(MM(3.5)); SetFont(&font);
	MovePenTo(rbox1.left+MM(15),rbox1.top+MM(5)+MM(4));
	DrawString(data->t1miejsc.String());
	MovePenTo(rbox1.left+MM(30),rbox1.top+MM(10)+MM(4));
	DrawString(data->t1nazwalokalna.String());
	MovePenTo(rbox1.left+MM(20),rbox1.top+MM(15)+MM(4));
	DrawString(data->t1gmina.String());
	MovePenTo(rbox1.left+MM(20),rbox1.top+MM(20)+MM(4));
	DrawString(data->t1powiat.String());
	MovePenTo(rbox1.left+MM(20),rbox1.top+MM(25)+MM(4));
	DrawString(data->t1wojewodztwo.String());
	MovePenTo(rbox1.right-MM(23)+MM(2),rbox1.top+MM(32)+MM(4));
	DrawString(data->t1nrobszaru.String());
	MovePenTo(rbox1.right-MM(23)+MM(2),rbox1.top+MM(37)+MM(4));
	DrawString(data->t1stanmiejsc.String());
	MovePenTo(rbox1.right-MM(23)+MM(2),rbox1.top+MM(42)+MM(4));
	DrawString(data->t1stanobszar.String());
	MovePenTo(rbox1.right-MM(23)+MM(5),rbox1.top+MM(47)+MM(4));
	DrawString(data->t1x.String());
	MovePenTo(rbox1.right-MM(23)+MM(5),rbox1.top+MM(52)+MM(4));
	DrawString(data->t1y.String());
	MovePenTo(rbox1.right-MM(23)+MM(2),rbox1.top+MM(57)+MM(4));
	DrawString(data->t1nrinwent.String());
	}
	
}
