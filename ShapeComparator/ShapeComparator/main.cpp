#include "include.h"

#include "app.h"

int main(int argc, char** argv) {
	// create application
	app* apka = new app();
	if (!apka) {
		cout << "At FILE: main.cpp\tLINE:8\t\t";
		cout << "Unable to start application";
		system("pause");
		return -1;
	}
	// initialization from file
	if (argc != 4) {
		cout << "Wrong argumnts!" << endl;
		cout << "Usage: .exe PATH PIC MET" << endl;
		cout << "PATH - path to database" << endl;
		cout << "PIC  - number of pictures in targets (must be equal to pictures per method!)" << endl;
		cout << "MET  - number of methods" << endl; 
		cout << "PATH\\targets\\000.png" << endl;
		cout << "          ..\\001.png" << endl;
		cout << "          ..\\..." << endl;
		cout << "  ..\\predicts\\000\\000.png" << endl;
		cout << "               ..\\001.png" << endl;
		cout << "               ..\\..." << endl;
		cout << "           ..\\001\\000.png" << endl;
		cout << "               ..\\001.png" << endl;
		cout << "               ..\\..." << endl;
		cout << "           ..\\...\\000.png" << endl;
		cout << "               ..\\001.png" << endl;
		cout << "               ..\\..." << endl;
		system("pause");
		return -1;
	}
	// default initialization
	else {
		if (apka->init(argv[1], stoi(argv[2]), stoi(argv[3]))) {
			cout << "At FILE: main.cpp\tLINE:26\t\t";
			cout << "Unable to initialize application" << endl;
			system("pause");
			return -1;
		}
	}
	// main cycle
	if (apka->cycle()) {
		cout << "At FILE: main.cpp\tLINE:34\t\t";
		cout << "Unable to continue application" << endl;
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