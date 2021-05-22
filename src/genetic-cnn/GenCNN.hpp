#ifndef SNAKE_ENGINE_GCNN_HPP
#define SNAKE_ENGINE_GCNN_HPP

#include "NeuralNetwork.hpp"
#include <boost/contract.hpp>
#include <type_traits>
#include <vector>
#include <algorithm>

namespace ct = boost::contract;


template<typename PrecisionType, typename Distribution = std::void_t<>>
class GenCNN {
  using T = PrecisionType;
  using D = Distribution;
  using NeuralNetworks = std::vector<NeuralNetwork<T, D>>;

public:
  GenCNN(const std::vector<unsigned int>& layer, std::size_t populationSize,
		 T min = 0.0, T max = 1.0, T mutationRate=0.2)
	  : m_population{}, m_currentIndex(0) {
	m_population.reserve(populationSize);
	for(std::size_t i = 0; i < populationSize; ++i)
	  m_population.push_back(std::move(NeuralNetwork<T, D>(layer, min, max)));
  }
  void endNetworkRun(T fitness, T mutationRate, T min, T max) {
	ct::check c = ct::function()
					  .precondition([&] {
						BOOST_CONTRACT_ASSERT(m_currentIndex <= m_population.size());
					  });
	m_population[m_currentIndex].setFitness(fitness);
	if (m_currentIndex == m_population.size() - 1) {
	  naturalSelection();
	  mutate(mutationRate, min, max);
	  m_currentIndex = 0;
	} else
	  ++m_currentIndex;
  }
  NeuralNetworks naturalSelection() {
	const auto maxFitness = std::max_element(m_population.begin(), m_population.end())->getFitness();
	NeuralNetworks nextGeneration;
	nextGeneration.reserve(m_population.size());
	for() {

	}

  }
  void mutate(T mutationRate,T min, T max) {
	for(auto &el : m_population)
	  el.mutate(mutationRate, min, max);
  }
private:
  NeuralNetworks m_population;
  std::size_t m_currentIndex;
};

#endif// SNAKE_ENGINE_GCNN_HPP
