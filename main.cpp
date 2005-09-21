
#include "bekesa.h"

int AppReturnValue(0);

int main (void) {
	AppReturnValue = B_ERROR;
	BeKESAApp myApp;
	myApp.Run();
	return AppReturnValue;
}