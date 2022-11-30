// 2022 Tygo Boons

// Using SFML graphics library
// found at https://www.sfml-dev.org/

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Core.h"
#include "Game.h"
#include "Scene.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	// Allocate console if in debug configuration.
	ALLOC_CONSOLE()


	// Create game instance.
	Core::Game game(Core::Game::GetWindowProperties(), new Core::SceneManager());

	// Call game start method.
	game.Start();

	// Create sfml clock to use to get delta time.
	sf::Clock deltaClock;

	// Main window loop
	while (game.GetWindow()->isOpen())
	{

		// Poll SFML events.
		sf::Event event;
		while (game.GetWindow()->pollEvent(event))
		{
			// Poll window close event.
			if (event.type == sf::Event::Closed)
			{
				game.GetWindow()->close();
			}
		}

		// Update game loop.
		game.Tick();

	}

	// Close the game.
	game.Shutdown();

	return 0;
}