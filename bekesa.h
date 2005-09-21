
#ifndef _BEKESAAPP_H
#define _BEKESAAPP_H

#include <SpLocaleApp.h>

#include "mainwindow.h"

class BeKESAApp : public SpLocaleApp {
	public:
		BeKESAApp();
		~BeKESAApp();
		virtual void ReadyToRun();
	private:
		BeKESAMainWindow *mainWindow;
};

#endif
