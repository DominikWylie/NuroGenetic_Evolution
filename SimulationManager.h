#pragma once

#include "Button.h"
#include <vector>
#include "Fighter.h"
#include "GeneticManager.h"
#include "Simulation.h"
#include <random>
#include <thread>


using std::vector;

class SimulationManager
{
public:
	SimulationManager(sf::RenderWindow& newWindow, int populationSize, float simulationTimeLimit);
	void Update(float frameTime);
	void RunThreadsSession();
	void Session(float frameTime, int vecRow);
	vector<float> Reset();
	void Render();
	void AllowRendering(bool value) { allowRendering = value; }
	void SetTimeScale(float value) {
		timeScale = value;
	}
	void setRenderNetworks();
	void saveFittestNetwork();
	void loadNetwork();

	int getGeneration() { return evolutionManager.GetCurrentGeneration(); }
	float getAverageFitness() { return evolutionManager.GetAverageFitness(); }

	void debugControl(sf::Vector2f move, float weapon, float frameTime);
public:
	std::function<void(Button&)> GetDrawButtonClick();
	std::function<void(Button&)> GetPauseButtonClick();
	std::function<std::map<std::string, std::string>()> GetSimulationDetailsCallback();

private:
	void DrawButtonClick(Button& button);
	void PauseButtonClick(Button& button);
	std::map<std::string, std::string> SimulationDetails();
private:
	void Step(float frameTime);
private:
	vector<Simulation> simulations;

	const static int THREADS_AMOUNT = 16;
	vector<vector<Simulation>> tSimulations;

	GeneticManager evolutionManager;
	sf::RenderWindow& window;
	float timeScale;
	bool allowRendering;
	bool bestRendering;
	bool paused;
	unsigned bestSimulationIndex;
	float simulationTime;
	float elapsedTime;
	float tickTime = 1.f / 60.f;
	float tickDTime = 0;
};