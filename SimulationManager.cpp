#include "SimulationManager.h"

SimulationManager::SimulationManager(sf::RenderWindow& newWindow, int populationSize, float simulationTimeLimit)
	: window(newWindow)
	, evolutionManager(10, 3, 10, 10, populationSize)
	, timeScale(1.f)
	, allowRendering(true)
	, bestRendering(false)
	, paused(false)
	, bestSimulationIndex(0)
	, simulationTime(simulationTimeLimit)
	, elapsedTime(0.f)
{

	//input
	//enemy position, (player - enemy)
	//weapon position, (player - weapon) - i might allready have this value
	//world position
	//velocity
	//enemy weapon position (enemy - enemy weapon) - i might allready have this value
	//

	//2, 2, 2, 2, 2
	//10

	//output
	//force direction
	//weapon move

	//2, 1
	//3

	//5 diffen layers
	//5 layer size
	//after its done have a mess with it

	sf::Vector2u simulationArea = window.getSize();
	simulationArea.y = (unsigned)(simulationArea.y * 0.9f);

	vector<NeuralNetwork>& networks = evolutionManager.GetPopulation();

	tSimulations.resize(THREADS_AMOUNT);
	int threadCount = 0;
	for (int i = 0; i < networks.size(); i += 2) {
		Simulation simulation(networks.at(i), networks.at(i + 1), simulationArea);
		tSimulations.at(threadCount).push_back(simulation);
		threadCount++;
		if (threadCount >= THREADS_AMOUNT - 1) {
			threadCount = 0;
		}
	}

	for (int i = 0; i < tSimulations.at(0).size(); i += 2) {
		Simulation simulation(networks.at(i), networks.at(i+1), simulationArea);
		simulations.push_back(simulation);
	}

	//grabbing the first set for rendering
	//simulations.reserve(tSimulations.at(0).size());
	//simulations = tSimulations.at(0);

	//for (auto& network : networks)
	//{
	//	sf::Vector2u simulationArea = window.getSize();
	//	simulationArea.y = (unsigned)(simulationArea.y * 0.9f);

	//	Simulation simulation()

	//	//Fighter 

	//	//Simulation simulation(network, simulationArea, 250.f);
	//	//simulations.push_back(simulation);
	//}

}

void SimulationManager::Update(float frameTime)
{
	if (!paused) {
		static float iterations = 0.f;

		tickDTime += frameTime * timeScale;

		if (tickDTime > tickTime) {

			Step(tickTime);

			//std::cout << tickTime << "\n";
			//std::cout << tickDTime << "\n";
			tickDTime = 0;
		}

		//if (tickDTime > tickDTime) {

		//	if ((int)iterations <= 0) {
		//		do {
		//			iterations += timeScale;

		//			//i do not understand this maybe kinda
		//		} while (iterations < timeScale);

		//		if (iterations > tickTime) {

		//			int stepsToComplete = (int)iterations;
		//			for (int i = 0; i < stepsToComplete; i++) {
		//				elapsedTime += frameTime;
		//				Step(frameTime);
		//				iterations -= 1.f;
		//			}

		//			tickDTime = 0;
		//		}
		//	}
		//}
	}
}

void SimulationManager::RunThreadsSession()
{
	std::vector<std::thread> sessionthreads;

	float fixedDT = 1.f / 60.f;

	for (int i = 0; i < THREADS_AMOUNT; i++) {

		sessionthreads.push_back(std::thread(&SimulationManager::Session, this, fixedDT, i));

		//std::cout << "thread added \n";

	}

	for (auto& seshThread : sessionthreads) {
		seshThread.join();
		//std::cout << "thread joined \n";
	}


}

void SimulationManager::Session(float frameTime, int vecRow)
{
		//no more pause really, only pause check at reset

	//std::cout << "thread : " << vecRow << "\n";

	float timeElapsed = 0.f;
	bool simContinue = true;

	while (simContinue) {
		simContinue = false;

		if (elapsedTime > simulationTime) {

			//std::cout << "time elapsed\n";

			break;
		}


		for (int i = 0; i < tSimulations.at(vecRow).size(); i++) {
			if (tSimulations.at(vecRow).at(i).IsInCombat()) {
				simContinue = true;

				tSimulations.at(vecRow).at(i).Update(frameTime);

				//if (vecRow == 0 && i == 0) {
				//	std::cout << "fitness : " << tSimulations.at(vecRow).at(0).GetFitness() <<
				//		", elapsed time : " << elapsedTime <<
				//		", movement : " << tSimulations.at(vecRow).at(0).getWeaponPos() <<
				//		"\n";
				//}


			}
		}

		elapsedTime += frameTime;

		//no avail - ask on discord tomo

		//for (Simulation& sim : tSimulation.at(vecRow)) {

		//	if (sim.IsInCombat()) {
		//		simContinue = true;

		//		sim.Update(frameTime);
		//	}

		//}
	}


}

vector<float> SimulationManager::Reset()
{
	//std::cout << "reset hit \n";

	vector<float> out =  evolutionManager.StepToNextGeneration();

	vector<NeuralNetwork>& networks = evolutionManager.GetPopulation();

	vector<NeuralNetwork*> rNetworks;
	rNetworks.reserve(networks.size());

	for (NeuralNetwork& network : networks) {
		rNetworks.push_back(&network);
	}

	//mix up the networks to fight eachother

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(rNetworks.begin(), rNetworks.end(), g);

	for (
		int netNum = 0, simNum = 0, threadNum = 0;
		netNum < rNetworks.size() && simNum < tSimulations.at(threadNum).size();
		netNum += 2, simNum++, threadNum++
		)
	{

		if (threadNum >= THREADS_AMOUNT - 1) {
			threadNum = 0;
		}

		tSimulations.at(threadNum).at(simNum).Reset(*rNetworks.at(netNum), *rNetworks.at(netNum + 1), evolutionManager.GetCurrentGeneration());
		//std::cout << "reset a sim\n";
	}
	elapsedTime = 0.f;

	return out;

	//tSimulations.resize(THREADS_AMOUNT);
	//int threadCount = 0;
	//for (int i = 0; i < networks.size(); i += 2) {
	//	Simulation simulation(networks.at(i), networks.at(i + 1), simulationArea);
	//	tSimulations.at(threadCount).push_back(simulation);
	//	threadCount++;
	//	if (threadCount >= THREADS_AMOUNT - 1) {
	//		threadCount = 0;
	//	}
	//}
}

void SimulationManager::Render()
{
	if (!allowRendering) {
		return;
	}

	if (bestRendering) {
		simulations.at(bestSimulationIndex).Render(window);
		//i changed it to not render any
		return;
	}

	for (Simulation simulation : simulations) {
		if (simulation.IsInCombat()) {
			simulation.Render(window);
		}
	}
}

void SimulationManager::Step(float frameTime)
{
	bool reset = true;

	for (Simulation& simulation : simulations) {
		simulation.Update(frameTime);

		if (!simulation.IsInCombat()) {
			simulation.Reset();
		}

		//if (elapsedTime >= simulationTime) {
		//	simulation.End(1.0f);
		//}

		//if (simulation.IsInCombat()) {
		//	reset = false;
		//}

		//if (simulation.AwaitingReset()) {
		//	simulation.End(elapsedTime / simulationTime);
		//}
	}

	//if (reset) {
	//	for (Simulation sim : simulations) {
	//		sim.Reset();
	//		std::cout << "reset\n";
	//	}
	//	elapsedTime = 0;
	//}

	//for (Simulation sim : simulations) {
	//	sim.Reset();
	//}

	//if (reset) {
	//	evolutionManager.StepToNextGeneration();

	//	vector<NeuralNetwork>& networks = evolutionManager.GetPopulation();

	//	//std::random_device rd;
	//	//std::mt19937 g(rd());
	//	//std::shuffle(networks.begin(), networks.end(), g);

	//	//for (
	//	//	int netNum = 0, simNum = 0; 
	//	//	netNum < networks.size() && simNum < simulations.size();
	//	//	netNum += 2, simNum++
	//	//	)
	//	//{
	//	//	simulations.at(simNum).Reset(networks.at(netNum), networks.at(netNum + 1));
	//	//}

	//	vector<NeuralNetwork*> rNetworks;
	//	rNetworks.reserve(networks.size());

	//	for (NeuralNetwork& network : networks) {
	//		rNetworks.push_back(&network);
	//	}

	//	//mix up the networks to fight eachother

	//	std::random_device rd;
	//	std::mt19937 g(rd());
	//	std::shuffle(rNetworks.begin(), rNetworks.end(), g);
	//	
	//	for (
	//		int netNum = 0, simNum = 0; 
	//		netNum < rNetworks.size() && simNum < simulations.size();
	//		netNum += 2, simNum++
	//		)
	//	{
	//		simulations.at(simNum).Reset(*rNetworks.at(netNum), *rNetworks.at(netNum + 1), evolutionManager.GetCurrentGeneration());
	//	}
	//	elapsedTime = 0.f;
	//}
}

void SimulationManager::setRenderNetworks()
{
	vector<NeuralNetwork>& networks = evolutionManager.GetPopulation();

	sf::Vector2u simulationArea = window.getSize();
	simulationArea.y = (unsigned)(simulationArea.y * 0.9f);

	simulations.clear();

	for (int i = 0; i < tSimulations.at(0).size(); i += 2) {
		Simulation simulation(networks.at(i), networks.at(i + 1), simulationArea);
		simulation.Reset();

		simulations.push_back(simulation);
	}


}

void SimulationManager::saveFittestNetwork()
{
	evolutionManager.saveFittestNetworkToFile();
}

void SimulationManager::loadNetwork()
{
	evolutionManager.loadNetwork();
}

void SimulationManager::debugControl(sf::Vector2f move, float weapon, float frameTime)
{
	simulations.at(0).debugControl(move, weapon, frameTime);
}

std::function<void(Button&)> SimulationManager::GetDrawButtonClick()
{
	return std::bind(&SimulationManager::DrawButtonClick, this, std::placeholders::_1);
}

std::function<void(Button&)> SimulationManager::GetPauseButtonClick()
{
	return std::bind(&SimulationManager::PauseButtonClick, this, std::placeholders::_1);
}

std::function<std::map<std::string, std::string>()> SimulationManager::GetSimulationDetailsCallback()
{
	return std::bind(&SimulationManager::SimulationDetails, this);
}

void SimulationManager::DrawButtonClick(Button& button)
{
	if (bestRendering)
		button.SetString("Draw Fittest");
	else
		button.SetString("Draw All");

	bestRendering = !bestRendering;
}

void SimulationManager::PauseButtonClick(Button& button)
{
	if (paused)
		button.SetString("Pause");
	else
		button.SetString("Resume");

	paused = !paused;
}

std::map<std::string, std::string> SimulationManager::SimulationDetails()
{
	std::map<std::string, std::string> simulationDetails;

	simulationDetails["generationNumber"] = std::to_string(evolutionManager.GetCurrentGeneration());

	simulationDetails["popSize"] = std::to_string(evolutionManager.GetPopulation().size());

	int simulationsAlive = 0;
	bestSimulationIndex = 0;
	for (int i = 0; i < simulations.size(); i++)
	{
		if (simulations.at(i).GetFitness() > simulations.at(bestSimulationIndex).GetFitness())
			bestSimulationIndex = i;

		if (simulations.at(i).IsInCombat())
			simulationsAlive++;
	}
	simulationDetails["popSizeAlive"] = std::to_string(simulationsAlive * 2);

	simulationDetails["bestFitness"] = std::to_string(simulations.at(bestSimulationIndex).GetFitness());

	simulationDetails["averageFitness"] = std::to_string(evolutionManager.GetAverageFitness());

	simulationDetails["timeLeft"] = std::to_string(simulationTime - elapsedTime);

	return simulationDetails;
}
