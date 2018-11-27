#include "Game.h"

int main(int argc, void** argv[]) {
	// Program entry point.
	Game game; // Creating our game object.

	while (!game.GetWindow()->IsDone()) {
		// Game loop.
		game.HandleInput();
		game.Update();
		game.Render();
		game.RestartClock();
	}

	return 0;
}