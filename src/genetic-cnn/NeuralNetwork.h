#pragma once
#include <json/json.h>
#include <armadillo>
#include <array>
#include <vector>
#include <cmath>
#include <functional>


class NeuralNetwork {
public:
  //Only the height
  // Input, HiddenLayer, OutputLayer
  NeuralNetwork(int input_nodes, std::vector<int> hiddenLayer, int output, float minRan, float maxRan );
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
private:
  void initWeights();
  std::vector<double> matToVec1D(arma::mat& A);
private:
  std::vector< arma::mat > m_outputPerLayer;
  std::vector< arma::mat > m_inputPerLayer;
  std::function<void(double&)> m_sigmoid;
  std::function<void(double&)> m_dsigmoid;
  std::function<void(double&)> m_random;
  std::function<void(double&)> m_randomG;
  double m_learning_rate;
  int m_input_nodes;
  int m_hiddenHeight_nodes;
  int m_hiddenCount_nodes;
  int m_output_nodes;
  int m_layer_count;
  std::vector<int> m_hiddenLayer;
  std::vector<arma::mat> m_biases;
  std::vector<int> m_destination_count;
  std::vector<int> m_source_count;
  std::vector<arma::mat> m_weights;
  float m_minRan;
  float m_maxRan;
private:
  constexpr const static double c_learningRate = 0.3;
};

