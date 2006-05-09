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

BeKESAPrint::BeKESAPrint(kesadat *dat, BMessage *pSettings) :
	BView(BRect(0,0,100,100), "BeKESAPrint", B_FOLLOW_ALL, B_WILL_DRAW) {
	data = dat;
//	data = new kesadat;
//	memcpy(data,dat,sizeof(kesadat));
//	data->t1miejsc = "daupduap";
printf("in kesaprint constr, %i\n",sizeof(kesadat));
	status_t result = B_OK;
	printJob = new BPrintJob(data->t1miejsc.String());
	printJob->SetSettings(new BMessage(*pSettings));
// dla przyspieszenia preview, potem wlaczyc!!!
//	result = printJob->ConfigJob();
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
	int32 firstPage = printJob->FirstPage();
	int32 lastPage = printJob->LastPage();
	int32 pageCount = printJob->LastPage() - printJob->FirstPage() + 1;
	printf("rect:[%f,%f,%f,%f]\n",pageRect.left,pageRect.top,pageRect.right,pageRect.bottom);
	printf("page1:%i,lastp:%i;pages:%i\n",firstPage,lastPage,pageCount);
	// calculate # of own pages (according to rect), clip lastpage against pages in document
	// dla debugu:
	// - przygotowac okno
	// - w oknie view do rysowania, tam w Draw() wszystko wyrysowac
	// - wyswietlic
printf("(%f,%f)->(%f,%f)\n",pageRect.top,pageRect.left,pageRect.bottom,pageRect.right);
	BRect r = pageRect;
	r.OffsetBy(20,20);
	pWindow = new BWindow(r, "Podgląd wydruku", B_TITLED_WINDOW, 0);
printf("go for %s!\n",data->t1miejsc.String());
	pWindow->AddChild(this);
printf("go!\n");
	MoveTo(pageRect.LeftTop());
	ResizeTo(pageRect.Width(),pageRect.Height());
	pWindow->Show();
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
	printf("(%f,%f)->(%f,%f)\n",pageRect.top,pageRect.left,pageRect.bottom,pageRect.right);

	BFont font(be_plain_font);
	font.SetFamilyAndStyle("Arial","Regular");
	font.SetFlags(B_DISABLE_ANTIALIASING);
	BFont fontb(be_bold_font);
	fontb.SetFamilyAndStyle("Arial","Bold");
	fontb.SetFlags(B_DISABLE_ANTIALIASING);
	font.SetSize(10.0);
	fontb.SetSize(10.0);

	SetFont(&font);
	MovePenTo(LEFT, TOP);
	DrawString("hello, world! drugalinia trzecialinia czwartalinia");
}
