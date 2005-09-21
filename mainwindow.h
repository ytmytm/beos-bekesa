
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

	#include <SpLocaleApp.h>
	#include <View.h>
	#include <Window.h>
	#include <TextControl.h>
	#include <TextView.h>
	#include <MenuItem.h>
//	#include <FilePanel.h>
	#include "globals.h"

	class BeKESAMainWindow : public BWindow {
		public:
			BeKESAMainWindow(const char *windowTitle);
			~BeKESAMainWindow();
//			virtual void FrameResized(float width, float height);
			virtual void MessageReceived(BMessage *Message);
//			virtual void DispatchMessage(BMessage *message, BHandler *handler);
			virtual bool QuitRequested();
//			virtual void RefsReceived(BMessage *Message);
	};

#endif
