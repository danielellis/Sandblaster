#include <iostream>
#include "GameWindow.h"

using namespace std;

int main(int argc, char **argv) {
    GameWindow game("Sandblaster", 1152, 864);

	if (!game.Initialize()) {
		cerr << "Could not initialize game." << endl; //print some related error msg
		exit(1);
	}

	try {
		game.Run();
	}
	catch (exception e) {
		cerr << "Error running game: " << e.what();
	}

	if (!game.ShutDown()) {
		cerr << "Could not shutdown game properly." << endl;
		exit(1);
	}

	return 0;
}