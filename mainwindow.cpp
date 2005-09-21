
#include "mainwindow.h"
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <Path.h>
//#include <Clipboard.h>
#include <Alert.h>
#include <stdio.h>

BeKESAMainWindow::BeKESAMainWindow(const char *windowTitle) : BWindow(
	BRect(100, 100, 740, 580), windowTitle, B_DOCUMENT_WINDOW, B_OUTLINE_RESIZE ) {

//	this->Hide();
//	config = new bydpConfig();

	BView *MainView(
		new BView(BWindow::Bounds(), NULL, B_FOLLOW_ALL, 0) );

	if (MainView == NULL) {
		AppReturnValue = B_NO_MEMORY;
		be_app->PostMessage(B_QUIT_REQUESTED);
		return;
	}

	MainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BWindow::AddChild(MainView);
}

BeKESAMainWindow::~BeKESAMainWindow() {
}

void BeKESAMainWindow::MessageReceived(BMessage *Message) {
	int item;
	this->DisableUpdates();
	switch (Message->what) {
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
