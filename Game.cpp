#include "Game.h"

Game::Game()
	:window(sf::VideoMode(1600, 900), "NeuroEvolition Fighting Game", sf::Style::Titlebar | sf::Style::None | sf::Style::Close)
	,uiManager(window)
	,simulationController(window, POPULATION_SIZE, SIMULATION_TIME_LIMIT)
{

	uiManager.AssignButtonCallback("ToggleThreadingButton", Game::threadingButtonClick());
	uiManager.AssignButtonCallback("PauseButton", simulationController.GetPauseButtonClick());
	uiManager.AssignButtonCallback("DrawFittestButton", simulationController.GetDrawButtonClick());
	uiManager.SetSimulationDataCallback(simulationController.GetSimulationDetailsCallback());

	simulationController.loadNetwork();

		//start at source
	//how many games at once
	//depends on the arenas and amounts of fighters
	//ill finbd out how my pc can handle it

	//fighter1.SetPosition(sf::Vector2f(300.f, 200.f));
	//fighter2.SetPosition(sf::Vector2f(400.f, 300.f));

	//sf::Vector2f startPos = sf::Vector2f(300.f, 200.f);
	//float increment = 100;

	//for (Fighter& fighter : fighters) {
	//	fighter.SetPosition(startPos);
	//	startPos.x += increment;
	//	startPos.y += increment;
	//}
}

void Game::run()
{
	std::ofstream outFile("fitness.txt");

	while (window.isOpen()) {
		sf::Event event;

		float dt = clock.restart().asSeconds();
		float framerate = 1 / 30;
		float timePassed = 0;

		window.clear(sf::Color(18, 33, 43));

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			uiManager.EventUpdate(event);
		}

		uiManager.Update();

		if (threadingEnabled) {

			simulationController.RunThreadsSession();
			vector<float> fitnesses = simulationController.Reset();

			if (simulationController.getGeneration() % 20 == 0) {
				std::cout << "Fittest net : " + std::to_string(fitnesses.at(0)) +
					", Average net : " + std::to_string(fitnesses.at(1)) + "\n";

				if (outFile.is_open()) {
					outFile << std::to_string(fitnesses.at(1)) << ", " << std::to_string(fitnesses.at(2)) << "\n";
				}

			//std::string out = "Fittest net : " + std::to_string(population.at(0).GetFitness()) + 
			//	", Average net : " + std::to_string(GetAverageFitness()) + "\n";
			}

		}
		else {

			//limit to 30fps
			//float frameRate = 1 / 30;

			//std::this_thread::sleep_for(std::chrono::seconds((long)frameRate));

			//std::cout << "render loop\n";


			if (IsDebugControllOn) {
				debugControl(dt);
			}

			if (renderNetsUpdateNeeded) {
				simulationController.setRenderNetworks();
				simulationController.saveFittestNetwork();
				renderNetsUpdateNeeded = false;
				//std::cout << "updated render nets\n";
			}

			timePassed += dt;

			//if (timePassed > framerate) {
				simulationController.SetTimeScale(uiManager.GetTimeScale());
				simulationController.Update(dt);
				timePassed = 0;
			//}

			simulationController.Render();

		}

		uiManager.Render();
		window.display();

	}



	////seperate for when observing, i think make 2 seperate systems one for training and one for watching the latest model
	//while (window.isOpen()) {
	//	sf::Event event;

	//	while (window.pollEvent(event)) {
	//		if (event.type == sf::Event::Closed) {
	//			window.close();
	//		}
	//	}

	//	float dt = dtClock.restart().asSeconds();

	//	window.clear(sf::Color(18, 33, 43)); // Color background

	//	float force = 0.2f;


	//	for (Fighter& fighter : fighters) {
	//		if (fighter.isAlive()) {
	//			fighter.update(dt);
	//			fighter.Render(window);
	//		}
	//	}

	//	collision();

	//	window.display();

	//}
}

void Game::debugControl(float frameTime)
{

	//input debugging

	sf::Vector2f force = sf::Vector2f(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

		force += sf::Vector2f(-1.f, 0.f);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

		force += sf::Vector2f(1.f, 0.f);

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

		force += sf::Vector2f(0.f, -1.f);

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

		force += sf::Vector2f(0.f, 1.f);

	}

	float weaponD = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

		weaponD = 1;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

		weaponD = -1;

	}

	simulationController.debugControl(force, weaponD, frameTime);
}

std::function<void(Button&)> Game::threadingButtonClick()
{
	return std::bind(&Game::toggleThreading, this, std::placeholders::_1);
}

void Game::toggleThreading(Button& button)
{
	threadingEnabled = !threadingEnabled;
	renderNetsUpdateNeeded = true;
}

//void Game::collision()
//{
	//std::vector<std::vector<bool>> compared(FIGHTERS_AMOUNT, std::vector<bool>(FIGHTERS_AMOUNT, false));

	//maybe change to the other for stuff

	//for (int i = 0; i < FIGHTERS_AMOUNT; i++) {
	//	for (int j = 0; j < FIGHTERS_AMOUNT; j++) {

	//		//skip if its checking against itself
	//		if (i == j || !fighters[i].isAlive() || !fighters[j].isAlive()) {
	//			continue;
	//		}

	//		float distanceToCentre = std::sqrt(
	//			pow(fighters[i].getCentrePosition().x - fighters[j].getWeaponEndPosition().x, 2) + 
	//			pow(fighters[i].getCentrePosition().y - fighters[j].getWeaponEndPosition().y, 2)
	//			);

	//		if (distanceToCentre < fighters[i].getRadius()) {
	//			fighters[i].kill();
	//		}
	//	}
	//}
//}
