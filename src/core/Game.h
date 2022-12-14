// 2022 Tygo Boons

// Game instance class partly from:
// Template, BUAS version https://www.buas.nl/games
// IGAD / BUAS(NHTV) / UU - Jacco Bikker - 2006 - 2020

#pragma once

#include "Core.h"
#include "Scene/SceneManager.h"
#include "Renderer/Renderer.h"
#include "Window.h"

namespace Core
{

	class Game
	{
	public:
		Game(const RefPtr<Window>& window)
			: m_Window(window)
		{
			// Fill singleton instance with this.
			s_Instance = this;
		}

		// Runs when the game starts.
		void Start();

		// Runs when game is shut down.
		void Shutdown();

		// Runs every frame.
		void Tick();

		// Runs for every input event.
		void OnEvent(sf::Event& event);

		// Get window instance.
		inline RefPtr<Window> GetWindow() const { return m_Window; }

		// Get scene manager instance.
		inline RefPtr<Core::SceneManager> GetSceneManager() const { return m_SceneManager; }

		// Static method to get the Game instance.
		static Game* Get() { return s_Instance; }

		// Get window initialization properties.
		static RefPtr<Window> GetWindowProperties();

	private:
		static Game* s_Instance;

		RefPtr<SceneManager> m_SceneManager;
		RefPtr<Window> m_Window;
		RefPtr<Entity::Camera> m_Camera;
	};

};