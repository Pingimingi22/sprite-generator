#include "FL/Fl.H"
#include "MyCubeViewUI.h"

int main(int argc, char** argv) {
	MyCubeViewUI* test = new MyCubeViewUI();
	test->show(argc, argv);
	return Fl::run();
}