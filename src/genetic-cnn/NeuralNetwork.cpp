#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(int input_nodes, std::vector<int> hiddenLayer, int output, float minRan, float maxRan){
  m_input_nodes = input_nodes;
  m_output_nodes = output;
  m_layer_count = hiddenLayer.size() + 1;
  m_learning_rate = c_learningRate;
  m_minRan = minRan;
  m_maxRan = maxRan;
  m_hiddenLayer = hiddenLayer;
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

void NeuralNetwork::initRandValsdouble(arma::mat& matrix){
  matrix.for_each(m_random);
}

void NeuralNetwork::initRandAddGauss(arma::mat& matrix) {
  arma::mat t = matrix;
  t = arma::randg<arma::mat>(matrix.n_rows, matrix.n_cols);
  matrix += t;
}

void NeuralNetwork::train(std::vector<double> input, std::vector<double> target) {
  std::vector<double> output = feedForward(input);
  backpropagation(output, target);
  m_inputPerLayer.clear();
  m_outputPerLayer.clear();
}

std::vector<double> NeuralNetwork::feedForward(std::vector<double> input) {
  if (input.size() != m_input_nodes) {
    std::cerr << "Input-Error: Input amount does not match with the constructed input_nodes!";
    exit(EXIT_FAILURE);
  }
  arma::mat inputMat = arma::mat(input);
  arma::mat output;
  for (int layer = 0; layer < m_layer_count; layer++) { //Iterates through the layers
    m_inputPerLayer.push_back(inputMat);
    output = m_weights[layer] * inputMat;//Dot product Weights * Input
    output += m_biases[layer];           //Matrix addition (weights * Input) + Bias
    output.for_each(m_sigmoid);	         //Activation function
    m_outputPerLayer.push_back(output);
    inputMat = output;		         // Next Layer Output becomes the input
  }
  return matToVec1D(output);
}

std::vector<arma::mat>& NeuralNetwork::getWeights() {
  return m_weights;
}

std::vector<arma::mat>& NeuralNetwork::getBiases() {
  return m_biases;
}

void NeuralNetwork::setWeights(std::vector<arma::mat> newWeights) {
  m_weights = newWeights;
}

void NeuralNetwork::setBiases(std::vector<arma::mat> newBiases) {
  m_biases = newBiases;
}

void NeuralNetwork::backpropagation(std::vector<double> output, std::vector<double> target) {
  if (output.size() != target.size()) {
    std::cerr << "Error: Output and Target size does not match";
    exit(EXIT_FAILURE);
  }
  //ERROR = TARGETS - OUTPUTS
  std::vector< arma::Mat<double> >errors;
  errors.resize(m_layer_count);
  arma::Mat<double> errorOutput = arma::Mat<double>(output);
  arma::Mat<double> targetMat = arma::Mat<double>(target);
  arma::Mat<double> outputMat = arma::Mat<double>(output);
  errorOutput = targetMat - errorOutput;
  double weightSum = 0;
  arma::Mat<double> transposedWeight;
  for (int layer = m_layer_count - 1; - 1 < layer; layer--) {//Backtrace the error	
    if (layer == m_layer_count - 1) //error of the output 
      errors[layer] = errorOutput;
    else {
      transposedWeight = m_weights[layer + 1].t(); 
      errors[layer] = transposedWeight * errorOutput;
      errorOutput = errors[layer]; // Matrix of Errors of one layer
    }
  }
  arma::Mat<double> gradients;
  arma::Mat<double> inputsPerLyTrans;
  arma::Mat<double> deltaBias;
  arma::Mat<double> deltaWeight;
  for (int layer = m_layer_count - 1; -1 < layer; layer--) {//Calculate thr gradient descent
    gradients = m_outputPerLayer[layer];          //output of this current neuron as a start value
    gradients.for_each(m_dsigmoid);	          //pass the outputs in the derivative of sigma   
    gradients %= errors[layer];		          //Multiply it element wise with the error of this neuron  
    deltaBias = gradients; 
    deltaBias *= m_learning_rate;                 //Multiply the learning rate 
    m_biases[layer] += deltaBias;		  //change the bias by delta Bias	 
    inputsPerLyTrans = m_inputPerLayer[layer].t();//transpose the inputs 
    deltaWeight = gradients * inputsPerLyTrans;   //multiplay the transposed input
    deltaWeight *= m_learning_rate;               //Multiply the learning rate 
    m_weights[layer] += deltaWeight;              //change the weights by deltaWeight (gradient descent)      
  }
}

void NeuralNetwork::initWeights() {
  m_source_count.resize(m_layer_count);
  m_destination_count.resize(m_layer_count);
  m_weights.resize(m_layer_count);
  m_biases.resize(m_layer_count);
  m_source_count[0] = m_input_nodes;
  m_destination_count[0] = m_hiddenLayer[0];
  for (int i = 1; i < m_layer_count; i++) {
    if (i == m_layer_count - 1) {
      m_source_count[i] = m_hiddenLayer[i - 1];
      m_destination_count[i] = m_output_nodes;
    }
    else {
      m_destination_count[i] = m_hiddenLayer[i];
      m_source_count[i] = m_hiddenLayer[i - 1];
    }
  }
  for (int i = 0; i < m_layer_count; i++) {
    m_weights[i] = arma::Mat<double>(m_destination_count[i], m_source_count[i]);
    initRandValsdouble(m_weights[i]);
    m_biases[i] = arma::Mat<double>(m_destination_count[i], 1); // Always 1 dimensional
    initRandValsdouble(m_biases[i]);
  }
}

std::vector<double> NeuralNetwork::matToVec1D(arma::mat& A) {
  std::vector<double> V(A.n_rows);
  for (size_t i = 0; i < A.n_rows; ++i) {
    V[i] = arma::conv_to< double >::from(A.row(i));
  }
  return V;
}
