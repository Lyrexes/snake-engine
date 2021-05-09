#include "NeuralNetwork.hpp"

NeuralNetwork::NeuralNetwork(std::vector<unsigned int> layer, float minRan, float maxRan){
  m_layer_count = layer.size();
  m_learning_rate = c_learningRate;
  m_minRan = minRan;
  m_maxRan = maxRan;
  m_layer = layer;
  m_sigmoid = [](double &x){ x = 1 / (1 + exp(-x)); };
  m_dsigmoid = [](double &x){ x = x * (1 - x); };
  //return sigmoid(x) * (1 - sigmoid(x));
  m_random = [minRan, maxRan](double& x) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double>
      dist(minRan, maxRan);
    x =  dist(rng);
  };
  initWeights();
}

void NeuralNetwork::initRandValsdouble(Matrix& matrix){
  matrix.for_each(m_random);
}

std::vector<double> NeuralNetwork::feedForward(std::vector<double> input) {
  if (input.size() != m_layer[0]) {
    std::cerr << "Input-Error: Input amount does not match with the constructed input_nodes!";
    exit(EXIT_FAILURE);
  }
  Matrix inputMat = Matrix(input);
  Matrix output;
  for (int layer = 0; layer < m_layer_count; layer++) { //Iterates through the layers
    m_inputPerLayer.push_back(inputMat);
    output = m_weights[layer] * inputMat;//Dot product Weights * Input
    output += m_biases[layer];           //Matrix addition (weights * Input) + Bias
    output.for_each(m_sigmoid);	         //Activation function For each element
    m_outputPerLayer.push_back(output);
    inputMat = output;		         // Next Layer Output becomes the input
  }
  return matToVec1D(output);
}

std::vector<Matrix>& NeuralNetwork::getWeights() {
  return m_weights;
}

std::vector<Matrix>& NeuralNetwork::getBiases() {
  return m_biases;
}

void NeuralNetwork::setWeights(std::vector<Matrix> newWeights) {
  m_weights = newWeights;
}

void NeuralNetwork::setBiases(std::vector<Matrix> newBiases) {
  m_biases = newBiases;
}

void NeuralNetwork::initWeights() {
  std::vector<unsigned int> source_count;
  std::vector<unsigned int> destination_count;
  source_count.resize(m_layer_count - 1);
  destination_count.resize(m_layer_count - 1);
  m_weights.resize(m_layer_count - 1);
  m_biases.resize(m_layer_count  - 1);

  for (int i = 0; i < m_layer_count - 1; i++) {
    source_count[i] = m_layer[i];
    destination_count[i] = m_layer[i + 1];
  }
  for (int i = 0; i < m_layer_count; i++) {
    m_weights[i] = Matrix(destination_count[i], source_count[i]);
    initRandValsdouble(m_weights[i]);
    m_biases[i] = Matrix(destination_count[i], 1); // Always 1 dimensional
    initRandValsdouble(m_biases[i]);
  }
}

std::vector<double> NeuralNetwork::matToVec1D(Matrix& A) {
  std::vector<double> V(A.n_rows);
  for (size_t row = 0; row < A.n_rows; ++row) {
    V[row] = a::conv_to<double>::from(A.row(row));
  }
  return V;
}
