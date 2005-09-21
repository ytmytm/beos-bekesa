
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

	#include <SpLocaleApp.h>
	#include <Window.h>
	#include "globals.h"

	class BButton;
	class BCheckBox;
	class BListView;
	class BMenu;
	class BMenuBar;
	class BTabView;
	class BTextControl;

	class BeKESAMainWindow : public BWindow {
		public:
			BeKESAMainWindow(const char *windowTitle);
			~BeKESAMainWindow();
//			virtual void FrameResized(float width, float height);
			virtual void MessageReceived(BMessage *Message);
//			virtual void DispatchMessage(BMessage *message, BHandler *handler);
			virtual bool QuitRequested();
//			virtual void RefsReceived(BMessage *Message);
		private:
			void initTab1(BTabView *tv);

			BMenuBar *menuBar;
			BMenu *menu;

			BListView *listView;

			BTabView *tabView;

			BButton *but_new, *but_save;

			BTextControl *t1miejsc, *t1nazwalokalna, *t1gmina, *t1powiat, *t1wojewodztwo;
			BTextControl *t1nrobszaru, *t1nrinwent, *t1x, *t1y, *t1stanmiejsc, *t1stanobszar;

			BCheckBox *t1cz, *t1ct, *t1cl, *t1cw, *t1ca, *t1cp, *t1cr;
	};

#endif
