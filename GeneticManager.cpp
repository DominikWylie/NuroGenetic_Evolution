#include "GeneticManager.h"
#include "Randomise.h"
#include <algorithm>
#include <iostream>
#include <list>
using std::sort;
using std::begin;
using std::end;

// Constructor -	Allows us to specify the size of the input layer, output layer, the number of hidden layers, 
//					the size of the hidden layers and the number of solutions in the population.
GeneticManager::GeneticManager(int inputLayers, int outputLayers, int hiddenLayers, int hiddenLayerSize, int popSize)
	: populationSize(popSize)
	, mutationRate(0.05f)
	, bestAgentSelection(20)
	, worstAgentSelection(3)
	, currentGeneration(0)
{
	for (int i = 0; i < populationSize; i++)
	{
		population.push_back(NeuralNetwork(inputLayers, outputLayers, hiddenLayers, hiddenLayerSize));
	}
}

vector<float>  GeneticManager::StepToNextGeneration()
{

	// Sort the population with highest fitness first
	SortPopulation();

	//best net is at 0

	vector<float> out;

	//fittest net
	out.push_back(population.at(0).GetFitness());
	//average net
	out.push_back(GetAverageFitness());
	//gen
	out.push_back(currentGeneration);

	//std::string out = "Fittest net : " + std::to_string(population.at(0).GetFitness()) + 
	//	", Average net : " + std::to_string(GetAverageFitness()) + "\n";

	// Select and return parent nodes for crossover
	vector<NeuralNetwork> genePool = Select();

	// Crossover and return the new child solutions
	vector<NeuralNetwork> newPopulation = Crossover(genePool);

	// Mutate the new child solutions
	Mutate(newPopulation);

	// Add the new population to the existing parents
	newPopulation.insert(newPopulation.end(), genePool.begin(), genePool.end());

	// The the size of the new population matches the old, replace it, otherwise do nothing
	if (newPopulation.size() == population.size())
		population = newPopulation;

	// Bump up the generation counter
	currentGeneration++;

	return out;
}

const NeuralNetwork& GeneticManager::GetFittestNetwork()
{
	// Sort and return the best - not super efficient but it works
	SortPopulation();
	return population.front();
}

float GeneticManager::GetAverageFitness()
{
	float averageFitness = 0.f;
	for (auto& member : population)
		averageFitness += member.GetFitness();
	averageFitness /= (float)population.size();
	return averageFitness;
}

void GeneticManager::saveFittestNetworkToFile()
{
	SortPopulation();
	population.front().saveNetworkToFile();
}

void GeneticManager::loadNetwork()
{

	std::ifstream loadFile("NeuralNetworkLoad.txt");

	if (!loadFile.good()) {
		std::cout << "no file to load found \n";
		return;
	}

	if (!loadFile.is_open()) {
		std::cout << "load failed not is open\n";
		return;
	}

	std::string line;
	std::vector<std::vector<float>> sections;
	std::vector<float> currentSection;
	bool inSection = false;

	bool reading = true;

	int inputLayers, outputLayers, hiddenLayers, hiddenLayerSize;

		//first will be the network sizes
		std::getline(loadFile, line);

		if (line.find("stats") != std::string::npos) {

			std::cout << "got stats \n";

			std::getline(loadFile, line);

			//this piece of code cam from claude, and subsiquent others in this function 
			std::stringstream ss(line);
			float value;
			while (ss >> value) {

				currentSection.push_back(value);
				if (ss.peek() == ',') {
					ss.ignore();
				}
			}
			//to here

			inputLayers = currentSection.at(0);
			outputLayers = currentSection.at(1);
			hiddenLayers = currentSection.at(2);
			hiddenLayerSize = currentSection.at(3);
		}

		NeuralNetwork loadedNet(inputLayers, outputLayers, hiddenLayers, hiddenLayerSize);

		currentSection.clear();

		std::getline(loadFile, line);
		if (line.find("input") != std::string::npos) {

			std::cout << "got input \n";

			std::getline(loadFile, line);

			//this piece of code cam from claude, and subsiquent others in this function 
			std::stringstream ss(line);
			float value;
			while (ss >> value) {

				currentSection.push_back(value);
				if (ss.peek() == ',') {
					ss.ignore();
				}
			}
			//to here

			loadedNet.setInputs(currentSection);

		}

		currentSection.clear();

		std::getline(loadFile, line);

		if (line.find("output") != std::string::npos) {

			std::cout << "got output \n";

			std::getline(loadFile, line);

			//this piece of code cam from claude, and subsiquent others in this function 
			std::stringstream ss(line);
			float value;
			while (ss >> value) {

				currentSection.push_back(value);
				if (ss.peek() == ',') {
					ss.ignore();
				}
			}
			//to here

			loadedNet.setOutputs(currentSection);
		}

		currentSection.clear();

		std::getline(loadFile, line);

		vector<vector<float>> hiddenLayersV;

		if (line.find("hidden") != std::string::npos) {

			std::cout << "got hidden \n";

			std::getline(loadFile, line);

			while (!(line.find("biases") != std::string::npos)) {
				//this piece of code cam from claude, and subsiquent others in this function 
				std::stringstream ss(line);
				float value;
				while (ss >> value) {

					currentSection.push_back(value);
					if (ss.peek() == ',') {
						ss.ignore();
					}
				}
				//to here
				
				hiddenLayersV.push_back(currentSection);
				std::getline(loadFile, line);
				currentSection.clear();

			}

			loadedNet.setHiddenLayers(hiddenLayersV);

		}

		currentSection.clear();

		std::getline(loadFile, line);

		if (line.find("biases") != std::string::npos) {

			std::cout << "got biases \n";

			std::getline(loadFile, line);

			//this piece of code cam from claude, and subsiquent others in this function 
			std::stringstream ss(line);
			float value;
			while (ss >> value) {

				currentSection.push_back(value);
				if (ss.peek() == ',') {
					ss.ignore();
				}
			}
			//to here

			loadedNet.setBiases(currentSection);
		}

		currentSection.clear();

		std::getline(loadFile, line);

		vector<vector<float>> weights;

		if (line.find("weights") != std::string::npos) {

			std::cout << "got weights \n";

			std::getline(loadFile, line);

			while (!(line.find("end") != std::string::npos)) {

				//this piece of code cam from claude, and subsiquent others in this function 
				std::stringstream ss(line);
				float value;
				while (ss >> value) {

					currentSection.push_back(value);
					if (ss.peek() == ',') {
						ss.ignore();
					}
				}
				//to here
				weights.push_back(currentSection);
				std::getline(loadFile, line);
				currentSection.clear();

			}

			loadedNet.setWeights(weights);

		}

		population.clear();

		for (int i = 0; i < populationSize; i++)
		{
			population.push_back(loadedNet);
		}

		StepToNextGeneration();

}

void GeneticManager::SortPopulation()
{
	sort(begin(population), end(population), SortDescending());
}

vector<NeuralNetwork> GeneticManager::Select()
{
	// Create a collection for all the selected networks
	vector<NeuralNetwork> genePool;

	// TODO -- Implement the selection function.

	// We need to select a number of the best performing agents and a number of the worst performing agents (for variety)
	// Hint: the number of agents to crossover select is contained in two variables, bestAgentSelection & worstAgentSelection

	//i think this is fine

	//best selection
	for (int i = 0; i < bestAgentSelection; i++) {

		//memory safetey
		if (i > population.size() - 1) {
			std::cout << "memory out of bounds genetic manager 1 \n";
			break;
		}

		genePool.push_back(NeuralNetwork(population.at(i)));

	}

	//if its the best of the best tripple it
//if (i == 0) {
//	for (int j = 0; j < 3; j++) {

//		float fitness = population.at(i).GetFitness();

//		//if (fitness > -0.001f && fitness < 0.001f) {
//		//	//strange convergence happening, i want to try get  rid of that
//		//	population.at(i).randomiseWeights();
//		//}

//		genePool.push_back(NeuralNetwork(population.at(i)));

//	}
//}
//else {
//	genePool.push_back(NeuralNetwork(population.at(i)));

//}

	//worst selection
	for (int i = 0; i < worstAgentSelection; i++) {
		//start at the end and work back for worst

		//memory safetey
		if (i > population.size() - 1) {
			std::cout << "memory out of bounds genetic manager 2 \n";
			break;
		}

		genePool.push_back(NeuralNetwork(population.at(population.size() - 1 - i)));
	}

	//forgot to add reset fitness
	for (NeuralNetwork& nn : genePool) {
		nn.SetFitness(0.f);
	}

	//genePool.push_back()

	// return the collection
	return genePool;
}

vector<NeuralNetwork> GeneticManager::Crossover(vector<NeuralNetwork>& genePool)
{

	//i think this is totally fine

	// Create vector for new members of the population
	vector<NeuralNetwork> newPopulation;

	//NeuralNetwork parent1, parent2;

	Randomise random;

	while (newPopulation.size() + genePool.size() < population.size()) {
		NeuralNetwork& parent1 = genePool[random.Int(0, genePool.size() - 1)];
		NeuralNetwork& parent2 = genePool[random.Int(0, genePool.size() - 1)];

		vector<vector<float>>& parent1Weights = parent1.GetWeights();
		vector<vector<float>>& parent2Weights = parent2.GetWeights();

		//copy over a parent for layers
		NeuralNetwork child(parent1);

		//faster than initialising i think
		vector<vector<float>> childNewWeights = parent1Weights;

		for (int i = 0; i < parent1Weights.size(); i++) {

			for (int j = 0; j < parent1Weights[i].size(); j++) {
				float randomParentWeight = random.Real(0.f, 1.f);

				try {
					parent2Weights.at(i).at(j);

				}
				catch (const std::out_of_range&) {
					break;
				}

				if (randomParentWeight > .5f) {

					childNewWeights[i][j] = parent1Weights[i][j];
				}
				else {
					childNewWeights[i][j] = parent2Weights[i][j];
				}
			}
		}

		newPopulation.push_back(child);
	}

	// Hint: the genePool vector contains all the parents. Randomly select two for creating a new solution
	// Remember that the newPopulation and the members of genePool are added together

	// Return the new members of the population
	return newPopulation;
}

void GeneticManager::Mutate(vector<NeuralNetwork>& newPopulation)
{
	// TODO -- Implement the mutation function

	//find the fraction of weightings against the mutation rate
	//generate that amount of random weightings to mutate

	//vector<vector<float>>& weights = newPopulation[0].GetWeights();

	Randomise random;

	for (NeuralNetwork& nn : newPopulation) {

		if (((random.Real(-1.0f, 1.0f) + 1.0f) / 2.0f) < mutationRate) {
			int amountOfMut = random.Int(1, (int)nn.GetWeights().size());

			for (int i = 0; i < amountOfMut; i++) {
				int layer = random.Int(1, (int)nn.GetWeights().size() - 1);
				int weight = random.Int(1, (int)nn.GetWeights().at(layer).size() - 1);

				nn.GetWeights().at(layer).at(weight) = random.Real(-1.f, 1.f);
			}
		}
	}


}
