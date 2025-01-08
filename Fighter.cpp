#include "Fighter.h"

Fighter::Fighter()
{
	body.setRadius(RADIUS);

	weaponLocalSpace[1].position.x = weaponDistance;
}

Fighter::~Fighter()
{
}

void Fighter::reset(NeuralNetwork& newNetwork)
{
	//do later
}

void Fighter::Render(sf::RenderWindow& window)
{
	body.setPosition(position);
	window.draw(body);

	sf::Vector2f weaponPos = weaponLocalSpace[1].position + weaponWorldSpace[1].position;
	window.draw(weaponWorldSpace, 2, sf::Lines);
}

void Fighter::update(float dt)
{
	prevPos = position;
	position += momentum * dt;

	weaponWorldSpace[0] = position + sf::Vector2f(RADIUS, RADIUS);
	weaponWorldSpace[1] = weaponLocalSpace[1].position + position + sf::Vector2f(RADIUS, RADIUS);



	//start will be in the middle
	//end will be somewhere outside the body
}

void Fighter::reset()
{
	momentum = sf::Vector2f(0, 0);
}

void Fighter::moveFighter(vector<float> networkOutputs, float timeFrame)
{
	if (networkOutputs.size() < 3) {
		return;
	}

	momentum.x += networkOutputs[0] * moveForce * timeFrame;
	momentum.y += networkOutputs[1] * moveForce * timeFrame;

	RotateWeapon(networkOutputs[2], timeFrame);
}

void Fighter::SetPosition(sf::Vector2f pos)
{
	position = pos;
	prevPos = pos;
	weaponWorldSpace[0] = pos;
}

void Fighter::AddForce(sf::Vector2f force)
{
	momentum += force;
}

void Fighter::RotateWeapon(WeaponDirection direction, float dt)
{
	//float radWeaponAngle;

	sf::Transform transform;

	switch (direction) {
	case LEFT:

		transform.rotate(-angleIncrese * dt);
		
		break;
	case RIGHT: 

		transform.rotate(angleIncrese * dt);

		break;
	}

	weaponLocalSpace[1] = transform.transformPoint(weaponLocalSpace[1].position);
}

void Fighter::RotateWeapon(float speed, float timeFrame)
{
	sf::Transform transform;

	transform.rotate((speed * angleIncrese) * timeFrame);

	weaponLocalSpace[1] = transform.transformPoint(weaponLocalSpace[1].position);
}

void Fighter::kill()
{
	alive = false;
}

void Fighter::end(float elapsedTimeScale)
{
	//network.SetFitness(getFitness() + elapsedTimeScale);
	//alive = false;
	//waitingReset = true;
}
