#include <iostream>
#include "GameWindow.h"
#include "TextureManager.h"

using std::cerr;
using std::endl;
using std::exception;

int main(int argc, char **argv) {
    // TODO remove me
    TextureManager *textureManager = new TextureManager();
    textureManager->Initialize();
    // TODO remove me

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

    // TODO remove me
    textureManager->Shutdown();
    delete textureManager;
    // TODO remove me

	return 0;
}
