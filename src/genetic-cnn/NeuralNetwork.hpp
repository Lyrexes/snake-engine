#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <functional>
#include <armadillo>

namespace a = arma;
using Matrix = a::mat;

class NeuralNetwork {
public:
  //Only the height
  // Input, HiddenLayer, OutputLayer
  NeuralNetwork(std::vector<unsigned int> layer, float minRan, float maxRan);
  void initRandValsdouble(Matrix& matrix);
  std::vector<double> feedForward(std::vector<double> input);
  std::vector<Matrix>& getWeights();
  std::vector<Matrix>& getBiases();
  void setWeights(std::vector<Matrix> newWeights);
  void setBiases(std::vector<Matrix> newBiases);
private:
  void initWeights();
  std::vector<double> matToVec1D(Matrix& A);
private:
  std::vector<Matrix> m_outputPerLayer;
  std::vector<Matrix> m_inputPerLayer;
  std::function<void(double&)> m_sigmoid;
  std::function<void(double&)> m_dsigmoid;
  std::function<void(double&)> m_random;
  std::function<void(double&)> m_randomG;
  double m_learning_rate;
  int m_hiddenHeight_nodes;
  int m_hiddenCount_nodes;
  int m_layer_count;
  std::vector<unsigned int> m_layer;
  std::vector<Matrix> m_biases;
  std::vector<Matrix> m_weights;
  float m_minRan;
  float m_maxRan;
private:
  constexpr const static double c_learningRate = 0.3;
};

