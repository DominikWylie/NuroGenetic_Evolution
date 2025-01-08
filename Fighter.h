#pragma once

#include "SFML/Graphics.hpp"
#include <cmath>
#include <iostream>

#include "NeuralNetwork.h"

enum WeaponDirection {
	LEFT,
	RIGHT
};

class Fighter
{
public:
	Fighter();
	//Fighter(NeuralNetwork& newNetwork, sf::Vector2f areaBounds);
	~Fighter();

	void reset(NeuralNetwork& newNetwork);

	void Render(sf::RenderWindow& window);

	void update(float dt);

	void reset();

	void moveFighter(vector<float> networkOutputs, float timeFrame);

	void SetPosition(sf::Vector2f pos);

	sf::Vector2f getCentrePosition() { return position + sf::Vector2f(RADIUS, RADIUS); };

	sf::Vector2f getWeaponEndPosition() { return weaponWorldSpace[1].position; }

	float getRadius() { return RADIUS; }

	void AddForce(sf::Vector2f force);

	sf::Vector2f getMomentum() { return momentum; }
	
	void RotateWeapon(WeaponDirection direction,float dt);

	void RotateWeapon(float speed, float dt);

	//debug func
	float getWeaponMove() { return position.x; }

	void kill();

	bool isAlive() { return alive; }

	void end(float elapsedTimeScale);

	sf::Vector2f getPreviousCentrePosition() { return prevPos + sf::Vector2f(RADIUS, RADIUS); }



protected:
	sf::CircleShape body;

	sf::Vertex weaponWorldSpace[2] =
	{
		sf::Vertex(sf::Vector2f(0.f, 0.f)),
		sf::Vertex(sf::Vector2f(0.f, 0.f))
	};

	sf::Vertex weaponLocalSpace[2] =
	{
		sf::Vertex(sf::Vector2f(0.f, 0.f)),
		sf::Vertex(sf::Vector2f(0.f, 0.f))
	};

	sf::Vector2f position;
	sf::Vector2f momentum = sf::Vector2f(0, 0);

	sf::Vector2f prevPos;

	sf::Vector2f bounds;
	
	float weaponDistance = 50.f;

	float angleIncrese = 500.f;

	const float RADIUS = 20.f;

	bool alive = true;
	bool waitingReset = false;

	float moveForce = .6f;
};

