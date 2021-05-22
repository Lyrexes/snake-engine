#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP
#include "Matrix.hpp"
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

template<typename T, typename Distribution = std::void_t<>>
class NeuralNetwork {
  using Matrix_t = Matrix<T>;

public:
  NeuralNetwork(std::vector<unsigned int> layer, T minRan, T maxRan)
	  : m_layer(std::move(layer)),
		m_biases(m_layer.size() - 1), m_weights(m_layer.size() - 1) {
	for (int i = 0; i < m_layer.size() - 1; i++) {
	  m_weights[i] = Matrix_t(m_layer[i + 1], m_layer[i]);
	  m_weights[i].template fillRandom<Distribution>(minRan, maxRan);
	  m_biases[i] = Matrix_t(m_layer[i + 1], 1);
	  m_biases[i].template fillRandom<Distribution>(minRan, maxRan);
	}
  }
  explicit NeuralNetwork(std::vector<unsigned int> layer)
	  : m_layer(std::move(layer)),
		m_biases(m_layer.size() - 1), m_weights(m_layer.size() - 1) {
	for (int i = 0; i < m_layer.size() - 1; i++) {
	  m_weights[i] = Matrix_t(m_layer[i + 1], m_layer[i]);
	  m_biases[i] = Matrix_t(m_layer[i + 1], 1);// Always 1 dimensional
	}
  }
  NeuralNetwork(NeuralNetwork &&other) noexcept
	  : m_layer(std::move(m_layer)),
		m_biases(std::move(other.m_biases)),
		m_weights(std::move(other.m_weights)),
		m_fitness(other.m_fitness) {
	other.m_layer.clear();
	other.m_biases.clear();
	other.m_weights.clear();
	other.m_fitness = static_cast<T>(0);
  }
  NeuralNetwork& operator=(NeuralNetwork&& other)  noexcept {
	m_layer = std::move(other.m_layer);
	m_biases = std::move(other.m_biases);
	m_weights = std::move(other.m_weights);
	m_fitness = other.m_fitness;
	other.m_layer.clear();
	other.m_biases.clear();
	other.m_weights.clear();
	other.m_fitness = static_cast<T>(0);
	return *this;
  }
  NeuralNetwork(const NeuralNetwork &other)
	  : m_biases(other.m_biases),
		m_weights(other.m_weights),
		m_fitness(other.m_fitness),
		m_layer(other.m_layer) {}
  NeuralNetwork &operator=(const NeuralNetwork &other) {
	m_biases  = other.m_biases;
	m_weights = other.m_weights;
	m_layer   = other.m_layer;
	m_fitness = other.m_fitness;
	return *this;
  }
  std::vector<double> feedForward(std::vector<double> input) {
	Matrix_t inputMat = Matrix(std::move(input));
	Matrix_t output;
	Expects(inputMat.rawSize() == m_layer[0]);
	std::cout << m_weights.size() << std::endl;
	for (int layer = 0; layer < m_layer.size() - 1; layer++) {//Iterates through the layers
	  output = m_weights[layer] * inputMat;                   //Dot product Weights * Input
	  output += m_biases[layer];                              //Matrix addition (weights * Input) + Bias
	  output.forEach(m_sigmoid);                              //Activation function For each element
	  inputMat = output;                                      // Next Layer Output becomes the input
	}
	return matToVec1D(output);
  }
  template<typename Functor>
  std::vector<double> feedForward(std::vector<double> input, const Functor &functor) {
	Matrix_t inputMat = Matrix(std::move(input));
	Matrix_t output;
	Expects(inputMat.rawSize() == m_layer[0]);
	std::cout << m_weights.size() << std::endl;
	for (int layer = 0; layer < m_layer.size() - 1; layer++) {//Iterates through the layers
	  output = m_weights[layer] * inputMat;                   //Dot product Weights * Input
	  output += m_biases[layer];                              //Matrix addition (weights * Input) + Bias
	  output.forEach(functor);                                //Activation function For each element
	  inputMat = output;                                      // Next Layer Output becomes the input
	}
	return matToVec1D(output);
  }
  std::vector<Matrix_t> &getWeights() {
	return m_weights;
  }
  std::vector<Matrix_t> &getBiases() {
	return m_biases;
  }
  void setWeights(std::vector<Matrix_t> newWeights) {
	m_weights = std::move(newWeights);
  }
  void setBiases(std::vector<Matrix_t> newBiases) {
	m_biases = std::move(newBiases);
  }
  template<class Stream>
  friend Stream &operator<<(Stream &s, NeuralNetwork const &value) {
	s << "<NeuralNetwork at " << &value << '>' << std::endl
	  << "weights: " << std::endl;
	for (auto &&el : value.m_weights)
	  s << el;
	s << "biases : " << std::endl;
	for (auto &&el : value.m_biases)
	  s << el;
	return s;
  }
  void mutate(double mutationRate, T min, T max) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(mutationRate == std::clamp(mutationRate, 0.0, 1.0));
						BOOST_CONTRACT_ASSERT(min < max);
					  });
	auto const randomMutate = [&](auto &x) {
	  if (Random::get<bool>(mutationRate))
		x = Random ::get(min, max);
	};
	for (std::size_t i = 0; i < m_weights.size(); ++i) {
	  m_weights[i].forEach(randomMutate);
	  m_biases[i].forEach(randomMutate);
	}
  }
  template<class D>
  void mutate(double mutationRate, T min, T max) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(mutationRate == std::clamp(mutationRate, 0.0, 1.0));
						BOOST_CONTRACT_ASSERT(min < max);
					  });
	auto const randomMutate = [&](auto &x) {
	  if (Random::get<bool>(mutationRate))
		x = Random ::get<D>(min, max);
	};
	for (std::size_t i = 0; i < m_weights.size(); ++i) {
	  m_weights[i].forEach(randomMutate);
	  m_biases[i].forEach(randomMutate);
	}
  }
  template<typename Type, typename D = std::void_t<>>
  static NeuralNetwork crossover(const NeuralNetwork<Type, D> &lhs, double lhsFitness,
								 const NeuralNetwork<Type, D> &rhs, double rhsFitness) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(lhs.m_layer == rhs.m_layer);
					  });
	NeuralNetwork<Type, D> child(lhs.m_layer);
	auto crossoverRateLhs = 0.5;
	if (lhsFitness > rhsFitness)
	  crossoverRateLhs = 1.0 - (rhsFitness / lhsFitness);
	else if (lhsFitness < rhsFitness)
	  crossoverRateLhs = (lhsFitness / rhsFitness);

	for (std::size_t i = 0; i < lhs.m_weights.size(); ++i) {
	  if (Random::get<bool>(crossoverRateLhs)) {
		child.m_weights[i] = lhs.m_weights[i];
		child.m_biases[i] = lhs.m_biases[i];
	  } else {
		child.m_weights[i] = rhs.m_weights[i];
		child.m_biases[i] = rhs.m_biases[i];
	  }
	}
	return child;
  }
  static std::vector<T> matToVec1D(const Matrix_t &mat) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(mat.columns() == 1);
					  });
	return mat.column(0);
  }
  void setFitness(T fitness) {
	m_fitness = fitness;
  }
  T getFitness() {
	return m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator<(const NeuralNetwork<Type, D> &lhs,
						const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness < rhs.m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator>(const NeuralNetwork<Type, D> &lhs,
						const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness > rhs.m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator==(const NeuralNetwork<Type, D> &lhs,
						 const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness == rhs.m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator!=(const NeuralNetwork<Type, D> &lhs,
						 const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness != rhs.m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator<=(const NeuralNetwork<Type, D> &lhs,
						 const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness <= rhs.m_fitness;
  }
  template<typename Type, typename D>
  friend bool operator>=(const NeuralNetwork<Type, D> &lhs,
						 const NeuralNetwork<Type, D> &rhs) {
	return lhs.m_fitness >= rhs.m_fitness;
  }
private:
  std::vector<unsigned int> m_layer;
  std::vector<Matrix_t> m_biases;
  std::vector<Matrix_t> m_weights;
  T m_fitness{static_cast<T>(0)};
  constexpr static const auto m_sigmoid = [](T &x) { x = 1 / (1 + std::exp(x)); };
};
#endif
