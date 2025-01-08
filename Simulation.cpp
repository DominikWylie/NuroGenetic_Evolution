#include "Simulation.h"

Simulation::Simulation(NeuralNetwork& newNetwork1, NeuralNetwork& newNetwork2, sf::Vector2u areaSize)
	: windowSize(areaSize)
	, inCombat(true)
	, network1(newNetwork1)
	, network2(newNetwork2)
	, waitingReset(false)
{
	//fighter1Pos = sf::Vector2f(windowSize.x - ((windowSize.x / 4) * 3), windowSize.y / 2);
	//fighter2Pos = sf::Vector2f(windowSize.x - (windowSize.x / 4), windowSize.y / 2);

	Randomise random;

	fighter1.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
	fighter2.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));

}

void Simulation::Reset()
{
	fighter1.reset();
	fighter2.reset();

	//fighter1Fitness = 0;
	//fighter2Fitness = 0;

	fighter1.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
	fighter2.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));

	//waitingReset = false;
	inCombat = true;
}

void Simulation::Reset(NeuralNetwork& newNetwork1, NeuralNetwork& newNetwork2, int GenNum)
{
	generationNumber = GenNum;

	network1 = newNetwork1;
	network2 = newNetwork2;

	fighter1.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
	fighter2.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));

	fighter1.reset();
	fighter2.reset();

	fighter1Fitness = 0;
	fighter2Fitness = 0;

	waitingReset = false;
	inCombat = true;
}

void Simulation::Update(float frameTime)
{
	if (inCombat) {
		vector<float> fighter1Inputs{
			fighter1.getCentrePosition().x - fighter2.getCentrePosition().x, fighter1.getCentrePosition().y - fighter2.getCentrePosition().y,
			fighter1.getCentrePosition().x - fighter1.getWeaponEndPosition().x, fighter1.getCentrePosition().y - fighter1.getWeaponEndPosition().y,
			fighter1.getCentrePosition().x, fighter1.getCentrePosition().y,
			fighter2.getCentrePosition().x - fighter2.getWeaponEndPosition().x, fighter2.getCentrePosition().y - fighter2.getWeaponEndPosition().y,
			fighter1.getMomentum().x, fighter1.getMomentum().y
		};

		fighter1.moveFighter(network1.RunNetwork(fighter1Inputs), frameTime);
		fighter1.update(frameTime);

		vector<float> fighter2Inputs{
			fighter2.getCentrePosition().x - fighter1.getCentrePosition().x, fighter2.getCentrePosition().y - fighter1.getCentrePosition().y,
			fighter2.getCentrePosition().x - fighter2.getWeaponEndPosition().x, fighter2.getCentrePosition().y - fighter2.getWeaponEndPosition().y,
			fighter2.getCentrePosition().x, fighter2.getCentrePosition().y,
			fighter1.getCentrePosition().x - fighter1.getWeaponEndPosition().x, fighter1.getCentrePosition().y - fighter1.getWeaponEndPosition().y,
			fighter2.getMomentum().x, fighter2.getMomentum().y
		};

		//means model wont move
		fighter2.moveFighter(network2.RunNetwork(fighter2Inputs), frameTime);
		fighter2.update(frameTime);

		CollisionDetection();
	}
}

void Simulation::Render(sf::RenderWindow& window)
{
	if (inCombat) {
		fighter1.Render(window);
		fighter2.Render(window);
	}
}

void Simulation::End(float elapsedTimeScale)
{
	//network1.SetFitness(network1.GetFitness() + elapsedTimeScale);
	//network2.SetFitness(network2.GetFitness() + elapsedTimeScale);
	inCombat = false;
	waitingReset = true;
}

float Simulation::GetFitness()
{
	return network1.GetFitness();
}

void Simulation::debugControl(sf::Vector2f move, float weapon, float frameTime)
{
	fighter1.AddForce(move);
	fighter1.RotateWeapon(weapon, frameTime);
}

void Simulation::CollisionDetection()
{
	//manually doing it to get it finished

	sf::Vector2f fighter1Pos = fighter1.getCentrePosition();
	sf::Vector2f fighter2Pos = fighter2.getCentrePosition();

	float fighter1WeaponDistance = std::sqrt(
		pow(fighter2Pos.x - fighter1.getWeaponEndPosition().x, 2) +
		pow(fighter2Pos.y - fighter1.getWeaponEndPosition().y, 2)
	);

	float fighter2WeaponDistance = std::sqrt(
		pow(fighter1Pos.x - fighter2.getWeaponEndPosition().x, 2) +
		pow(fighter1Pos.y - fighter2.getWeaponEndPosition().y, 2)
	);

	//encorage getting cloaser to eachother

	//sf::Vector2f currentVDistance = fighter1Pos - fighter2Pos;

	//if (generationNumber < 500) {
		float currentDistance = std::sqrt(
			pow(fighter1Pos.x - fighter2Pos.x, 2) +
			pow(fighter1Pos.y - fighter2Pos.y, 2)
		);

		float previousDistance = std::sqrt(
			pow(fighter1.getPreviousCentrePosition().x - fighter2.getPreviousCentrePosition().x, 2) +
			pow(fighter1.getPreviousCentrePosition().y - fighter2.getPreviousCentrePosition().y, 2)
		);

		//encourage them moving towards eachother just a wee bit, might still be way too much
		if (currentDistance < previousDistance) {
			fighter1Fitness += .1f;
			network1.SetFitness(fighter1Fitness);

			fighter2Fitness += .1f;
			network2.SetFitness(fighter2Fitness);
		}
	//}


	//if leave the screen decrese fitness

	if (fighter1Pos.x < 0 ||
		fighter1Pos.y < 0 ||
		fighter1Pos.x > windowSize.x ||
		fighter1Pos.y > windowSize.y) 
	{
		//fighter2Fitness++;
		//network2.SetFitness(fighter2Fitness);

		//fighter1Fitness -= 5;
		//network1.SetFitness(fighter1Fitness);
		inCombat = false;
	}

	if (fighter2Pos.x < 0 ||
		fighter2Pos.y < 0 ||
		fighter2Pos.x > windowSize.x ||
		fighter2Pos.y > windowSize.y)
	{
		//fighter1Fitness++;
		//network1.SetFitness(fighter1Fitness);

		//just to discorage leaving the screen intensely
		//fighter 2 lose 3/4 fitness
		//fighter2Fitness -= 5;
		//network2.SetFitness(fighter2Fitness);
		inCombat = false;
	}

	if (fighter1WeaponDistance < fighter2.getRadius() && fighter2WeaponDistance < fighter1.getRadius()) {
		//draw
		//no change

	}
	else if (fighter1WeaponDistance < fighter2.getRadius()) {
		//figter 1 win
		fighter1Fitness++;
		network1.SetFitness(fighter1Fitness);

		//fighter 2 lose 3/4 fitness
		fighter2Fitness--;
		network2.SetFitness(fighter2Fitness);
		fighter2.reset();
		fighter2.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
		//RandomiseFighterPosition(fighter2);
	}
	else if (fighter2WeaponDistance < fighter1.getRadius()) {
		//fighter 2 win
		fighter2Fitness++;
		network2.SetFitness(fighter2Fitness);

		fighter1Fitness--;
		network1.SetFitness(fighter1Fitness);
		fighter1.reset();
		fighter1.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
		//RandomiseFighterPosition(fighter1);
	}

	//for (int i = 0; i < FIGHTERS_AMOUNT; i++) {
	//	for (int j = 0; j < FIGHTERS_AMOUNT; j++) {

	//		//skip if its checking against itself
	//		if (i == j || !fighters[i].isAlive() || !fighters[j].isAlive()) {
	//			continue;
	//		}

	//		float distanceToCentre = std::sqrt(
	//			pow(fighters[i].getCentrePosition().x - fighters[j].getWeaponEndPosition().x, 2) +
	//			pow(fighters[i].getCentrePosition().y - fighters[j].getWeaponEndPosition().y, 2)
	//		);

	//		if (distanceToCentre < fighters[i].getRadius()) {
	//			fighters[i].kill();
	//		}
	//	}
	//}
}

void Simulation::MoveFighters(vector<float> networkOutputs, vector<float> network1Outputs, float frameTime)
{



}

void Simulation::RandomiseFighterPosition(Fighter& fighter)
{
	fighter.SetPosition(sf::Vector2f(Randomise::Real(0, windowSize.x), Randomise::Real(0, windowSize.y)));
}
