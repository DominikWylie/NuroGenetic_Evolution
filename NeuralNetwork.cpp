#include "NeuralNetwork.h"

#include "NeuralNetwork.h"
#include <cmath>
#include "Randomise.h"

NeuralNetwork::NeuralNetwork(int inputNeuronCount, int outputNeuronCount, int hiddenLayerCount, int hiddenNeuronCount)
	: fitness(0.f)
{
	// Create the vectors for the input layer, output layer and all the hidden layers
	inputLayer = vector<float>(inputNeuronCount);
	outputLayer = vector<float>(outputNeuronCount);
	hiddenLayers = vector<vector<float>>(hiddenLayerCount);

	// Add all the weights to the weight vector
	for (int i = 0; i < hiddenLayerCount; i++)
	{
		hiddenLayers.at(i) = vector<float>(hiddenNeuronCount);

		// Input to hidden
		if (i == 0)
		{
			vector<float> inputToHidden(inputNeuronCount * hiddenNeuronCount);
			weights.push_back(inputToHidden);
		}

		vector<float> hiddenWeights(hiddenNeuronCount * hiddenNeuronCount);
		weights.push_back(hiddenWeights);
	}

	vector<float> outputWeights(hiddenNeuronCount * outputNeuronCount);
	weights.push_back(outputWeights);

	// Create a bias vector
	biases = vector<float>(hiddenLayerCount + 2);

	// Self-explanatory
	randomiseWeights();
}

// Copy Constructor - Useful for making copies
NeuralNetwork::NeuralNetwork(const NeuralNetwork& nn)
{
	fitness = nn.fitness;
	inputLayer = nn.inputLayer;
	outputLayer = nn.outputLayer;
	hiddenLayers = nn.hiddenLayers;
	weights = nn.weights;
	biases = nn.biases;
}

vector<float> NeuralNetwork::RunNetwork(vector<float> inputs)
{
	// Check we have the correct number of inputs, otherwise return 0
	if (inputs.size() != inputLayer.size())
		return vector<float>(outputLayer.size(), 0);

	// Assign the input values directly into the input layuer
	inputLayer = inputs;

	// Call the activation function on the input layer
	activationFunction(inputLayer);

	// Move data from input to the first hidden layer and activate it
	propagateLayer(inputLayer, hiddenLayers[0], weights[0], biases[0]);
	activationFunction(hiddenLayers[0]);

	// For each hidden layer, propagate data to the next and call the activation function on that layer
	for (size_t i = 1; i < hiddenLayers.size(); i++)
	{
		propagateLayer(hiddenLayers[i - 1], hiddenLayers[i], weights[i], biases[i]);
		activationFunction(hiddenLayers[i]);
	}

	// Move data from the last hidden layer to the output layer and activate again
	propagateLayer(hiddenLayers[hiddenLayers.size() - 1], outputLayer, weights[weights.size() - 1], biases[biases.size() - 1]);
	activationFunction(outputLayer);

	// Return the output
	return outputLayer;
}

void NeuralNetwork::randomiseWeights()
{
	biases.clear();

	Randomise randomise;

	for (vector<float>& layer : weights) {
		for (float& weight : layer) {
			weight = randomise.Real(-0.7f, 0.7f);
		}
		biases.push_back(randomise.Real(-0.7f, 0.7f));
	}

}


void NeuralNetwork::saveNetworkToFile()
{
	//std::ifstream file(filename);

	int nextNum = 0;
	do {
		nextNum++;
	} while (std::ifstream(filename + std::to_string(nextNum) + ".txt").good());

	std::ofstream outputNetFile(filename + std::to_string(nextNum) + ".txt");

	if (outputNetFile.is_open()) {
		// input, output, hidden layers, hidden layer size, biases
		outputNetFile << "stats : \n";
		outputNetFile <<
			inputLayer.size() << ", " <<
			outputLayer.size() << ", " <<
			hiddenLayers.size() << ", " <<
			hiddenLayers.at(0).size() << "\n";

		//input
		outputNetFile << "input: \n";

		for (float input : inputLayer) {
			outputNetFile << input << ", ";
		}
		outputNetFile << "\n";

		//output
		outputNetFile << "output: \n";

		for (float output : outputLayer) {
			outputNetFile << output << ", ";
		}
		outputNetFile << "\n";

		//hidden layers
		outputNetFile << "hidden: \n";

		for (vector<float> hiddenL : hiddenLayers) {
			for (float hidden : hiddenL) {
				outputNetFile << hidden << ", ";
			}
			outputNetFile << "\n";
		}

		//biases
		outputNetFile << "biases: \n";

		for (float bias : biases) {
			outputNetFile << bias << ", ";
		}
		outputNetFile << "\n";

		//weights
		outputNetFile << "weights: \n";

		for (vector<float> weightL : weights) {
			for (float weight : weightL) {
				outputNetFile << weight << ", ";
			}
			outputNetFile << "\n";
		}

		outputNetFile << "end\n";

		outputNetFile.close();
		std::cout << "saved best net to file \n";
	}
}

void NeuralNetwork::activationFunction(vector<float>& layer)
{
	//come back and have a play with different functions once it works

	for (float& item : layer)
		item = tanh(item);

	//for (float& x : layer)
	//{
	//	x = std::max(0.f, x);
	//}
}

void NeuralNetwork::propagateLayer(vector<float> layer1, vector<float>& layer2, vector<float> weights, float bias)
{
	for (int i = 0; i < layer2.size(); i++) {
		for (int j = 0; j < layer1.size(); j++) {
			//layer2i += layer1i * weight[j * sizeof(layer2) + i]

			layer2[i] = 0.f;

			layer2[i] += layer1[j] * weights[j * layer2.size() + i];
		}
		layer2[i] += bias;
	}
}

//void NeuralNetwork::propagateLayer(vector<float> layer1, vector<float>& layer2, vector<float> weights, float bias)
//{
//	for (size_t i = 0; i < layer2.size(); i++)
//	{
//		float& value = layer2.at(i);
//		value = 0.f;
//
//		for (size_t j = 0; j < layer1.size(); j++)
//			value += layer1.at(j) * weights.at((j * layer2.size()) + i);
//
//		value += bias;
//	}
//}