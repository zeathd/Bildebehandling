#include <iostream>
using namespace std;

#include "Bilde.h"

int main() {
	Bilde *testbilde = new Bilde;
	testbilde->lesBitmapFil("tester.bmp");

	testbilde->redtogreyscale();
	testbilde->lagreBitmapFil("tester1.bmp");

	testbilde->linefilter();
	testbilde->lagreBitmapFil("tester2.bmp");

	delete testbilde;
	testbilde = 0;

	return 0;
}