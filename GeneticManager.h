#pragma once
#include "NeuralNetwork.h"
#include <string>
#include <sstream>

class GeneticManager
{
public:
	GeneticManager(int inputLayers, int outputLayers, int hiddenLayers, int hiddenLayerSize, int popSize = 50);
	vector<float> StepToNextGeneration();
	const NeuralNetwork& GetFittestNetwork();
	float GetAverageFitness();
	vector<NeuralNetwork>& GetPopulation() { return population; }
	const int GetCurrentGeneration() const { return currentGeneration; }
	void saveFittestNetworkToFile();
	void loadNetwork();
private:
	// Sort function - sorts in descending levels of fitness
	class SortDescending
	{
	public:
		bool operator()(const NeuralNetwork& nn1, const NeuralNetwork& nn2)
		{
			return nn1.GetFitness() > nn2.GetFitness();
		}
	};
private:
	void SortPopulation();
	vector<NeuralNetwork> Select();
	vector<NeuralNetwork> Crossover(vector<NeuralNetwork>& genePool);
	void Mutate(vector<NeuralNetwork>& newPopulation);
private:
	int populationSize;
	float mutationRate;
	int bestAgentSelection;
	int worstAgentSelection;
	int currentGeneration;
	vector<int> genePool;
	vector<NeuralNetwork> population;
	std::string filename = "NeuralNetwork";

};