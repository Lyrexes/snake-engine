#pragma once
#include <json/json.h>
#include <armadillo>
#include <array>
#include <vector>
#include <cmath>
#include <functional>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#define CONST_ONE 1
#define LEARNING_RATE 0.1


class NeuralNetwork {

public://Only the height
	NeuralNetwork(int input_nodes, std::vector<int> hiddenLayer, int output, float minRan, float maxRan ); // Input, HiddenLayer, OutputLayer
	NeuralNetwork();
	~NeuralNetwork();
	void initRandValsdouble(arma::mat& matrix);
	void initRandAddGauss(arma::mat& matrix);
	void train(std::vector<double> input, std::vector<double> target);
	std::vector<double> feedForward(std::vector<double> input);
	std::vector<arma::mat>& getWeights();
	std::vector<arma::mat>& getBiases();
	void setWeights(std::vector<arma::mat> newWeights);
	void setBiases(std::vector<arma::mat> newBiases);
	void backpropagation(std::vector<double> output, std::vector<double> target);
	void loading();
	void saving();
private:
	void initWeights();
	std::vector< arma::mat > outputPerLayer;
	std::vector< arma::mat > inputPerLayer;
	std::vector<double> matToVec1D(arma::mat& A);
	std::function<void(double&)> sigmoid;
	std::function<void(double&)> dsigmoid;
	std::function<void(double&)> random;
	std::function<void(double&)> randomG;
	double learning_rate;
	int input_nodes;
	int hiddenHeight_nodes;
	int hiddenCount_nodes;
	int output_nodes;
	int layer_count;
	std::vector<int> hiddenLayer;
	std::vector<arma::mat> biases;
	std::vector<int> destination_count;
	std::vector<int> source_count;
	std::vector<arma::mat> weights;
	float minRan;
	float maxRan;
};

