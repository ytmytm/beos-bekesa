
#ifndef _BEKESAPRINT_H
#define _BEKESAPRINT_H

#include <View.h>
#include "mainwindow.h"

class BMessage;
class BPrintJob;
class BWindow;

class BeKESAPrint : public BView {

	public:
		BeKESAPrint(int id, sqlite *db, BMessage *pSettings);
		virtual ~BeKESAPrint();
		void Go(void);
		virtual void Draw(BRect updateRect);
		virtual void AttachedToWindow(void);
	private:
		kesadat *data;

		// database
		sqlite *dbData;
		char *dbErrMsg;

		BWindow	*pWindow;
		BPrintJob *printJob;
};

#endif
