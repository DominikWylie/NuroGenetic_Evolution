#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Fighter.h"
#include "UIManager.h"
#include "SimulationManager.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include "Button.h"
#include <thread>


class Game
{
public:
	Game();
	
	void run();

	void debugControl(float frameTime);

	std::function<void(Button&)> threadingButtonClick();
	void toggleThreading(Button& button);

	//void collision();

private:
	sf::RenderWindow window;

	const int SCREEN_WIDTH = 1300, SCREEN_HEIGHT = 800;

	const int POPULATION_SIZE = 300;
	const float SIMULATION_TIME_LIMIT = 150.f;

	bool threadingEnabled = true;
	bool renderNetsUpdateNeeded = false;

	bool IsDebugControllOn = false;

	sf::Clock clock;

	UIManager uiManager;

	SimulationManager simulationController;
};

