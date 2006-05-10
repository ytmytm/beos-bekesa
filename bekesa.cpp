
#include "bekesa.h"
#include "globals.h"

BeKESAApp::BeKESAApp() : BApplication(APP_SIGNATURE) {
	mainWindow = new BeKESAMainWindow(APP_NAME);
	if (mainWindow != NULL) {
		mainWindow->Show();
	} else {
		AppReturnValue = B_NO_MEMORY;
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
}

BeKESAApp::~BeKESAApp() {
	if (mainWindow != NULL)
		if (mainWindow->LockWithTimeout(30000000) == B_OK)
			mainWindow->Quit();
}

void BeKESAApp::ReadyToRun() {
	// sth etc. just before running Run()
}
