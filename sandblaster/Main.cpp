#include <iostream>
#include "GameWindow.h"

using namespace std;

int main(int argc, char **argv) {
    GameWindow game("Sandblaster", 1024, 768);

	if (!game.Initialize()) {
		cerr << "Could not initialize game." << endl;
		exit(1);
	}

	try {
		game.Run();
	}
	catch (exception e) {
		cerr << "Error running game: " << e.what();
		exit(1);
	}

	if (!game.Shutdown()) {
		cerr << "Could not shutdown game properly." << endl;
		exit(1);
	}

	return 0;
}
