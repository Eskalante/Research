#include "app.h"

int main(int argc, char** argv) {
	debug::setDebugLevel(5);
	// create application
	app* apka = new app();
	// initialization from file
	if (argc >= 2) {
		if (apka->init(argv[1])) {
			debug::error(0, "Unable to start application.");
			system("pause");
			return -1;
		}
	}
	// default initialization
	else {
		if (apka->init()) {
			debug::error(0, "Unable to start application");
			system("pause");
			return -1;
		}
	}
	// main cycle
	if (apka->cycle()) {
		debug::error(0, "Unable to continue application");
		system("pause");
		return -1;
	}
	// termination
	if (apka) {
		apka->term();
		delete apka;
		apka = NULL;
	}
	// exit
	system("pause");
	return 0;
}