#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


using std::vector;

class NeuralNetwork
{
public:
	NeuralNetwork(int inputNCount, int outputNCount, int hiddenLayerCount, int hiddenNCount);
	NeuralNetwork(const NeuralNetwork& n);
	void randomiseWeights();
	vector<float> RunNetwork(vector<float> inputs);
	const float GetFitness() const { return fitness; }
	void SetFitness(float newFitness) { fitness = newFitness; }
	vector<vector<float>>& GetWeights() { return weights; }

	void saveNetworkToFile();

	void setInputs(vector<float> input) { inputLayer = input; }
	void setOutputs(vector<float> output) { outputLayer = output; }
	void setHiddenLayers(vector<vector<float>> hidden) { hiddenLayers = hidden; }
	void setBiases(vector<float> bia) { biases = bia; }
	void setWeights(vector<vector<float>> wei) { weights = wei; }

private:
	void activationFunction(vector<float>& layer);
	void propagateLayer(vector<float> layer1, vector<float>& layer2, vector<float> weights, float bias);

private:
	vector<float> inputLayer, outputLayer, biases;
	vector<vector<float>> hiddenLayers, weights;
	float fitness;

	std::string filename = "NeuralNetwork";

};

