
#ifndef _BEKESAAPP_H
#define _BEKESAAPP_H

#include <Application.h>

#include "mainwindow.h"

class BeKESAApp : public BApplication {
	public:
		BeKESAApp();
		~BeKESAApp();
		virtual void ReadyToRun();
	private:
		BeKESAMainWindow *mainWindow;
};

#endif
