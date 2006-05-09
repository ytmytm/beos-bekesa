
#ifndef _BEKESAPRINT_H
#define _BEKESAPRINT_H

#include <View.h>
#include "mainwindow.h"

class BMessage;
class BPrintJob;
class BWindow;

class BeKESAPrint : public BView {

	public:
		BeKESAPrint(kesadat *dat, BMessage *pSettings);
		virtual ~BeKESAPrint();
		void Go(void);
		virtual void Draw(BRect updateRect);
	private:
		kesadat *data;

		BWindow	*pWindow;
		BPrintJob *printJob;
};

#endif