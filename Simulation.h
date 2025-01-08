#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "NeuralNetwork.h"
#include "Fighter.h"
#include "Randomise.h"

using std::vector;

class Simulation
{
public:
	Simulation(NeuralNetwork& newNetwork1, NeuralNetwork& newNetwork2, sf::Vector2u areaSize);
	void Reset();
	void Reset(NeuralNetwork& newNetwork1, NeuralNetwork& newNetwork2, int GenNum);
	void Update(float frameTime);
	void Render(sf::RenderWindow& window);
	const bool IsInCombat() const { return inCombat; }
	void End(float elapsedTimeScale);
	float GetFitness();
	void ForceControllerColour(sf::Color newColour) { controller.setFillColor(newColour); }
	bool AwaitingReset() { return waitingReset; }

	void debugControl(sf::Vector2f move, float weapon, float frameTime);

	float getWeaponPos() { return fighter1.getWeaponMove(); }

private:
	void CollisionDetection();
	void MoveFighters(vector<float> network1Outputs, vector<float> network2Outputs, float frameTime);
	void RandomiseFighterPosition(Fighter& fighter);

private:
	sf::RectangleShape controller;
	sf::RectangleShape block;
	bool inCombat;
	sf::Vector2u windowSize;

	NeuralNetwork& network1;
	Fighter fighter1;
	sf::Vector2f fighter1Pos;
	sf::Vector2f prevFighter1Pos;

	NeuralNetwork& network2;
	Fighter fighter2;
	sf::Vector2f fighter2Pos;
	sf::Vector2f prevFighter2Pos;

	float force;
	float fighter1Fitness = 0;
	float fighter2Fitness = 0;
	bool waitingReset;

	int generationNumber;

	//input
//enemy position, (player - enemy)
//enemy weapon position (enemy - enemy weapon) - i might allready have this value
//world position
//weapon position, (player - weapon) - i might allready have this value
//velocity
//

		//output
	//force direction
	//weapon move

	//2, 1
	//3

};

